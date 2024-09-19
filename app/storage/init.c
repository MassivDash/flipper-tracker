#include "../app.h"
#include "../enums.h"
#include "../tasks/task.h"
#include <storage/storage.h>

#define TAG "tracker_app"

void storage_init(App *app) {

  if (!app) {
    FURI_LOG_E(TAG, "App is NULL");
    return;
  }

  Task test_task = {.name = "Test Task",
                    .description = "This is a test task",
                    .price_per_hour = 25.0,
                    .start_time = "2023-10-01 10:00",
                    .end_time = "2023-10-01 12:00",
                    .total_time_minutes = 120};

  // Open storage
  Storage *storage = furi_record_open(RECORD_STORAGE);

  // Allocate file
  File *file = storage_file_alloc(storage);

  // Check if the file exists
  if (storage_file_exists(storage, APP_DATA_PATH("data.csv"))) {
    FURI_LOG_I(TAG, "File exists, closing and freeing memory");

    // Read file from csv and store in tasks

    if (!read_tasks_from_csv(file, app->tasks)) {
      FURI_LOG_E(TAG, "Failed to read tasks from CSV file");
    }

    storage_file_close(file);
  } else {
    // File does not exist, create the file with one test entry
    FURI_LOG_I(TAG, "File does not exist, creating new file with test entry");

    if (!storage_file_open(file, APP_DATA_PATH("data.csv"), FSAM_WRITE,
                           FSOM_CREATE_ALWAYS)) {
      FURI_LOG_E(TAG, "Failed to open file for writing");
    }

    // Write the test task to the file
    if (!write_task_to_csv(file, &test_task)) {
      FURI_LOG_E(TAG, "Failed to write test task to CSV file");
    }

    if (!write_task_to_csv(file, &test_task)) {
      FURI_LOG_E(TAG, "Failed to write test task to CSV file");
    }

    storage_file_close(file);
  }

  // Deallocate file
  storage_file_free(file);
}