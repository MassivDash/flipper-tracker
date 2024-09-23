#include "../../app.h"
#include "../../csv/csv.h"
#include "../../datetime/datetime.h"
#include "../../tasks/task.h"
#include <furi.h>
#include <furi_hal.h>
#include <gui/modules/dialog_ex.h>
#include <gui/view_dispatcher.h>
#include <locale/locale.h>

#define TAG "tracker_app"

static void task_continue_scene_dialog_callback(DialogExResult result,
                                                void *context) {
  App *app = context;
  view_dispatcher_send_custom_event(app->view_dispatcher, result);
}

static void task_continue_update(App *app) {
  DialogEx *dialog_ex = app->dialog;

  char buffer_title[64];
  char buffer_text[256];

  bool is_task_running = app->current_task->status == TaskStatus_Running;
  if (is_task_running) {
    snprintf(buffer_title, sizeof(buffer_title), "Stop task: %s",
             app->current_task->name);

    snprintf(buffer_text, sizeof(buffer_text),
             "Your task has been running for %d minutes, press center to stop "
             "tracking.",
             app->current_task->total_time_minutes);

    dialog_ex_set_center_button_text(dialog_ex, "Stop");
  } else {
    snprintf(buffer_title, sizeof(buffer_title), "Start task: %s",
             app->current_task->name);

    snprintf(buffer_text, sizeof(buffer_text),
             "Your task has been running for %d minutes, press center to start "
             "tracking.",
             app->current_task->total_time_minutes);

    dialog_ex_set_center_button_text(dialog_ex, "Start");
  }

  dialog_ex_set_header(dialog_ex, buffer_title, 64, 0, AlignCenter, AlignTop);
  dialog_ex_set_text(dialog_ex, buffer_text, 64, 29, AlignCenter, AlignCenter);
  dialog_ex_set_left_button_text(dialog_ex, "Exit");
  dialog_ex_set_result_callback(dialog_ex, task_continue_scene_dialog_callback);
  dialog_ex_set_context(dialog_ex, app);
}

void scene_on_enter_task_continue(void *context) {
  FURI_LOG_T(TAG, "scene_on_enter_task_actions");
  App *app = context;

  task_continue_update(app);

  view_dispatcher_switch_to_view(app->view_dispatcher, AppView_TaskContinue);
}

bool scene_on_event_task_continue(void *context, SceneManagerEvent event) {
  App *app = context;
  DialogEx *dialog_ex = app->dialog;
  bool consumed = false;

  if (event.type == SceneManagerEventTypeCustom) {

    switch (event.event) {
    case DialogExResultCenter:

      // Task is running -> stop it
      if (app->current_task->status == TaskStatus_Running) {
        // Change status to stopped
        app->current_task->status = TaskStatus_Stopped;

        // Get current date and time, update the end time
        DateTime datetime_end_time = {0};
        furi_hal_rtc_get_datetime(&datetime_end_time);
        FuriString *tmp_string = furi_string_alloc();
        locale_format_time(tmp_string, &datetime_end_time,
                           locale_get_time_format(), false);
        strncpy(app->current_task->end_time, furi_string_get_cstr(tmp_string),
                sizeof(app->current_task->end_time) - 1);
        furi_string_free(tmp_string);
        app->current_task->end_time[sizeof(app->current_task->end_time) - 1] =
            '\0'; // Ensure null-termination

        // Calculate the time difference
        DateTime datetime_start_time = {0};
        sscanf(app->current_task->last_start_time,
               "%04hu-%02hhu-%02hhu %02hhu:%02hhu", &datetime_start_time.year,
               &datetime_start_time.month, &datetime_start_time.day,
               &datetime_start_time.hour, &datetime_start_time.minute);

        int32_t time_difference_minutes = calculate_time_difference_in_minutes(
            &datetime_start_time, &datetime_end_time);
        app->current_task->total_time_minutes += time_difference_minutes;

        // Update UI
        dialog_ex_set_center_button_text(dialog_ex, "Start");

      } else {
        // Task is stopped -> start it
        app->current_task->status = TaskStatus_Running;

        // Get current date and time, update the last start time
        DateTime datetime_start_time = {0};
        furi_hal_rtc_get_datetime(&datetime_start_time);
        FuriString *tmp_string = furi_string_alloc();
        locale_format_time(tmp_string, &datetime_start_time,
                           locale_get_time_format(), false);
        strncpy(app->current_task->last_start_time,
                furi_string_get_cstr(tmp_string),
                sizeof(app->current_task->last_start_time) - 1);
        furi_string_free(tmp_string);
        app->current_task
            ->last_start_time[sizeof(app->current_task->last_start_time) - 1] =
            '\0'; // Ensure null-termination

        // Update UI
        dialog_ex_set_center_button_text(dialog_ex, "Stop");
      }

      tasks_update(app, app->current_task);
      find_and_replace_task_in_csv(app, app->current_task);
      // update the scene
      task_continue_update(app);

      consumed = true;
      break;
    case DialogExResultLeft:
      scene_manager_search_and_switch_to_previous_scene(app->scene_manager,
                                                        TaskActions);
      consumed = true;
      break;
    case DialogExResultRight:
      scene_manager_search_and_switch_to_previous_scene(app->scene_manager,
                                                        TaskActions);
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