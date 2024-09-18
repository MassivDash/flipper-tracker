#ifndef QUICK_START_H
#define QUICK_START_H

#include "../../app.h"
#include <gui/modules/popup.h>

// Function declarations
void scene_on_enter_popup_one(void *context);
bool scene_on_event_popup_one(void *context, SceneManagerEvent event);
void scene_on_exit_popup_one(void *context);

void scene_on_enter_popup_two(void *context);
bool scene_on_event_popup_two(void *context, SceneManagerEvent event);
void scene_on_exit_popup_two(void *context);

#endif // QUICK_START_H