
#ifndef INIT_H
#define INIT_H
#include <storage/storage.h>

#include "../enums.h"

void tasks_init(Tasks *tasks);
void tasks_add(Tasks *tasks, const Task *task);
void tasks_free(Tasks *tasks);

bool write_task_to_csv(File *file, const Task *task);
bool read_tasks_from_csv(File *file, Tasks *tasks);
#endif // INIT_H