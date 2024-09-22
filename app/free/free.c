#include "../app.h"
#include "../structs.h"
#include "../tasks/task.h"
#include <furi.h>
#include <gui/gui.h>
#include <gui/icon_i.h>
#include <gui/modules/menu.h>
#include <gui/modules/popup.h>
#include <gui/modules/submenu.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>
#include <storage/storage.h>

#define TAG "tracker_app"

void app_free(App *app) {
  FURI_LOG_T(TAG, "free");
  scene_manager_free(app->scene_manager);
  view_dispatcher_remove_view(app->view_dispatcher, AppView_Menu);
  view_dispatcher_remove_view(app->view_dispatcher, AppView_Popup);
  view_dispatcher_remove_view(app->view_dispatcher, AppView_ViewTasks);
  view_dispatcher_remove_view(app->view_dispatcher, AppView_TaskActions);
  view_dispatcher_remove_view(app->view_dispatcher, AppView_TaskContinue);
  view_dispatcher_free(app->view_dispatcher);
  menu_free(app->menu);
  popup_free(app->popup);
  submenu_free(app->submenu);
  submenu_free(app->submenu_task_actions);
  dialog_ex_free(app->dialog);
  tasks_free(app->tasks);
  current_task_free(app->current_task);
  furi_record_close(RECORD_STORAGE);
  storage_file_free(app->file);
  free(app);
}