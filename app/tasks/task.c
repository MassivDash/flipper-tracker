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

void tasks_free(Tasks *tasks) {
  free(tasks->array);
  tasks->array = NULL;
  tasks->size = 0;
  tasks->capacity = 0;
}

bool write_task_to_csv(App *app, const Task *task) {
  // Allocate a FuriString buffer
  FuriString *buffer = furi_string_alloc();

  // Format the task into the buffer
  furi_string_printf(buffer, "%s,%s,%s,%.2f,%s,%s,%s,%d,%u,\n", task->id,
                     task->name, task->description,
                     (double)task->price_per_hour, task->start_time,
                     task->end_time, task->last_start_time, task->completed,
                     task->total_time_minutes);

  // Write the buffer to the file
  if (!storage_file_write(app->file, furi_string_get_cstr(buffer),
                          furi_string_size(buffer))) {
    FURI_LOG_E(TAG, "Failed to write task to file");
    furi_string_free(buffer);
    return false;
  }

  // Free the buffer
  furi_string_free(buffer);
  return true;
}

static bool read_line_from_file(App *app, FuriString *str_result) {
  furi_string_reset(str_result);
  uint8_t buffer[1]; // Read one character at a time
  bool result = false;

  FURI_LOG_I(TAG, "starting to read the file");

  while (true) {
    size_t read_count = storage_file_read(app->file, buffer, sizeof(buffer));
    FURI_LOG_I(TAG, "read count: %zu", read_count);

    if (read_count == 0) {
      break; // End of file
    }

    if (buffer[0] == '\n') {
      result = true;
      break; // End of line
    } else {
      furi_string_push_back(str_result, buffer[0]);
    }
  }

  // Ensure the string is null-terminated
  furi_string_push_back(str_result, '\0');

  FURI_LOG_I(TAG, "result: %s", furi_string_get_cstr(str_result));
  return result;
}

bool delete_task_from_csv(App *app, const char *task_id) {
  // Allocate a FuriString buffer for reading lines
  FuriString *buffer = furi_string_alloc();
  // Allocate a FuriString buffer for writing new content
  FuriString *new_content = furi_string_alloc();

  // Ensure the file is open before reading
  if (!storage_file_open(app->file, APP_DATA_PATH("data.csv"), FSAM_READ,
                         FSOM_OPEN_EXISTING)) {
    FURI_LOG_E(TAG, "Failed to open file");
    furi_string_free(buffer);
    furi_string_free(new_content);
    return false;
  }

  bool task_found = false;

  // Read each line and check if it matches the task_id
  while (read_line_from_file(app, buffer)) {
    char current_id[50] = {0};
    sscanf(furi_string_get_cstr(buffer), "%49[^,]", current_id);

    if (strcmp(current_id, task_id) != 0) {
      // If the current line does not match the task_id, append it to
      // new_content
      furi_string_cat(new_content, furi_string_get_cstr(buffer));
      furi_string_push_back(new_content, '\n');
    } else {
      task_found = true;
    }
  }

  // Close the file after reading
  storage_file_close(app->file);

  if (task_found) {
    // Reopen the file for writing
    if (!storage_file_open(app->file, APP_DATA_PATH("data.csv"), FSAM_WRITE,
                           FSOM_OPEN_EXISTING)) {
      FURI_LOG_E(TAG, "Failed to open file for writing");
      furi_string_free(buffer);
      furi_string_free(new_content);
      return false;
    }

    // Write the new content to the file
    if (!storage_file_write(app->file, furi_string_get_cstr(new_content),
                            furi_string_size(new_content))) {
      FURI_LOG_E(TAG, "Failed to write new content to file");
      furi_string_free(buffer);
      furi_string_free(new_content);
      return false;
    }

    // Close the file after writing
    storage_file_close(app->file);
  }

  // Free the buffers
  furi_string_free(buffer);
  furi_string_free(new_content);

  return task_found;
}

bool read_tasks_from_csv(App *app) {
  // Allocate a FuriString buffer
  FuriString *buffer = furi_string_alloc();
  // Ensure the file is open before reading
  if (!storage_file_open(app->file, APP_DATA_PATH("data.csv"), FSAM_READ,
                         FSOM_OPEN_EXISTING)) {
    FURI_LOG_E(TAG, "Failed to open file");
    furi_string_free(buffer);
    return false;
  }

  furi_assert(app);
  FURI_LOG_I(TAG, "read_tasks_from_csv");

  while (read_line_from_file(app, buffer)) {
    FURI_LOG_I(TAG, "while");
    // Allocate a task
    Task task = {0};

    // Ensure the buffer is null-terminated
    const char *buffer_str = furi_string_get_cstr(buffer);
    FURI_LOG_I(TAG, "Buffer string: %s", buffer_str);

    // Temporary string to hold price_per_hour
    char price_per_hour_str[20] = {0};

    sscanf(buffer_str,
           "%49[^,],%49[^,],%99[^,],%19[^,],%19[^,],%19[^,],%19[^,],%d,%u",
           task.id, task.name, task.description, price_per_hour_str,
           task.start_time, task.end_time, task.last_start_time,
           (int *)&task.completed, &task.total_time_minutes);

    // Convert price_per_hour_str to float
    task.price_per_hour = strtof(price_per_hour_str, NULL);

    // LOG NAME FOR TESTING
    FURI_LOG_I(TAG,
               "Task details: ID=%s, Name=%s, Description=%s, "
               "PricePerHour=%.2f, StartTime=%s, EndTime=%s, LastStartTime=%s, "
               "Completed=%d, TotalTimeMinutes=%u",
               task.id, task.name, task.description,
               (double)task.price_per_hour, task.start_time, task.end_time,
               task.last_start_time, task.completed, task.total_time_minutes);

    // Add the task to the tasks
    tasks_add(app, &task);
  }

  // Free the buffer
  FURI_LOG_I(TAG, "closed file");
  furi_string_free(buffer);
  storage_file_close(app->file);
  return true;
}