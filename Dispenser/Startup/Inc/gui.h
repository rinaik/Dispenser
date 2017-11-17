/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_H
#define __GUI_H

#include "ugui.h"
char buffer[5];

enum {NO_TOUCH, STOP, START, FWD, REV, PLUS, MINUS};

int gui_state;
int motor_speed;
int direction;

#define MAX_OBJECTS 10

UG_GUI gui;
UG_WINDOW window_1;
UG_OBJECT obj_buff_wnd_1[MAX_OBJECTS];
UG_BUTTON button_1, button_2, button_3, button_4, button_5, button_6;
UG_TEXTBOX textbox_1, textbox_2, textbox_3, textbox_4;

//Notes: Screen Size: 240x320

void save_data_to_flash(int data);

void LCDInit(void);

void GUIInit(void);

void TouchPress(void);

void UserPixelSet(UG_S16 x, UG_S16 y, UG_COLOR c);

#endif /* __GUI_H */
