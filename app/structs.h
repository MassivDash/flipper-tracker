#include <datetime/datetime.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef STRUCTS_H
#define STRUCTS_H

typedef enum {
  MainMenu,
  QuickStart,
  CreateTask,
  ViewTasks,
  TaskActions,
  TaskContinue,
  ViewStats,
  Count
} AppScene;

typedef enum {
  AppView_Menu,
  AppView_Popup,
  AppView_ViewTasks,
  AppView_TaskActions,
  AppView_TaskContinue,
  AppView_ViewStats,
} AppView;

/** all custom events */
typedef enum {
  AppEvent_ShowQuickStart,
  AppEvent_ShowCreateTask,
  AppEvent_ViewTasks,
  AppEvent_TaskActions,
  AppEvent_TaskContinue,
} AppEvent;

/* main menu scene */

/** indices for menu items */
typedef enum {
  QuickStart_Menu,
  CreateTask_Menu,
  ViewTasks_Menu,
} AppMenuSelection;

/** Submenu for task view */
typedef enum {
  TaskAction_Continue,
  TaskAction_Edit,
  TaskAction_Stats,
  TaskAction_ToggleCompleted
} TaskActionMenu;

/** Statues of entries */
typedef enum {
  TaskStatus_Running,
  TaskStatus_Stopped,
} TaskStatus;

// Define the Task structure
typedef struct {
  char id[50];
  char name[50];
  char description[100];
  float price_per_hour;
  DateTime start_time;
  DateTime end_time;
  DateTime last_start_time;
  bool completed;
  unsigned int total_time_minutes;
  TaskStatus status;
} Task;

typedef struct {
  Task *array;
  size_t size;
  size_t capacity;
} Tasks;

#endif // STRUCTS_H