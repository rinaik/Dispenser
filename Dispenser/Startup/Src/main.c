/**
  ******************************************************************************
  * File Name          : main.c by RIN
  * Description        : Main program body
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "cmsis_os.h"

#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h" // lcd header
#include "stm32f429i_discovery_ts.h"  // touch-screen header
#include "stm32f429i_discovery_io.h"  // i-o header

#include "params.h" // motor parameters
#include "L6470.h" // motor control

#include "gui.h" //  gui functions

/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim1;

osThreadId defaultTaskHandle;

SPI_HandleTypeDef hspi5;

int motor_speed = 1000;
int direction = 0;

/* Private function prototypes ---------------------------------------------------------*/

static void vLedBlinkGreen(void *pvParameters);
static void vLedBlinkRed(void *pvParameters);
static void vGUIRun();
static void vMotorRun();

static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI5_Init(void);

static void StartDefaultTask(void const * argument);

// Private functions

static void vLedBlinkGreen(void *pvParameters)
{
    for(;;)
    {
        HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_13);

        vTaskDelay( 500 / portTICK_RATE_MS );
    }
};

static void vLedBlinkRed(void *pvParameters)
{
    for(;;)
    {
        HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_14);

        vTaskDelay( 1000 / portTICK_RATE_MS );
    }
};


static void vGUIRun() {
	for(;;)
	    {
		  TouchPress();
		  UG_Update();

		  vTaskDelay( 100 / portTICK_RATE_MS );
	    }
};

void MotorSPI() {
	          GPIO_InitTypeDef GPIO_InitStruct;

			  /* GPIO Ports Clock Enable */
			  __HAL_RCC_GPIOF_CLK_ENABLE();

			  /* Enable SPI CLK */
			    __HAL_RCC_SPI5_CLK_ENABLE();

			 /*Configure GPIO pins :SPI5 */

			  GPIO_InitStruct.Pin = GPIO_PIN_6;               // Chip select
			  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			  GPIO_InitStruct.Pull = GPIO_NOPULL;
			  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
			  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

			  GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 ; // SCLK MISO MOSI
			  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
			  GPIO_InitStruct.Pull = GPIO_NOPULL;
			  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
			  GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
			  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

static void vMotorRun() {
	for(;;)
	{
		  MotorSPI();
		  MX_SPI5_Init();

		  L6470_GetStatus(0);

		  if (gui_state == START)
		  {
			  motor_speed = 1000;
			  L6470_Run(0,direction,motor_speed);
		  }
		  if (gui_state == STOP)
		  {
			  motor_speed = 0;
			  L6470_SoftStop(0);
		  }
		  if (gui_state == PLUS)
		  {
			  motor_speed = motor_speed + 1000;
              if (motor_speed > 10000) {motor_speed = 10000;}
		      L6470_Run(0,direction,motor_speed);
		  }
		  if (gui_state == MINUS)
		  {
			  motor_speed = motor_speed - 1000;
			  if (motor_speed < 0) {motor_speed = 0;}
		      L6470_Run(0,direction,motor_speed);
		  }
		  if (gui_state == REV) {
			  direction = 1;
			  L6470_Run(0,direction,motor_speed);
		  }
		  if (gui_state == FWD) {
			  direction = 0;
		      L6470_Run(0,direction,motor_speed);
		  }

          vTaskDelay( 100 / portTICK_RATE_MS );
	}
}


int main(void)
{

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured IO and SPI */
  MX_GPIO_Init();
  MX_SPI5_Init();

  /* Initialize LCD and Touch-screen */

  LCDInit();

  /* Initialize GUI with 240x320 screen size*/

  UG_Init(&gui, UserPixelSet, 240, 320);
  GUIInit();

  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  xTaskCreate( vLedBlinkGreen, (const char*)"Led Blink Green",
      128, NULL, tskIDLE_PRIORITY, NULL );

  xTaskCreate( vLedBlinkRed, (const char*)"Led Blink Red",
        128, NULL, tskIDLE_PRIORITY, NULL );

  xTaskCreate( vGUIRun, (const char*)"GUI Run",
 	 128, NULL, tskIDLE_PRIORITY, NULL );

  xTaskCreate( vMotorRun, (const char*)"Motor Run",
 	 128, NULL, tskIDLE_PRIORITY, NULL );


  vTaskStartScheduler();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
static void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 260;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}


/* SPI6 init function */
static void MX_SPI5_Init(void)
{

  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi5.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
    printf("SPI HAL INIT ERROR !\n");
  }
}


static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* Enable SPI CLK */
    __HAL_RCC_SPI5_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, NCS_MEMS_SPI_Pin|CSX_Pin|OTG_FS_PSO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ACP_RST_GPIO_Port, ACP_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, RDX_Pin|WRX_DCX_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, LD3_Pin|LD4_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pins : NCS_MEMS_SPI_Pin CSX_Pin OTG_FS_PSO_Pin */
  GPIO_InitStruct.Pin = NCS_MEMS_SPI_Pin|CSX_Pin|OTG_FS_PSO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : B1_Pin MEMS_INT1_Pin MEMS_INT2_Pin TP_INT1_Pin */
  GPIO_InitStruct.Pin = B1_Pin|MEMS_INT1_Pin|MEMS_INT2_Pin|TP_INT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ACP_RST_Pin */
  GPIO_InitStruct.Pin = ACP_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ACP_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OC_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TE_Pin */
  GPIO_InitStruct.Pin = TE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RDX_Pin WRX_DCX_Pin */
  GPIO_InitStruct.Pin = RDX_Pin|WRX_DCX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin LD4_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
}

// Period elapsed callback in non blocking mode

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM1) {
    HAL_IncTick();
   }
}

void _Error_Handler(char * file, int line)
{
    printf("ERROR AT LINE %d\n",line);
}

void assert_failed(uint8_t* file, uint32_t line)
{
	printf ("Assert Error at line %lu\n",line);
}


