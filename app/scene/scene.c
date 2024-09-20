
#include "../app.h"
#include "../enums.h"                // Has all the Id's / Enums
#include "./view_tasks/view_tasks.h" // scenes
// scenes
#include "./main_menu/main_menu.h"
#include "./quick_start/quick_start.h"

#define TAG "tracker_app"

#include <trackerflipx_icons.h> // created later after ubft run

/** collection of all scene on_enter handlers - in the same order as their enum
 */
void (*const scene_on_enter_handlers[])(void *) = {
    scene_on_enter_main_menu, scene_on_enter_popup_one,
    scene_on_enter_popup_two, scene_on_enter_view_tasks};

/** collection of all scene on event handlers - in the same order as their enum
 */
bool (*const scene_on_event_handlers[])(void *, SceneManagerEvent) = {
    scene_on_event_main_menu, scene_on_event_popup_one,
    scene_on_event_popup_two, scene_on_event_view_tasks};

/** collection of all scene on exit handlers - in the same order as their enum
 */
void (*const scene_on_exit_handlers[])(void *) = {
    scene_on_exit_main_menu, scene_on_exit_popup_one, scene_on_exit_popup_two,
    scene_on_exit_view_tasks};

/** collection of all on_enter, on_event, on_exit handlers */
const SceneManagerHandlers scene_event_handlers = {
    .on_enter_handlers = scene_on_enter_handlers,
    .on_event_handlers = scene_on_event_handlers,
    .on_exit_handlers = scene_on_exit_handlers,
    .scene_num = Count};

/** custom event handler - passes the event to the scene manager */
bool scene_manager_custom_event_callback(void *context, uint32_t custom_event) {
  FURI_LOG_T(TAG, "scene_manager_custom_event_callback");
  furi_assert(context);
  App *app = context;
  return scene_manager_handle_custom_event(app->scene_manager, custom_event);
}

/** navigation event handler - passes the event to the scene manager */
bool scene_manager_navigation_event_callback(void *context) {
  FURI_LOG_T(TAG, "scene_manager_navigation_event_callback");
  furi_assert(context);
  App *app = context;
  return scene_manager_handle_back_event(app->scene_manager);
}

/** initialise the scene manager with all handlers */
void scene_manager_init(App *app) {
  FURI_LOG_T(TAG, "scene_manager_init");
  app->scene_manager = scene_manager_alloc(&scene_event_handlers, app);
}

/** initialise the views, and initialise the view dispatcher with all views */
void view_dispatcher_init(App *app) {
  FURI_LOG_T(TAG, "view_dispatcher_init");
  app->view_dispatcher = view_dispatcher_alloc();

  // allocate each view
  FURI_LOG_D(TAG, "view_dispatcher_init allocating views");
  app->menu = menu_alloc();
  app->popup = popup_alloc();
  app->submenu = submenu_alloc();

  // assign callback that pass events from views to the scene manager
  FURI_LOG_D(TAG, "view_dispatcher_init setting callbacks");
  view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
  view_dispatcher_set_custom_event_callback(
      app->view_dispatcher, scene_manager_custom_event_callback);
  view_dispatcher_set_navigation_event_callback(
      app->view_dispatcher, scene_manager_navigation_event_callback);

  // add views to the dispatcher, indexed by their enum value
  FURI_LOG_D(TAG, "view_dispatcher_init adding view menu");
  view_dispatcher_add_view(app->view_dispatcher, AppView_Menu,
                           menu_get_view(app->menu));

  FURI_LOG_D(TAG, "view_dispatcher_init adding view popup");
  view_dispatcher_add_view(app->view_dispatcher, AppView_Popup,
                           popup_get_view(app->popup));

  FURI_LOG_D(TAG, "view_dispatcher_init adding view submenu");
  view_dispatcher_add_view(app->view_dispatcher, AppView_ViewTasks,
                           submenu_get_view(app->submenu));
}
