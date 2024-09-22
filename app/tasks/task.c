#include "../app.h"
#include "../csv/csv.h"
#include <furi.h>
#include <furi_hal.h>
#include <stdlib.h>
#include <storage/storage.h>

#define TAG "tracker_app"
#define ID_LENGTH 6

void generate_id(char *id, size_t length) {
  const char charset[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  size_t charset_size = sizeof(charset) - 1;

  uint32_t random_value = furi_hal_random_get();

  for (size_t i = 0; i < length; ++i) {
    int key = random_value % charset_size;
    id[i] = charset[key];
    random_value /= charset_size;
  }
  id[length] = '\0'; // Null-terminate the string
}

void tasks_init(App *app) {
  app->tasks = malloc(sizeof(Tasks));
  app->tasks->array = NULL;
  app->tasks->size = 0;
  app->tasks->capacity = 0;
}

void current_task_init(App *app) {
  app->current_task = malloc(sizeof(Task));
  app->current_task->id[0] = '\0';
  app->current_task->name[0] = '\0';
  app->current_task->description[0] = '\0';
  app->current_task->price_per_hour = 0.0;
  app->current_task->start_time[0] = '\0';
  app->current_task->end_time[0] = '\0';
  app->current_task->last_start_time[0] = '\0';
  app->current_task->completed = false;
  app->current_task->total_time_minutes = 0;
  app->current_task->status = TaskStatus_Stopped;
}

void current_task_update(App *app, const Task *current_task) {
  if (app->current_task == NULL) {
    app->current_task = malloc(sizeof(Task));
  }
  *app->current_task = *current_task;
}

void current_task_free(Task *current_task) {
  free(current_task);
  current_task = NULL;
}

void tasks_add(App *app, const Task *task) {
  Tasks *tasks = app->tasks;
  if (tasks->size == tasks->capacity) {
    tasks->capacity = tasks->capacity == 0 ? 1 : tasks->capacity * 2;
    Task *new_array = realloc(tasks->array, tasks->capacity * sizeof(Task));
    if (new_array == NULL) {
      FURI_LOG_E(TAG, "Failed to allocate memory for tasks array");
      return;
    }
    tasks->array = new_array;
  }
  tasks->array[tasks->size++] = *task;
}

void get_current_datetime(char *datetime_str, size_t size) {
  DateTime datetime;
  furi_hal_rtc_get_datetime(&datetime);
  snprintf(datetime_str, size, "%04d-%02d-%02d %02d:%02d", datetime.year,
           datetime.month, datetime.day, datetime.hour, datetime.minute);
}

void create_new_task(App *app) {
  char id[ID_LENGTH + 1]; // 6 characters + 1 for the null terminator
  generate_id(id, ID_LENGTH);

  char nameId[50];
  generate_id(nameId, ID_LENGTH);

  // "New Task" with the random name
  Task default_task = {0}; // Initialize all fields to zero
  strncpy(default_task.id, id, ID_LENGTH + 1);
  strncpy(default_task.name, nameId, 50);

  // Copy the description into the array
  strncpy(default_task.description, "Very flipppery important task",
          sizeof(default_task.description) - 1);
  default_task.description[sizeof(default_task.description) - 1] =
      '\0'; // Ensure null-termination

  // Copy the start_time, end_time, and last_start_time into the arrays

  default_task.start_time[sizeof(default_task.start_time) - 1] =
      '\0'; // Ensure null-termination

  get_current_datetime(default_task.start_time,
                       sizeof(default_task.start_time));

  strncpy(default_task.end_time, "NULL", sizeof(default_task.end_time) - 1);
  default_task.end_time[sizeof(default_task.end_time) - 1] =
      '\0'; // Ensure null-termination

  default_task.last_start_time[sizeof(default_task.last_start_time) - 1] =
      '\0'; // Ensure null-termination

  get_current_datetime(default_task.last_start_time,
                       sizeof(default_task.last_start_time));

  default_task.price_per_hour = 0;
  default_task.completed = false;
  default_task.total_time_minutes = 0;
  default_task.status = TaskStatus_Stopped;

  write_task_to_csv(app, &default_task);
  tasks_add(app, &default_task);
  current_task_update(app, &default_task);
}

void tasks_update(App *app, const Task *current_task) {
  Tasks *tasks = app->tasks;
  bool task_found = false;

  for (size_t i = 0; i < tasks->size; ++i) {
    if (strcmp(tasks->array[i].id, current_task->id) == 0) {
      tasks->array[i] = *current_task;
      task_found = true;
      FURI_LOG_I(TAG, "Task with ID %s updated.", current_task->id);
      break;
    }
  }

  if (!task_found) {
    FURI_LOG_E(TAG, "Task with ID %s not found.", current_task->id);
  }
}

void tasks_remove(App *app, const char *task_id) {
  Tasks *tasks = app->tasks;
  bool task_found = false;

  for (size_t i = 0; i < tasks->size; ++i) {
    if (strcmp(tasks->array[i].id, task_id) == 0) {
      task_found = true;
      // Shift the remaining tasks to fill the gap
      for (size_t j = i; j < tasks->size - 1; ++j) {
        tasks->array[j] = tasks->array[j + 1];
      }
      tasks->size--;
      FURI_LOG_I(TAG, "Task with ID %s removed.", task_id);
      break;
    }
  }

  if (!task_found) {
    FURI_LOG_E(TAG, "Task with ID %s not found.", task_id);
  }
}

void tasks_free(Tasks *tasks) {
  free(tasks->array);
  tasks->array = NULL;
  tasks->size = 0;
  tasks->capacity = 0;
}

TaskStatus string_to_task_status(const char *status_str) {
  if (strcmp(status_str, "Running") == 0)
    return TaskStatus_Running;
  if (strcmp(status_str, "Stopped") == 0)
    return TaskStatus_Stopped;
  return TaskStatus_Stopped; // Default to Stopped if unknown
}

const char *task_status_to_string(TaskStatus status) {
  switch (status) {
  case TaskStatus_Running:
    return "Running";
  case TaskStatus_Stopped:
    return "Stopped";
  default:
    return "Stopped";
  }
}