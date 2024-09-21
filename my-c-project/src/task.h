// task.h

#ifndef TASK_H
#define TASK_H

#include <stdbool.h>

typedef struct {
    char id[50];
    char name[100];
    char description[200];
    float price_per_hour;
    char start_time[20];
    char end_time[20];
    char last_start_time[20];
    bool completed;
    unsigned int total_time_minutes;
} Task;

typedef struct {
    Task *array;
    size_t size;
    size_t capacity;
} Tasks;

void tasks_init(App *app);
void current_task_init(App *app);
void current_task_free(Task *current_task);
void tasks_add(App *app, const Task *task);
void tasks_update(App *app, const Task *current_task);
void tasks_remove(App *app, const char *task_id);
void tasks_free(Tasks *tasks);
bool write_task_to_csv(App *app, const Task *task);
bool read_tasks_from_csv(App *app);
bool find_and_replace_task_in_csv(App *app, const Task *current_task);

#endif // TASK_H