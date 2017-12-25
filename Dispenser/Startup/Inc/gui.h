/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_H
#define __GUI_H

#include "ugui.h"
char buffer[5];

#include "stm32f4xx_hal_flash.h"

// flash locations for motor speed

#define ADDR_FLASH_SECTOR_11_A  ((uint32_t)0x080E0000)
#define ADDR_FLASH_SECTOR_11_B  ((uint32_t)0x080E0004)

#define location_a ADDR_FLASH_SECTOR_11_A
#define location_b ADDR_FLASH_SECTOR_11_B

// state machine for gui

enum { STOP, START, FWD, REV, PLUS, MINUS};

// gui global variables

int gui_state, gui_state_a, gui_state_b;
int motor_state;
int motor_speed_a, motor_speed_b;
int direction_a, direction_b;

#define MAX_OBJECTS 20

UG_GUI gui;
UG_WINDOW window_1;
UG_OBJECT obj_buff_wnd_1[MAX_OBJECTS];
UG_BUTTON button_1, button_2, button_3, button_4, button_5, button_6, button_7;
UG_TEXTBOX textbox_1, textbox_2, textbox_3, textbox_4;

//Notes: Screen Size: 240x320

// functions for GUI

void save_data_to_flash(int data_a, int data_b);

void LCDInit(void);

void GUIInit(void);

void TouchPress(void);

void UserPixelSet(UG_S16 x, UG_S16 y, UG_COLOR c);

#endif /* __GUI_H */
