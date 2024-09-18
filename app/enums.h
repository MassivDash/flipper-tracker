#ifndef ENUMS_H
#define ENUMS_H

typedef enum { MainMenu, FirstPopup, SecondPopup, Count } AppScene;

/** ids for the 2 types of view used by the app */
typedef enum { AppView_Menu, AppView_Popup } AppView;

/** all custom events */
typedef enum { AppEvent_ShowPopupOne, AppEvent_ShowPopupTwo } AppEvent;

/* main menu scene */

/** indices for menu items */
typedef enum { AppMenuSelection_One, AppMenuSelection_Two } AppMenuSelection;

#endif // ENUMS_H