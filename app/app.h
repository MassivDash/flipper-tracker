#include <gui/modules/menu.h>
#include <gui/modules/popup.h>
#include <gui/modules/submenu.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>
#include <storage/storage.h>

#include "enums.h"

// app/app.h
#ifndef APP_H
#define APP_H

typedef struct {
  SceneManager *scene_manager;
  ViewDispatcher *view_dispatcher;
  Menu *menu;
  Submenu *submenu;
  Popup *popup;
  Tasks *tasks;
} App;

#endif // APP_H