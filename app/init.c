/** initialise app data, scene manager, and view dispatcher */
#include "app.h"
#include "scene/scene.h"

#define TAG "tracker_app"

#include <trackerflipx_icons.h> // created later after ubft run

App *init() {
  FURI_LOG_T(TAG, "init");
  App *app = malloc(sizeof(App));
  scene_manager_init(app);
  view_dispatcher_init(app);
  return app;
}
