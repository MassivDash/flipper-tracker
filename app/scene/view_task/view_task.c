#include "../../app.h"
#include "../../csv/csv.h"
#include "../../structs.h"
#include "../../tasks/task.h"
#include <furi.h>
#include <furi_hal.h>
#include <gui/modules/submenu.h>

#define TAG "tracker_app"

void submenu_callback_task_actions(void *context, uint32_t index) {
  FURI_LOG_T(TAG, "submenu_callback_task_actions");
  furi_assert(context);
  App *app = context;

  // Ensure app and its members are properly initialized
  if (!app || !app->current_task) {
    FURI_LOG_E(TAG, "Invalid app or uninitialized current_task");
    return;
  }

  switch (index) {
  case TaskAction_Continue:
    // Handle "Continue" action
    FURI_LOG_I(TAG, "Continue task: %s", app->current_task->name);
    // Add your logic here
    break;
  case TaskAction_Edit:
    // Handle "Edit" action
    FURI_LOG_I(TAG, "Edit task: %s", app->current_task->name);
    scene_manager_set_scene_state(app->scene_manager, ViewTaskDetails, 0);
    scene_manager_next_scene(app->scene_manager, ViewTaskDetails);
    break;
  case TaskAction_Stats:
    // Handle "Stats" action
    FURI_LOG_I(TAG, "Stats for task: %s", app->current_task->name);
    // Add your logic here
    break;
  case TaskAction_ToggleCompleted:
    // Handle "Toggle Completed" action
    FURI_LOG_I(TAG, "Toggle completed for task: %s", app->current_task->name);
    app->current_task->completed = !app->current_task->completed;
    // Update the submenu to reflect the new state
    submenu_change_item_label(
        app->submenu_task_actions, TaskAction_ToggleCompleted,
        app->current_task->completed ? "Unmark as done" : "Mark as done");

    // Log the task completion status

    FURI_LOG_I(TAG, "Task %s marked as %s", app->current_task->name,
               app->current_task->completed ? "done" : "not done");

    // Update the task in the CSV file
    find_and_replace_task_in_csv(app, app->current_task);
    tasks_update(app, app->current_task);

    break;
  default:
    break;
  }
}

void scene_on_enter_task_actions(void *context) {
  FURI_LOG_T(TAG, "scene_on_enter_task_actions");
  App *app = context;
  furi_assert(app);

  // Ensure app and its members are properly initialized
  if (!app || !app->current_task) {
    FURI_LOG_E(TAG, "Invalid app or uninitialized current_task");
    return;
  }

  submenu_reset(app->submenu_task_actions);

  // Set the header to the current task name
  Task *task = app->current_task;
  char header[256];
  snprintf(header, sizeof(header), "Task: %s", task->name);
  submenu_set_header(app->submenu_task_actions, header);
  submenu_add_item(app->submenu_task_actions, "Continue", TaskAction_Continue,
                   submenu_callback_task_actions, app);
  submenu_add_item(app->submenu_task_actions, "Edit", TaskAction_Edit,
                   submenu_callback_task_actions, app);
  submenu_add_item(app->submenu_task_actions, "Stats", TaskAction_Stats,
                   submenu_callback_task_actions, app);

  // Add the "Mark as done" or "Unmark as done" item based on the completed flag
  if (task->completed) {
    submenu_add_item(app->submenu_task_actions, "Unmark as done",
                     TaskAction_ToggleCompleted, submenu_callback_task_actions,
                     app);
  } else {
    submenu_add_item(app->submenu_task_actions, "Mark as done",
                     TaskAction_ToggleCompleted, submenu_callback_task_actions,
                     app);
  }

  view_dispatcher_switch_to_view(app->view_dispatcher, AppView_TaskActions);
}

bool scene_on_event_task_actions(void *context, SceneManagerEvent event) {
  FURI_LOG_T(TAG, "scene_on_event_task_actions");
  App *app = context;
  furi_assert(app);

  bool consumed = false;
  switch (event.type) {
  case SceneManagerEventTypeCustom:
    switch (event.event) {
    case AppEvent_TaskActions:
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

void scene_on_exit_task_actions(void *context) {
  FURI_LOG_T(TAG, "scene_on_exit_task_actions");
  App *app = context;
  submenu_reset(app->submenu_task_actions);
}