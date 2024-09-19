#ifndef SHOW_STATS_H
#define SHOW_STATS_H

#include "../../app.h"
#include <furi.h>

// Function declarations
void submenu_callback_show_stats(void *context, uint32_t index);
void scene_on_enter_show_stats(void *context);
bool scene_on_event_show_stats(void *context, SceneManagerEvent event);
void scene_on_exit_show_stats(void *context);

#endif // SHOW_STATS_H