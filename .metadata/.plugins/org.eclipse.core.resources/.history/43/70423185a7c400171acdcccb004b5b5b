#include "gui.h"

#include "stm32f429i_discovery_ts.h"  // touch-screen header
#include "stm32f429i_discovery_lcd.h" // lcd header

//Notes: Screen Size: 240x320

void UserPixelSet(UG_S16 x, UG_S16 y, UG_COLOR c) {BSP_LCD_DrawPixel(x, y, c);}

void TouchPress(void) {
	TS_StateTypeDef TS_State;
	int x, y;

	BSP_TS_GetState(&TS_State);
	if ( TS_State.TouchDetected ) {
		x = TS_State.X;
		y = TS_State.Y;
		printf("* x=%d y=%d\n", x, y);
		UG_TouchUpdate(x, y, TOUCH_STATE_PRESSED);
		}
	else {
		printf("no touch\n");
		UG_TouchUpdate( -1, -1, TOUCH_STATE_RELEASED);
	}
}





