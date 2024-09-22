
#include "../../app.h"
#include "../../csv/csv.h"
#include "../../tasks/task.h"
#include <furi.h>
#include <furi_hal.h>
#include <gui/modules/dialog_ex.h>

#define TAG "tracker_app"

static void task_continue_scene_dialog_callback(DialogExResult result,
                                                void *context) {
  App *app = context;

  view_dispatcher_send_custom_event(app->view_dispatcher, result);
}

void scene_on_enter_task_continue(void *context) {
  FURI_LOG_T(TAG, "scene_on_enter_task_actions");
  App *app = context;
  DialogEx *dialog_ex = app->dialog;

  char buffer_title[64];
  char buffer_text[256];

  bool is_task_running = app->current_task->status == TaskStatus_Running;
  if (is_task_running) {

    snprintf(buffer_title, sizeof(buffer_title), "Stop task: %s",
             app->current_task->name);

    snprintf(buffer_text, sizeof(buffer_text),
             "Your task has been running for %d minutes, press center to stop "
             "tracking. Tracking started at %s",
             app->current_task->total_time_minutes,
             app->current_task->last_start_time);

    dialog_ex_set_center_button_text(dialog_ex, "Stop");
  } else {
    snprintf(buffer_title, sizeof(buffer_title), "Start task: %s",
             app->current_task->name);

    snprintf(buffer_text, sizeof(buffer_text),
             "Your task has been running for %d minutes, press center to stop "
             "tracking. Tracking was last stopped at %s",
             app->current_task->total_time_minutes,
             app->current_task->end_time);
    dialog_ex_set_center_button_text(dialog_ex, "Start");
  }

  dialog_ex_set_header(dialog_ex, buffer_title, 64, 0, AlignCenter, AlignTop);
  dialog_ex_set_text(dialog_ex, buffer_text, 64, 29, AlignCenter, AlignCenter);

  dialog_ex_set_left_button_text(dialog_ex, "Exit");

  dialog_ex_set_result_callback(dialog_ex, task_continue_scene_dialog_callback);

  dialog_ex_set_context(dialog_ex, app);

  view_dispatcher_switch_to_view(app->view_dispatcher, AppView_TaskContinue);
}

bool scene_on_event_task_continue(void *context, SceneManagerEvent event) {
  App *app = context;
  DialogEx *dialog_ex = app->dialog;
  bool consumed = false;

  if (event.type == SceneManagerEventTypeCustom) {
    switch (event.event) {
    case DialogExResultCenter:
      if (app->current_task->status == TaskStatus_Running) {
        app->current_task->status = TaskStatus_Stopped;
        dialog_ex_set_center_button_text(dialog_ex, "Start");
      } else {
        app->current_task->status = TaskStatus_Running;
        dialog_ex_set_center_button_text(dialog_ex, "Start");
      }

      tasks_update(app, app->current_task);
      find_and_replace_task_in_csv(app, app->current_task);
      scene_manager_next_scene(app->scene_manager, TaskContinue);
      consumed = true;
      break;
    case DialogExResultLeft:
      scene_manager_next_scene(app->scene_manager, TaskActions);
      consumed = true;
      break;
    case DialogExResultRight:
      scene_manager_next_scene(app->scene_manager, TaskActions);
      consumed = true;
      break;
    default:
      break;
    }
  }

  return consumed;
}

void scene_on_exit_task_continue(void *context) {
  FURI_LOG_T(TAG, "scene_on_exit_task_continue");
  App *app = context;
  dialog_ex_reset(app->dialog);
}