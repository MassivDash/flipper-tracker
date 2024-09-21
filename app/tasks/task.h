
#ifndef TASK_H
#define TASK_H
#include "../app.h"
#include <storage/storage.h>

#include "../enums.h"

void tasks_init(App *app);
void tasks_add(App *app, const Task *task);
void current_task_init(App *app);
void current_task_free(Task *task);
void tasks_free(Tasks *tasks);
bool write_task_to_csv(App *app, const Task *task);
bool read_tasks_from_csv(App *app);
bool delete_task_from_csv(App *app, const char *task_id);
bool find_and_replace_task_in_csv(App *app, const Task *current_task);
#endif // TASK_H