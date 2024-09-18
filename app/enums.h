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

#endif // ENUMS_H