#include "TaskScheduler.h"

TaskScheduler::Scheduler &TaskScheduler::Scheduler::getInstance()
{
    static Scheduler instance;
    return instance;
}

void TaskScheduler::Scheduler::loop()
{
    if (millis() - _last_scan < TASK_SCHEDULER_SCAN_INTERVAL)
    {
        return;
    }

    for (int i = 0; i < Task::tasks_amount; i++)
    {
        Task *task = Task::tasks[i];

        if (task->isExecutable())
        {
            char task_name[TASK_NAME_LENGTH + 1];
            task->getName(task_name);
            Serial.printf("Task %s is starting\n", task_name);

            task->execute();
        }
    }
    _last_scan = millis();
}