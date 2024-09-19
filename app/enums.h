#include <stdlib.h>

#ifndef ENUMS_H
#define ENUMS_H

typedef enum { MainMenu, QuickStart, CreateTask, Count } AppScene;

/** ids for the 2 types of view used by the app */
typedef enum { AppView_Menu, AppView_Popup } AppView;

/** all custom events */
typedef enum { AppEvent_ShowQuickStart, AppEvent_ShowCreateTask } AppEvent;

/* main menu scene */

/** indices for menu items */
typedef enum {
  QuickStart_Menu,
  CreateTask_Menu,
  ViewTasks_Menu,
  ShowStats_Menu,
  Exit_Menu
} AppMenuSelection;

// Define the Task structure
typedef struct {
  char name[50];
  char description[100];
  float price_per_hour;
  char start_time[20];
  char end_time[20];
  unsigned int total_time_minutes;
} Task;

typedef struct {
  Task *array;
  size_t size;
  size_t capacity;
} Tasks;

#endif // ENUMS_H