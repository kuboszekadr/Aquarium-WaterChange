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

    uint8_t day_of_week = getDayofWeek();
    
    String date = getTime("%F");
    date.replace("-", "");
    uint16_t current_date = date.toInt();
    
    date = getTime("%T");
    date.replace(":", "");
    uint16_t current_time = date.toInt();

    // check if task was run during a day
    if (_last_run_date == current_date)
    {
        return false;
    }

    return _schedule[day_of_week] <= current_time;
}

void TaskScheduler::Task::execute()
{
    String date = getTime("%F");
    date.replace("-", "");
    uint16_t current_date = date.toInt();
    
    date = getTime("%T");
    date.replace(":", "");
    uint16_t current_time = date.toInt();

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