#include "Task.h"

TaskScheduler::Task *TaskScheduler::Task::tasks[TASK_MAX_TASKS] = {};
uint8_t TaskScheduler::Task::tasks_amount = 0;

TaskScheduler::Task::Task(const char *name, void (*fnc)()) : ESP32Time()
{
    _name = name;
    _fnc = fnc;

    if (tasks_amount == TASK_MAX_TASKS)
    {
        return;
    }

    tasks[tasks_amount++] = this;
}

void TaskScheduler::Task::getName(char *buf)
{
    memcpy(buf, _name, TASK_NAME_LENGTH);
}

bool TaskScheduler::Task::isExecutable()
{
    // return false always when task is deactivated
    if (!_is_active)
    {
        return _is_active;
    }

    String date = getTime("%F");
    date.replace("-", "");
    long current_date = date.toInt();
    
    date = getTime("%T");
    date.replace(":", "");
    long current_time = date.toInt();

    // check if task was run during a day
    if (_last_run_date == current_date)
    {
        return false;
    }

    uint8_t day_of_week = getDayofWeek();
    return _schedule[day_of_week] <= current_time;
}

bool TaskScheduler::Task::loadConfig()
{
    config_status_t status = Config::load(_name, "tasks");
    if (!status == CONFIG_LOADED)
    {
        Serial.printf("Error loading %s.json file. Status code: %d\n", _name, status);
        return false;
    }
    
    Config *config = Config::getByName(_name);

    JsonArray schedules = config->data.as<JsonArray>();
    int day = 0;
    for (JsonVariant entry : schedules)
    {
        uint16_t hour = entry["hour"];
        schedule(day, hour);
        Serial.printf("Scheduled task %s: day %d hour %d\n", _name, day, hour);
        day++;
    }
    
    return true;
}

void TaskScheduler::Task::execute()
{
    String date = getTime("%F");
    date.replace("-", "");
    long current_date = date.toInt();
    
    date = getTime("%T");
    date.replace(":", "");
    long current_time = date.toInt();

    _last_run_date = current_date;
    _last_run_time = current_time;

    forceExecute();
}

void TaskScheduler::Task::schedule(uint16_t hour)
{
    for (uint8_t i = 0; i < 7; i++)
    {
        schedule(i, hour);
    }
}