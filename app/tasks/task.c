#include "../app.h"
#include "../enums.h"
#include <furi.h>

#define TAG "tracker_app"

void tasks_init(App *app) {
  app->tasks = malloc(sizeof(Tasks));
  app->tasks->array = NULL;
  app->tasks->size = 0;
  app->tasks->capacity = 0;
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

void tasks_free(App *app) {
  Tasks *tasks = app->tasks;
  free(tasks->array);
  tasks->array = NULL;
  tasks->size = 0;
  tasks->capacity = 0;
}

bool write_task_to_csv(File *file, const Task *task) {
  // Allocate a FuriString buffer
  FuriString *buffer = furi_string_alloc();

  // Format the task into the buffer
  furi_string_printf(buffer, "%s,%s,%s,%.2f,%s,%s,%u\n", task->id, task->name,
                     task->description, (double)task->price_per_hour,
                     task->start_time, task->end_time,
                     task->total_time_minutes);

  // Write the buffer to the file
  if (!storage_file_write(file, furi_string_get_cstr(buffer),
                          furi_string_size(buffer))) {
    FURI_LOG_E(TAG, "Failed to write task to file");
    furi_string_free(buffer);
    return false;
  }

  // Free the buffer
  furi_string_free(buffer);
  return true;
}

static bool read_line_from_file(File *file, FuriString *str_result) {
  furi_string_reset(str_result);
  uint8_t buffer[32];
  bool result = false;

  do {
    size_t read_count = storage_file_read(file, buffer, sizeof(buffer));
    if (storage_file_get_error(file) != FSE_OK) {
      return false;
    }

    for (size_t i = 0; i < read_count; i++) {
      if (buffer[i] == '\n') {
        uint32_t position = storage_file_tell(file);
        if (storage_file_get_error(file) != FSE_OK) {
          return false;
        }

        position = position - read_count + i + 1;
        storage_file_seek(file, position, true);
        if (storage_file_get_error(file) != FSE_OK) {
          return false;
        }

        result = true;
        break;
      } else {
        furi_string_push_back(str_result, buffer[i]);
      }
    }

    if (result || read_count == 0) {
      break;
    }
  } while (true);

  return result;
}

// This is wrong FIX !!
bool read_tasks_from_csv(File *file, App *app) {
  // Allocate a FuriString buffer
  FuriString *buffer = furi_string_alloc();

  // Read the file line by line
  while (read_line_from_file(file, buffer)) {
    // Allocate a task
    Task task = {0};

    // Allocate memory for task fields
    // Ensure task fields are large enough to hold the data
    strncpy(task.id, "", sizeof(task.id));
    strncpy(task.name, "", sizeof(task.name));
    strncpy(task.description, "", sizeof(task.description));
    strncpy(task.start_time, "", sizeof(task.start_time));
    strncpy(task.end_time, "", sizeof(task.end_time));

    sscanf(furi_string_get_cstr(buffer),
           "%49[^,],%63[^,],%147[^,],%f,%19[^,],%19[^,],%u", task.id, task.name,
           task.description, &task.price_per_hour, task.start_time,
           task.end_time, &task.total_time_minutes);

    // Add the task to the tasks
    tasks_add(app, &task);
  }

  // Free the buffer
  furi_string_free(buffer);
  return true;
}
