#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"

#define TAG "tracker_app"

void initialize_app(App *app) {
    app->tasks = malloc(sizeof(Tasks));
    app->tasks->array = NULL;
    app->tasks->size = 0;
    app->tasks->capacity = 0;
    app->current_task = malloc(sizeof(Task));
    strcpy(app->current_task->id, "");
    strcpy(app->current_task->name, "");
    strcpy(app->current_task->description, "");
    app->current_task->price_per_hour = 0.0;
    strcpy(app->current_task->start_time, "");
    strcpy(app->current_task->end_time, "");
    strcpy(app->current_task->last_start_time, "");
    app->current_task->completed = false;
    app->current_task->total_time_minutes = 0;
}

void free_app(App *app) {
    tasks_free(app->tasks);
    free(app->current_task);
}

void handle_user_input(App *app) {
    // Placeholder for user input handling logic
}

int main() {
    App *app = malloc(sizeof(App));
    initialize_app(app);

    // Main application loop
    while (1) {
        handle_user_input(app);
        // Add logic to exit the loop
    }

    free_app(app);
    free(app);
    return 0;
}