#include "stdbool.h" //boolean operators
#include "gui.h"

#include "stm32f429i_discovery_ts.h"  // touch-screen header
#include "stm32f429i_discovery_lcd.h" // lcd header

//Notes: Screen Size: 240x320

bool tsPress(void) {
	TS_StateTypeDef TS_State;
	static bool flgTouch=true;

	BSP_TS_GetState(&TS_State);
	if ( TS_State.TouchDetected ) {
		x = TS_State.X;
		y = TS_State.Y;
		printf("* x=%d y=%d\n", x, y);
		flgTouch = true;	// Set flag on new touch.
	}
	else {
		printf("no touch\n");
		flgTouch = false;
	}

	return flgTouch;	// Return true on touch.
}

void init_gui(void) {
	touch_location = STOP;
	x = 0;
	y = 0;
}

int find_location (int current_location) {
	int location;

	location = NO_TOUCH;
	if (tsPress()) {
		// START BOX
		if ((x > 40) && (x < 80) && (y > 260) && (y < 300)) {
		  location = START;
		}
		// MINUS BOX
		if ((x > 40) && (x < 80) && (y > 180) && (y < 220)) {
		 location = MINUS;
		}
		// PLUS BOX
	    if ((x > 160) && (x < 200) && (y > 180) && (y < 220)) {
		 location = PLUS;
		}
		// STOP BOX
		if ((x > 160) && (x < 200) && (y > 260) && (y < 300)) {
		 location = STOP;
		}

	}
	return location;
}

void screen_setup (void) {

		// DRAW -

	    BSP_LCD_SetTextColor( LCD_COLOR_BLUE);
	    BSP_LCD_DisplayStringAt( 0, LINE(9), (uint8_t *)"    -", LEFT_MODE );
	    BSP_LCD_DrawRect(40, 180, 40, 40);

		// DRAW +

	    BSP_LCD_SetTextColor( LCD_COLOR_BLUE );
	    BSP_LCD_DisplayStringAt( 0, LINE(9), (uint8_t *)"+    ", RIGHT_MODE );
	    BSP_LCD_DrawRect(160, 180, 40, 40);

	    // DRAW START

	    BSP_LCD_SetTextColor( LCD_COLOR_GREEN );
	   	BSP_LCD_DisplayStringAt( 0, LINE(14), (uint8_t *)"  START", LEFT_MODE );
	    BSP_LCD_DrawRect(40, 260, 40, 40);

	    // DRAW STOP

	    BSP_LCD_SetTextColor( LCD_COLOR_RED );
	   	BSP_LCD_DisplayStringAt( 0, LINE(14), (uint8_t *)"STOP  ", RIGHT_MODE );
	    BSP_LCD_DrawRect(160, 260, 40, 40);

}
