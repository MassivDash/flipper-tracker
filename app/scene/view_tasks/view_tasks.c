#include "../../app.h"
#include "../../structs.h"
#include "../../tasks/task.h"
#include <furi.h>
#include <furi_hal.h>
#include <gui/modules/submenu.h>

#define TAG "tracker_app"

void sort_tasks(App *app) {
  size_t incomplete_index = 0;
  size_t completed_index = app->tasks->size - 1;

  while (incomplete_index < completed_index) {
    while (incomplete_index < app->tasks->size &&
           !app->tasks->array[incomplete_index].completed) {
      incomplete_index++;
    }
    while (completed_index > 0 &&
           app->tasks->array[completed_index].completed) {
      completed_index--;
    }
    if (incomplete_index < completed_index) {
      Task temp = app->tasks->array[incomplete_index];
      app->tasks->array[incomplete_index] = app->tasks->array[completed_index];
      app->tasks->array[completed_index] = temp;
    }
  }
}

void submenu_callback_view_tasks(void *context, uint32_t index) {
  FURI_LOG_T(TAG, "submenu_callback_view_tasks");
  furi_assert(context);
  App *app = context;

  if (index > 0 && index <= app->tasks->size) {
    Task task = app->tasks->array[index - 1];
    current_task_update(app, &task);
    scene_manager_handle_custom_event(app->scene_manager, AppEvent_TaskActions);
  } else {
    FURI_LOG_W(TAG, "Invalid task index: %lu", (unsigned long)index);
  }
}

void submenu_callback_no_tasks(void *context, uint32_t index) {
  FURI_LOG_T(TAG, "submenu_callback_no_tasks");
  furi_assert(index);
  App *app = context;
  scene_manager_search_and_switch_to_previous_scene(app->scene_manager,
                                                    MainMenu);
}

void scene_on_enter_view_tasks(void *context) {
  FURI_LOG_T(TAG, "scene_on_enter_view_task");
  App *app = context;
  submenu_reset(app->submenu);
  submenu_set_header(app->submenu, "Tasks");

  if (app->tasks->array != NULL && app->tasks->size > 0) {

    // Sort tasks to push completed tasks to the bottom
    // sort_tasks(app);

    // Add tasks to submenu
    for (size_t i = 0; i < app->tasks->size; i++) {
      Task *task = &app->tasks->array[i];

      FURI_LOG_I(TAG, "Task %lu: Name: %s, Status: %s, Completed: %s",
                 (unsigned long)i, task->name,
                 task_status_to_string(task->status),
                 task->completed ? "Yes" : "No");

      char task_label[128];
      if (!task->completed) {
        snprintf(task_label, sizeof(task_label), "%s (%s)", task->name,
                 task_status_to_string(task->status));
      } else {
        snprintf(task_label, sizeof(task_label), "%s (completed)", task->name);
      }
      submenu_add_item(app->submenu, task_label, i + 1,
                       submenu_callback_view_tasks, app);
    }
  } else {
    submenu_add_item(app->submenu, "No tasks", NoTask_Menu,
                     submenu_callback_no_tasks, app);
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
  for (size_t i = 0; i < app->tasks->size; i++) {
    Task task = app->tasks->array[i];

    FURI_LOG_T(TAG, "Task %lu: Name: %s, Status: %s, Completed: %s",
               (unsigned long)i, task.name, task_status_to_string(task.status),
               task.completed ? "Yes" : "No");
    break;
  }

  submenu_reset(app->submenu);
}