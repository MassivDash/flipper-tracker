#include "../../app.h"
#include "../../structs.h"
#include "../../tasks/task.h"
#include <furi.h>
#include <furi_hal.h>
#include <gui/modules/submenu.h>

#define TAG "tracker_app"

void submenu_callback_view_tasks(void *context, uint32_t index) {
  FURI_LOG_T(TAG, "submenu_callback_view_tasks");
  furi_assert(context);
  App *app = context;

  if (index > 0 && index <= app->tasks->size) {
    Task *task = &app->tasks->array[index - 1];
    FURI_LOG_I(TAG, "Switching to task view for task: %s", task->name);
    app->current_task = task;
    scene_manager_handle_custom_event(app->scene_manager, AppEvent_TaskActions);
  } else {
    FURI_LOG_W(TAG, "Invalid task index: %lu", (unsigned long)index);
  }
}

void scene_on_enter_view_tasks(void *context) {
  FURI_LOG_T(TAG, "scene_on_enter_view_task");
  App *app = context;
  submenu_reset(app->submenu);
  submenu_set_header(app->submenu, "Tasks");

  if (app->tasks->array != NULL) {
    // First, add incomplete tasks
    for (size_t i = 0; i < app->tasks->size; i++) {
      Task *task = &app->tasks->array[i];

      if (!task->completed) {
        FURI_LOG_I(TAG, "task: %lu", (unsigned long)i);
        char task_label[256];
        snprintf(task_label, sizeof(task_label), "%s (%s)", task->name,
                 task_status_to_string(task->status));
        submenu_add_item(app->submenu, task_label, i + 1,
                         submenu_callback_view_tasks, app);
      }
    }
    // Then, add completed tasks
    for (size_t i = 0; i < app->tasks->size; i++) {
      Task *task = &app->tasks->array[i];
      if (task->completed == true || task->completed == 1) {
        FURI_LOG_I(TAG, "task: %lu (completed)", (unsigned long)i);
        char completed_task_name[256];
        snprintf(completed_task_name, sizeof(completed_task_name),
                 "%s (completed)", task->name);
        submenu_add_item(app->submenu, completed_task_name, i + 1,
                         submenu_callback_view_tasks, app);
      }
    }
  } else {
    submenu_add_item(app->submenu, "No tasks", 0, NULL, app);
  }

  view_dispatcher_switch_to_view(app->view_dispatcher, AppView_ViewTasks);
}

bool scene_on_event_view_tasks(void *context, SceneManagerEvent event) {
  FURI_LOG_T(TAG, "scene_on_event_view_task");
  App *app = context;
  bool consumed = false;
  switch (event.type) {
  case SceneManagerEventTypeCustom:
    switch (event.event) {
    case AppEvent_TaskActions:
      // Add log before switching to TaskActions
      FURI_LOG_I(TAG, "Switching to TaskActions");
      scene_manager_next_scene(app->scene_manager, TaskActions);
      consumed = true;
      break;
    }
    break;

  default: // eg. SceneManagerEventTypeBack, SceneManagerEventTypeTick
    consumed = false;
    break;
  }
  return consumed;
}

void scene_on_exit_view_tasks(void *context) {
  FURI_LOG_T(TAG, "scene_on_exit_view_task");
  App *app = context;
  submenu_reset(app->submenu);
}