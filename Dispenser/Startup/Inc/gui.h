/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_H
#define __GUI_H

#include "ugui.h"

//Notes: Screen Size: 240x320

enum { NO_TOUCH, STOP, START, REV, FWD, PLUS, MINUS };

bool tsPress(void);

void UserPixelSet(UG_S16 x, UG_S16 y, UG_COLOR c);

#endif /* __GUI_H */
