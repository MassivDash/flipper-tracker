// task.c
#include "../app.h"
#include "../enums.h"
#include <furi.h>
#include <stdlib.h>
#include <storage/storage.h>

#define TAG "tracker_app"

bool find_and_replace_task_in_csv(App *app, const Task *current_task) {
    FURI_LOG_I(TAG, "Finding and replacing task with ID: %s", current_task->id);
    furi_assert(app);

    // Open the CSV file for reading
    if (!storage_file_open(app->file, APP_DATA_PATH("data.csv"), FSAM_READ, FSOM_OPEN_EXISTING)) {
        FURI_LOG_E(TAG, "Failed to open file for reading");
        return false;
    }

    // Allocate buffers for reading and writing
    FuriString *buffer_task = furi_string_alloc();
    FuriString *buffer_rest = furi_string_alloc();
    bool task_found = false;

    // Read each line and check if it matches the task_id
    while (read_line_from_file(app, buffer_task)) {
        Task task = {0};
        const char *buffer_str = furi_string_get_cstr(buffer_task);

        sscanf(buffer_str, "%49[^,],%49[^,],%99[^,],%lf,%19[^,],%19[^,],%19[^,],%d,%u",
               task.id, task.name, task.description, &task.price_per_hour,
               task.start_time, task.end_time, task.last_start_time,
               (int *)&task.completed, &task.total_time_minutes);

        if (strcmp(task.id, current_task->id) == 0) {
            task_found = true;
            furi_string_printf(buffer_rest, "%s,%s,%s,%.2f,%s,%s,%s,%d,%u\n",
                               current_task->id, current_task->name,
                               current_task->description, current_task->price_per_hour,
                               current_task->start_time, current_task->end_time,
                               current_task->last_start_time, current_task->completed,
                               current_task->total_time_minutes);
        } else {
            furi_string_printf(buffer_rest, "%s\n", buffer_str);
        }
    }

    storage_file_close(app->file);

    if (task_found) {
        // Reopen the file for writing
        if (!storage_file_open(app->file, APP_DATA_PATH("data.csv"), FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
            FURI_LOG_E(TAG, "Failed to open file for writing");
            furi_string_free(buffer_task);
            furi_string_free(buffer_rest);
            return false;
        }

        // Write the updated content back to the file
        if (!storage_file_write(app->file, furi_string_get_cstr(buffer_rest), furi_string_size(buffer_rest))) {
            FURI_LOG_E(TAG, "Failed to write new content to file");
            furi_string_free(buffer_task);
            furi_string_free(buffer_rest);
            storage_file_close(app->file);
            return false;
        }

        storage_file_close(app->file);
    }

    furi_string_free(buffer_task);
    furi_string_free(buffer_rest);
    FURI_LOG_I(TAG, "Task find and replace completed. Task found: %d", task_found);
    return task_found;
}