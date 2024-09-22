
#ifndef TASK_H
#define TASK_H
#include "../app.h"
#include <storage/storage.h>

#include "../structs.h"

void create_new_task(App *app);
void tasks_init(App *app);
void tasks_add(App *app, const Task *task);
void tasks_update(App *app, const Task *current_task);
void tasks_remove(App *app, const char *task_id);
void current_task_init(App *app);
void current_task_free(Task *task);
void current_task_update(App *app, const Task *current_task);
void tasks_free(Tasks *tasks);

#endif // TASK_H