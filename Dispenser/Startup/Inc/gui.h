/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_H
#define __GUI_H

//Notes: Screen Size: 240x320

enum { STOP, START, MINUS, PLUS, NO_TOUCH };
int touch_location;
int x;
int y;

void init_gui(void);
bool tsPress(void);
int find_location (int current_location);
void screen_setup (void);

#endif /* __GUI_H */
