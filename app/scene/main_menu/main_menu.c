#include "../../app.h"
#include "../../structs.h"
#include "../../tasks/task.h"
#include <datetime/datetime.h>
#include <furi.h>

#define TAG "tracker_app"

/** main menu callback - sends a custom event to the scene manager based on the
 * menu selection */
void menu_callback_main_menu(void *context, uint32_t index) {
  FURI_LOG_T(TAG, "menu_callback_main_menu");
  App *app = context;
  switch (index) {
  case QuickStart_Menu:
    scene_manager_handle_custom_event(app->scene_manager,
                                      AppEvent_ShowQuickStart);
    break;
  case CreateTask_Menu:
    scene_manager_handle_custom_event(app->scene_manager,
                                      AppEvent_ShowCreateTask);
    break;
  case ViewTasks_Menu:
    scene_manager_handle_custom_event(app->scene_manager, AppEvent_ViewTasks);
    break;
  }
}

void scene_on_enter_main_menu(void *context) {
  FURI_LOG_T(TAG, "scene_on_enter_main_menu");
  App *app = context;
  menu_reset(app->menu);

  // NB. icons are specified as NULL below, because:
  // * icons are _always_ animated by the menu
  // * the icons provided (&I_one, &I_two) are generated by the build process
  // * these icons do not have a framerate (resulting in a division by zero)

  // Check if any tasks are in app->tasks loaded from the csv file
  // Check if any tasks are in app->tasks loaded from the csv file
  if (app->tasks->array != NULL) {
    // Find the latest not completed task
    Task *latest_task = NULL;
    uint32_t latest_task_timestamp = 0;
    for (size_t i = 0; i < app->tasks->size; i++) {
      Task *task = &app->tasks->array[i];
      if (!task->completed) {
        uint32_t task_timestamp =
            datetime_datetime_to_timestamp(&task->last_start_time);
        if (latest_task == NULL || task_timestamp > latest_task_timestamp) {
          latest_task = task;
          latest_task_timestamp = task_timestamp;
        }
      }
    }

    if (latest_task != NULL) {
      app->current_task = latest_task;
      menu_add_item(app->menu, "Continue last task", NULL, QuickStart_Menu,
                    menu_callback_main_menu, app);
    }
  }

  menu_add_item(app->menu, "Create a new task", NULL, CreateTask_Menu,
                menu_callback_main_menu, app);
  menu_add_item(app->menu, "View tasks", NULL, ViewTasks_Menu,
                menu_callback_main_menu, app);

  view_dispatcher_switch_to_view(app->view_dispatcher, AppView_Menu);
}

/** main menu event handler - switches scene based on the event */
bool scene_on_event_main_menu(void *context, SceneManagerEvent event) {
  FURI_LOG_T(TAG, "scene_on_event_main_menu");
  App *app = context;
  bool consumed = false;
  switch (event.type) {
  case SceneManagerEventTypeCustom:
    switch (event.event) {
    case AppEvent_ShowQuickStart:
      scene_manager_next_scene(app->scene_manager, TaskActions);
      consumed = true;
      break;
    case AppEvent_ShowCreateTask:

      if (create_new_task(app)) {
        scene_manager_next_scene(app->scene_manager, TaskActions);
      }

      consumed = true;
      break;
    case AppEvent_ViewTasks:
      scene_manager_next_scene(app->scene_manager, ViewTasks);
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

void scene_on_exit_main_menu(void *context) {
  FURI_LOG_T(TAG, "scene_on_exit_main_menu");
  App *app = context;
  menu_reset(app->menu);
}