#include "../../app.h"
#include "../../enums.h"
#include <furi.h>
#include <furi_hal.h>
#include <gui/modules/submenu.h>

#define TAG "tracker_app"

void submenu_callback_view_tasks(void *context, uint32_t index) {
  FURI_LOG_T(TAG, "menu_callback_main_menu");
  furi_assert(context);
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
    scene_manager_handle_custom_event(app->scene_manager,
                                      AppEvent_ShowCreateTask);
    break;
  }
}

void scene_on_enter_view_tasks(void *context) {
  FURI_LOG_T(TAG, "scene_on_enter_view_task");
  App *app = context;
  submenu_reset(app->submenu);
  submenu_set_header(app->submenu, "Show stats");

  if (app->tasks->array != NULL) {
    for (size_t i = 0; i < app->tasks->size; i++) {
      FURI_LOG_I(TAG, "task: %d", i);
      Task *task = &app->tasks->array[i];
      submenu_add_item(app->submenu, task->name, i + 1,
                       submenu_callback_view_tasks, app);
    }
  } else {
    submenu_add_item(app->submenu, "No tasks", 0, submenu_callback_view_tasks,
                     app);
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

void scene_on_exit_view_tasks(void *context) {
  FURI_LOG_T(TAG, "scene_on_exit_view_task");
  App *app = context;
  submenu_reset(app->submenu);
}
