#include "../../app.h"
#include "../../enums.h"
#include "../../tasks/task.h"
#include <furi.h>
#include <furi_hal.h>
#include <gui/modules/submenu.h>

#define TAG "tracker_app"

void submenu_callback_show_stats(void *context, uint32_t index) {
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
  case ShowStats_Menu:
    scene_manager_handle_custom_event(app->scene_manager, AppEvent_ShowStats);
    break;
  case Exit_Menu:
    scene_manager_handle_back_event(app->scene_manager);
    break;
  }
}

void scene_on_enter_show_stats(void *context) {
  FURI_LOG_T(TAG, "scene_on_enter_show_stats");
  App *app = context;
  submenu_reset(app->submenu);
  submenu_set_header(app->submenu, "Show stats");

  if (app->tasks->array != NULL) {
    for (size_t i = 0; i < app->tasks->size; i++) {
      FURI_LOG_I(TAG, "task: %d", i);
      Task *task = &app->tasks->array[i];
      submenu_add_item(app->submenu, task->name, i + 1,
                       submenu_callback_show_stats, app);
    }
  } else {
    submenu_add_item(app->submenu, "No tasks", 0, submenu_callback_show_stats,
                     app);
  }

  view_dispatcher_switch_to_view(app->view_dispatcher, AppView_ShowStats);
}

bool scene_on_event_show_stats(void *context, SceneManagerEvent event) {
  FURI_LOG_T(TAG, "scene_on_event_show_stats");
  App *app = context;
  bool consumed = false;
  switch (event.type) {
  case SceneManagerEventTypeCustom:
    switch (event.event) {
    case AppEvent_ShowStats:
      scene_manager_next_scene(app->scene_manager, ShowStats);
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

void scene_on_exit_show_stats(void *context) {
  FURI_LOG_T(TAG, "scene_on_exit_show_stats");
  App *app = context;
  submenu_reset(app->submenu);
}
