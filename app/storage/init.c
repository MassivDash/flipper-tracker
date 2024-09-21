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

  // Test task remove after development
  Task test_task = {.id = "ID1",
                    .name = "Test Task",
                    .description = "This is a test task",
                    .price_per_hour = 25.0,
                    .start_time = "2023-10-01 10:00",
                    .end_time = "2023-10-01 12:00",
                    .last_start_time = "2023-10-01 10:00",
                    .completed = false,
                    .total_time_minutes = 120};

  Task test_task2 = {.id = "ID2",
                     .name = "Test Task2",
                     .description = "This is a test task",
                     .price_per_hour = 25.0,
                     .start_time = "2023-10-01 10:00",
                     .end_time = "2023-10-01 12:00",
                     .last_start_time = "2023-10-01 10:00",
                     .completed = true,
                     .total_time_minutes = 120};

  // Open storage
  Storage *storage = furi_record_open(RECORD_STORAGE);

  // Allocate file
  app->file = storage_file_alloc(storage);

  // Check if the file exists
  if (storage_file_exists(storage, APP_DATA_PATH("data.csv"))) {
    FURI_LOG_I(TAG, "File exists, reading tasks from file");

    if (!read_tasks_from_csv(app)) {
      FURI_LOG_E(TAG, "Failed to read tasks from CSV file");
    }
  } else {
    // File does not exist, create the file with one test entry
    FURI_LOG_I(TAG, "File does not exist, creating new file with test entry");

    // Write the test task to the file
    if (!write_task_to_csv(app, &test_task)) {
      FURI_LOG_E(TAG, "Failed to write test task to CSV file");
    }

    FURI_LOG_I(
        TAG, "2n entry File does not exist, creating new file with test entry");

    if (!write_task_to_csv(app, &test_task2)) {
      FURI_LOG_E(TAG, "Failed to write test task to CSV file");
    }
  }
}