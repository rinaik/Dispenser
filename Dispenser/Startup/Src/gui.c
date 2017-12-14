#include "gui.h"

#include "stm32f429i_discovery_ts.h"  // touch-screen header
#include "stm32f429i_discovery_lcd.h" // lcd header

#include <stdlib.h>

//Notes: Screen Size: 240x320

// do flash routine here for now

#include "stm32f4xx_hal_flash.h"
#define ADDR_FLASH_SECTOR_11_A  ((uint32_t)0x080E0000)
#define ADDR_FLASH_SECTOR_11_B  ((uint32_t)0x080E0004)

#define location_a ADDR_FLASH_SECTOR_11_A
#define location_b ADDR_FLASH_SECTOR_11_B

void save_data_to_flash_a(int data) {
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR );
    FLASH_Erase_Sector(FLASH_SECTOR_11, VOLTAGE_RANGE_3);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_11_A,data);
	HAL_FLASH_Lock();
}

void save_data_to_flash_b(int data) {
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR );
    FLASH_Erase_Sector(FLASH_SECTOR_11, VOLTAGE_RANGE_3);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_11_B,data);
	HAL_FLASH_Lock();
}

void LCDInit()
{
	  BSP_LCD_Init();
	  BSP_TS_Init(240,360);

	  BSP_LCD_LayerDefaultInit(1,SDRAM_DEVICE_ADDR);
	  BSP_LCD_SelectLayer(1);

	  BSP_LCD_DisplayOn();
}

void window_1_callback (UG_MESSAGE* msg)
{
	 if (motor_state == 0) {
		gui_state = gui_state_a;
	 }
	 else {
	    gui_state = gui_state_a;
	}
    if (msg->type == MSG_TYPE_OBJECT )
    {
    	if (msg->id == OBJ_TYPE_BUTTON )
    	{
    		if (msg->sub_id == BTN_ID_1)
    		{
    		   if (msg->event == OBJ_EVENT_PRESSED) {
    			 gui_state = START;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_1, C_GREEN ) ;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_0, C_BLUE ) ;
    		     UG_ButtonSetText(&window_1, BTN_ID_1, "PRESS!");
    		   }
    		   else {
    			 UG_ButtonSetForeColor(&window_1, BTN_ID_1, C_BLACK );
    			 UG_ButtonSetText(&window_1, BTN_ID_1, "START");
    		   }
    		}
    		if (msg->sub_id == BTN_ID_0)
    		{
    		  if (msg->event == OBJ_EVENT_PRESSED) {
    			 gui_state = STOP;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_1, C_BLUE ) ;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_2, C_BLUE ) ;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_3, C_BLUE ) ;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_0, C_RED ) ;
    			 UG_ButtonSetText(&window_1, BTN_ID_0, "PRESS!");

    			 save_data_to_flash_a(motor_speed_a);
    			 save_data_to_flash_b(motor_speed_b);
    	      }
    	      else {
    			 UG_ButtonSetForeColor(&window_1, BTN_ID_0, C_BLACK );
    			 UG_ButtonSetText(&window_1, BTN_ID_0, "STOP");
    		  }
    		}
    		if (msg->sub_id == BTN_ID_2)
    		{
    		  if (msg->event == OBJ_EVENT_PRESSED) {
    			 if (gui_state != STOP) {gui_state = FWD;}
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_2, C_YELLOW ) ;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_3, C_BLUE ) ;
    			 UG_ButtonSetText(&window_1, BTN_ID_2, "PRESS!");
    		  }
    	      else {
    	    	 if (gui_state != FWD ) { UG_ButtonSetBackColor (&window_1 ,BTN_ID_2, C_BLUE);}
                 UG_ButtonSetBackColor (&window_1 , BTN_ID_3, C_BLUE ) ;
    		     UG_ButtonSetText(&window_1, BTN_ID_2, "FWD");
    		  }
    		}
    		if (msg->sub_id == BTN_ID_3)
    		{
    		  if (msg->event == OBJ_EVENT_PRESSED) {
    		     if (gui_state != STOP) {gui_state = REV;}
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_2, C_BLUE ) ;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_3, C_YELLOW ) ;
    			 UG_ButtonSetText(&window_1, BTN_ID_3, "PRESS!");
    		  }
    		  else {
    			 if (gui_state != REV ) { UG_ButtonSetBackColor (&window_1 ,BTN_ID_3, C_BLUE);}
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_2, C_BLUE ) ;
    			 UG_ButtonSetText(&window_1, BTN_ID_3, "REV");
    		  }
    		}
    		if (msg->sub_id == BTN_ID_4)
    		{
    		  if (msg->event == OBJ_EVENT_PRESSED) {
    		     gui_state = PLUS;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_4, C_GREEN ) ;
    		     UG_ButtonSetText(&window_1, BTN_ID_4, "PRESS!");
    		  }
    		  else {
    			 gui_state = NO_TOUCH;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_4, C_BLUE ) ;
    		     UG_ButtonSetText(&window_1, BTN_ID_4, "+");
    		  }
    		}
    		if (msg->sub_id == BTN_ID_5)
    		{
    		  if (msg->event == OBJ_EVENT_PRESSED) {
    		     gui_state = MINUS;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_5, C_GREEN ) ;
                 UG_ButtonSetText(&window_1, BTN_ID_5, "PRESS!");
    		  }
    		  else {
    			 gui_state = NO_TOUCH;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_5, C_BLUE ) ;
    		     UG_ButtonSetText(&window_1, BTN_ID_5, "-");
    		  }
    	    }
    		if (msg->sub_id == BTN_ID_6)
    		{
    		    if (msg->event == OBJ_EVENT_PRESSED) {
    		    	UG_ButtonSetBackColor (&window_1 , BTN_ID_6, C_GREEN ) ;
    		    	UG_ButtonSetText(&window_1, BTN_ID_6, "PRESS!");
    		    	if (motor_state == 0) {
    		    			motor_state = 1;
    		    	}
    		    	else {
    		    			motor_state = 0;
    		    	}
    		    }
    		    else {
    		    	if (motor_state == 0) {

        		    	UG_ButtonSetBackColor (&window_1 , BTN_ID_6, C_YELLOW ) ;
    		    		UG_ButtonSetText(&window_1, BTN_ID_6, "MTR A");
    		    	}
    		    	if (motor_state == 1) {

    	    			UG_ButtonSetBackColor (&window_1 , BTN_ID_6, C_WHITE ) ;
    		    		UG_ButtonSetText(&window_1, BTN_ID_6, "MTR B");
    		    	}
    		    	 if (motor_state == 0) {itoa(motor_speed_a,buffer,10);}
    		    	 if (motor_state == 1) {itoa(motor_speed_b,buffer,10);}
    		    	 UG_TextboxSetText ( &window_1 , TXB_ID_1, buffer);




    		   }
    		}
    		if (motor_state == 0) {
    		    	gui_state_a = gui_state;
    		}
    		else {
    		    	gui_state_b = gui_state;
    		}

    	}
    }

}

void GUIInit()
{
	 // Create a window

	          UG_WindowCreate(&window_1, obj_buff_wnd_1, MAX_OBJECTS, window_1_callback);

	          // Modify the window title

	          UG_WindowSetTitleText(&window_1,"Motor Control");
	          UG_WindowSetTitleTextFont(&window_1, &FONT_12X20);

	          // Change window fore and back color
	          UG_WindowSetForeColor(&window_1, C_WHITE);
	          UG_WindowSetBackColor(&window_1, C_BLUE);

	          // Create some buttons
	          UG_ButtonCreate(&window_1, &button_1, BTN_ID_0, 10, 10, 110, 60);
	          UG_ButtonCreate(&window_1, &button_2, BTN_ID_1, 10, 80, 110, 130);
	          UG_ButtonCreate(&window_1, &button_3, BTN_ID_2, 10, 150, 110, 200);
	          UG_ButtonCreate(&window_1, &button_4, BTN_ID_3, 10, 220, 110, 270);
	          UG_ButtonCreate(&window_1, &button_5, BTN_ID_4, 130, 150, 220, 200);
	          UG_ButtonCreate(&window_1, &button_6, BTN_ID_5, 130, 220, 220, 270);
	          UG_ButtonCreate(&window_1, &button_7, BTN_ID_6, 130, 80, 220, 130);

	          // Label the buttons
	          UG_ButtonSetForeColor(&window_1, BTN_ID_0, C_BLACK);
	          UG_ButtonSetBackColor(&window_1, BTN_ID_0, C_RED);
	          UG_ButtonSetForeColor(&window_1, BTN_ID_1, C_GREEN);
	          UG_ButtonSetForeColor(&window_1, BTN_ID_2, C_BLACK);
	          UG_ButtonSetForeColor(&window_1, BTN_ID_3, C_BLACK);
	          UG_ButtonSetForeColor(&window_1, BTN_ID_4, C_BLACK);
	          UG_ButtonSetForeColor(&window_1, BTN_ID_5, C_BLACK);
	          UG_ButtonSetForeColor(&window_1, BTN_ID_6, C_BLACK);

	          UG_ButtonSetFont ( &window_1, BTN_ID_0, &FONT_12X20);
	          UG_ButtonSetText ( &window_1, BTN_ID_0, "STOP");
	          UG_ButtonSetFont ( &window_1, BTN_ID_1, &FONT_12X20);
	          UG_ButtonSetText ( &window_1, BTN_ID_1, "START");
	          UG_ButtonSetFont ( &window_1, BTN_ID_2, &FONT_12X20);
	          UG_ButtonSetText ( &window_1, BTN_ID_2, "FWD");
	          UG_ButtonSetFont ( &window_1, BTN_ID_3, &FONT_12X20);
	          UG_ButtonSetText ( &window_1, BTN_ID_3, "REV");

	          UG_ButtonSetFont ( &window_1, BTN_ID_4, &FONT_12X20);
	          UG_ButtonSetText ( &window_1, BTN_ID_4, "+");

	          UG_ButtonSetFont ( &window_1, BTN_ID_5, &FONT_12X20);
	          UG_ButtonSetText ( &window_1, BTN_ID_5, "-");

	          UG_ButtonSetFont ( &window_1, BTN_ID_6, &FONT_12X20);
  			  UG_ButtonSetBackColor (&window_1 , BTN_ID_6, C_YELLOW );
	          UG_ButtonSetText ( &window_1, BTN_ID_6, "MTR A");

	          //  Create Textboxs

	          UG_TextboxCreate( &window_1 , &textbox_1 , TXB_ID_0 , 130 , 10 , 220 , 40 );
	          UG_TextboxSetFont ( &window_1 , TXB_ID_0 , &FONT_12X20 ) ;
	          UG_TextboxSetText ( &window_1 , TXB_ID_0, "SPEED:" );
	          UG_TextboxSetAlignment ( &window_1 , TXB_ID_0 , ALIGN_TOP_CENTER );


	          UG_TextboxCreate( &window_1 , &textbox_2 , TXB_ID_1 , 130 , 40 , 220 , 70 );
	          UG_TextboxSetFont ( &window_1 , TXB_ID_1 , &FONT_12X20 );

	          if (motor_state == 0) {
	        	  itoa(motor_speed_a,buffer,10);
	          }
	          if (motor_state == 1) {
	          	  itoa(motor_speed_b,buffer,10);
	          }

	          UG_TextboxSetText ( &window_1 , TXB_ID_1, buffer);
	          UG_TextboxSetAlignment ( &window_1 , TXB_ID_1 , ALIGN_TOP_CENTER );

              UG_WindowShow(&window_1);
}


void UserPixelSet(UG_S16 x, UG_S16 y, UG_COLOR c) {BSP_LCD_DrawPixel(x, y, c);}

void TouchPress(void) {
	TS_StateTypeDef TS_State;

	BSP_TS_GetState(&TS_State);
	if ( TS_State.TouchDetected ) {
		UG_TouchUpdate(TS_State.X, TS_State.Y, TOUCH_STATE_PRESSED);
		}
	else {
		UG_TouchUpdate( -1, -1, TOUCH_STATE_RELEASED);
	}
}





