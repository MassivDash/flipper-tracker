
#ifndef TASK_H
#define TASK_H
#include "../app.h"
#include <storage/storage.h>

#include "../enums.h"

void tasks_init(App *app);
void tasks_add(App *app, const Task *task);
void tasks_free(App *app);
bool write_task_to_csv(App *app, const Task *task);
bool read_tasks_from_csv(App *app);
#endif // TASK_H