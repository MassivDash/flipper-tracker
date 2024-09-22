#include <gui/modules/dialog_ex.h>
#include <gui/modules/menu.h>
#include <gui/modules/popup.h>
#include <gui/modules/submenu.h>
// #include <gui/modules/variable_item_list.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>
#include <storage/storage.h>

#include "structs.h"

// app/app.h
#ifndef APP_H
#define APP_H

typedef struct {
  SceneManager *scene_manager;
  ViewDispatcher *view_dispatcher;
  Menu *menu;
  Submenu *submenu;
  Submenu *submenu_task_actions;
  Popup *popup;
  Tasks *tasks;
  Task *current_task;
  File *file;
  DialogEx *dialog;
} App;

#endif // APP_H