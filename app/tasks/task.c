#include "../app.h"
#include "../enums.h"
#include <furi.h>

#define TAG "tracker_app"

void tasks_init(App *app) {
  if (app->tasks == NULL) {
    app->tasks = malloc(sizeof(Tasks));
    if (app->tasks == NULL) {
      FURI_LOG_E(TAG, "Failed to allocate memory for tasks");
      return;
    }
  }
  app->tasks->array = NULL;
  app->tasks->size = 0;
  app->tasks->capacity = 0;
}

void tasks_add(Tasks *tasks, const Task *task) {
  if (tasks->size == tasks->capacity) {
    tasks->capacity = tasks->capacity == 0 ? 1 : tasks->capacity * 2;
    tasks->array = realloc(tasks->array, tasks->capacity * sizeof(Task));
  }
  tasks->array[tasks->size++] = *task;
}

void tasks_free(Tasks *tasks) {
  free(tasks->array);
  tasks->array = NULL;
  tasks->size = 0;
  tasks->capacity = 0;
}

bool write_task_to_csv(File *file, const Task *task) {
  // Allocate a FuriString buffer
  FuriString *buffer = furi_string_alloc();

  // Format the task into the buffer
  furi_string_printf(buffer, "%s,%s,%.2f,%s,%s,%u\n", task->name,
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

bool read_tasks_from_csv(File *file, Tasks *tasks) {
  // Allocate a FuriString buffer
  FuriString *buffer = furi_string_alloc();
  if (buffer == NULL) {
    FURI_LOG_E(TAG, "Failed to allocate FuriString buffer");
    return false;
  }

  // Read the file line by line
  while (read_line_from_file(file, buffer)) {
    // Allocate a task
    Task task = {0};

    FURI_LOG_E(TAG, "Failed to allocate FuriString buffer");
    // Parse the task from the buffer
    sscanf(furi_string_get_cstr(buffer), "%[^,],%[^,],%f,%[^,],%[^,],%u",
           task.name, task.description, &task.price_per_hour, task.start_time,
           task.end_time, &task.total_time_minutes);

    // Add the task to the tasks
    tasks_add(tasks, &task);
  }

  // Free the buffer
  furi_string_free(buffer);
  return true;
}