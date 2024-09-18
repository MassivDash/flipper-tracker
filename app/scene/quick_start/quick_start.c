#include "../../app.h"
#include "../../enums.h"
#include <gui/modules/popup.h>

#define TAG "tracker_app"

#include <trackerflipx_icons.h> // created later after ubft run

void scene_on_enter_popup_one(void *context) {
  FURI_LOG_T(TAG, "scene_on_enter_popup_one");
  App *app = context;
  popup_reset(app->popup);
  popup_set_context(app->popup, app);
  popup_set_header(app->popup, "Popup One", 64, 10, AlignCenter, AlignTop);
  popup_set_icon(app->popup, 10, 10, &I_cvc_36x36);
  popup_set_text(app->popup, "One! One popup. Ah ah ah...", 64, 20, AlignLeft,
                 AlignTop);
  view_dispatcher_switch_to_view(app->view_dispatcher, AppView_Popup);
}

bool scene_on_event_popup_one(void *context, SceneManagerEvent event) {
  FURI_LOG_T(TAG, "scene_on_event_popup_one");
  UNUSED(context);
  UNUSED(event);
  return false; // don't handle any events
}

void scene_on_exit_popup_one(void *context) {
  FURI_LOG_T(TAG, "scene_on_exit_popup_one");
  App *app = context;
  popup_reset(app->popup);
}

/* popup 2 scene */

void scene_on_enter_popup_two(void *context) {
  FURI_LOG_T(TAG, "scene_on_enter_popup_two");
  App *app = context;
  popup_reset(app->popup);
  popup_set_context(app->popup, app);
  popup_set_header(app->popup, "Popup Two", 64, 10, AlignCenter, AlignTop);
  popup_set_icon(app->popup, 10, 10, &I_cvc_36x36);
  popup_set_text(app->popup, "Two! Two popups. (press back)", 64, 20, AlignLeft,
                 AlignTop);
  view_dispatcher_switch_to_view(app->view_dispatcher, AppView_Popup);
}

bool scene_on_event_popup_two(void *context, SceneManagerEvent event) {
  FURI_LOG_T(TAG, "scene_on_event_popup_two");
  UNUSED(context);
  UNUSED(event);
  return false; // don't handle any events
}

void scene_on_exit_popup_two(void *context) {
  FURI_LOG_T(TAG, "scene_on_exit_popup_two");
  App *app = context;
  popup_reset(app->popup);
}
