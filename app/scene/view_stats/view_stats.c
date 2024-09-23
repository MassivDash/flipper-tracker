#include "../../app.h"
#include <furi.h>
#include <furi_hal.h>
#include <gui/elements.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>

#define TAG "tracker_app"
#define LIST_ITEMS 4U
#define LIST_LINE_H 13U
#define HEADER_H 12U
#define MOVE_X_OFFSET 5U

typedef struct {
  const char *labels[LIST_ITEMS];
  int32_t list_offset;
  int32_t current_idx;
} ViewStatsModel;

Task *shared_task;

static void view_stats_draw_callback(Canvas *canvas, void *_model) {
  ViewStatsModel *model = _model;
  furi_assert(model != NULL, "Model is NULL");

  canvas_clear(canvas);
  canvas_set_color(canvas, ColorBlack);
  canvas_set_font(canvas, FontPrimary);
  elements_multiline_text_aligned(canvas, canvas_width(canvas) / 2, 0,
                                  AlignCenter, AlignTop, "Task Details");

  if (!shared_task) {
    canvas_draw_str(canvas, 4, 9, "No current task");
    furi_record_close("app");
    return;
  }

  const char *fields[] = {"ID",         "Name",  "Description", "Price per h",
                          "Start",      "End",   "Last Start",  "Completed",
                          "Total Time", "Status"};

  char price_per_hour_str[16];
  snprintf(price_per_hour_str, sizeof(price_per_hour_str), "%.2f",
           (double)shared_task->price_per_hour);

  char total_time_minutes_str[16];
  snprintf(total_time_minutes_str, sizeof(total_time_minutes_str), "%u",
           shared_task->total_time_minutes);

  const char *values[] = {
      shared_task->id,
      shared_task->name,
      shared_task->description,
      price_per_hour_str,
      shared_task->start_time,
      shared_task->end_time,
      shared_task->last_start_time,
      shared_task->completed ? "Yes" : "No",
      total_time_minutes_str,
      shared_task->status == TaskStatus_Running ? "Running" : "Stopped"};

  const size_t btn_number = sizeof(fields) / sizeof(fields[0]);
  const bool show_scrollbar = btn_number > LIST_ITEMS;

  for (uint32_t i = 0; i < MIN(btn_number, LIST_ITEMS); i++) {
    int32_t idx = CLAMP((uint32_t)(i + model->list_offset), btn_number, 0U);
    uint8_t x_offset = (model->current_idx == idx) ? MOVE_X_OFFSET : 0;
    uint8_t y_offset = HEADER_H + i * LIST_LINE_H;
    uint8_t box_end_x = canvas_width(canvas) - (show_scrollbar ? 6 : 1);

    canvas_set_color(canvas, ColorBlack);
    if (model->current_idx == idx) {
      canvas_draw_box(canvas, x_offset, y_offset, box_end_x - x_offset,
                      LIST_LINE_H);
      canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_str_aligned(canvas, x_offset + 3, y_offset + 3, AlignLeft,
                            AlignTop, fields[idx]);
    canvas_draw_str_aligned(canvas, x_offset + 64, y_offset + 3, AlignLeft,
                            AlignTop, values[idx]);
  }

  if (show_scrollbar) {
    elements_scrollbar_pos(canvas, canvas_width(canvas) - 1, HEADER_H,
                           canvas_height(canvas) - HEADER_H, model->list_offset,
                           btn_number);
  }
}

static void update_list_offset(ViewStatsModel *model) {
  const size_t btn_number = 10; // Total number of items
  const int32_t bounds = btn_number > (LIST_ITEMS - 1) ? 2 : btn_number;

  if ((btn_number > (LIST_ITEMS - 1)) &&
      (model->current_idx >= ((int32_t)btn_number - 1))) {
    model->list_offset = model->current_idx - (LIST_ITEMS - 1);
  } else if (model->list_offset < model->current_idx - bounds) {
    model->list_offset = CLAMP(model->current_idx - (int32_t)(LIST_ITEMS - 2),
                               (int32_t)btn_number - bounds, 0);
  } else if (model->list_offset > model->current_idx - bounds) {
    model->list_offset =
        CLAMP(model->current_idx - 1, (int32_t)btn_number - bounds, 0);
  }
}

static bool view_stats_input_callback(InputEvent *event, void *context) {
  App *app = context;
  View *view = app->view;

  FURI_LOG_I(TAG, "view_stats_input_callback: %d", event->key);

  if ((event->type == InputTypeShort || event->type == InputTypeRepeat) &&
      (event->key == InputKeyUp || event->key == InputKeyDown)) {
    with_view_model(
        view, ViewStatsModel * model,
        {
          const size_t btn_number = 10; // Total number of items
          if (event->key == InputKeyUp) {
            if (model->current_idx <= 0) {
              model->current_idx = btn_number - 1;
            } else {
              model->current_idx--;
            }
          } else if (event->key == InputKeyDown) {
            if (model->current_idx >= (int32_t)(btn_number - 1)) {
              model->current_idx = 0;
            } else {
              model->current_idx++;
            }
          }
          update_list_offset(model);
        },
        true);
    return true;
  } else if (event->key == InputKeyBack) {
    scene_manager_search_and_switch_to_previous_scene(app->scene_manager,
                                                      TaskActions);
    return false; // Pass the back event through
  }

  return false;
}

uint32_t view_navigation_callback() {
  return AppView_TaskActions; // Return the view id that is registered in the
                              // ViewDispatcher.
}

void scene_on_enter_view_stats(void *context) {
  FURI_LOG_D(TAG, "scene_on_enter_view_stats");
  App *app = context;
  view_allocate_model(app->view, ViewModelTypeLocking, sizeof(ViewStatsModel));
  view_set_context(app->view, app);
  shared_task = app->current_task;
  view_set_draw_callback(app->view, view_stats_draw_callback);
  view_set_input_callback(app->view, view_stats_input_callback);
  view_dispatcher_switch_to_view(app->view_dispatcher, AppView_ViewStats);
}

void scene_on_event_view_stats(void *context, SceneManagerEvent event) {
  FURI_LOG_D(TAG, "scene_on_event_view_stats");
  App *app = context;

  if (event.type == SceneManagerEventTypeCustom) {
    switch (event.event) {
    case DialogExResultCenter:
      // Handle center button press if needed
      break;
    case DialogExResultLeft:
      scene_manager_handle_back_event(app->scene_manager);
      break;
    default:
      break;
    }
  }
}

void scene_on_exit_view_stats(void *context) {
  FURI_LOG_D(TAG, "scene_on_exit_view_stats");
  App *app = context;
  view_free_model(app->view);
}