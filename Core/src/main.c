/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
void Gpio_Init(void);
void Init_Show(void);
void Init_Button(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  Gpio_Init();
  Init_Button();

  LL_mDelay(500);
  Init_Show();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SetSystemCoreClock(8000000);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

void Gpio_Init(void)
{

	//Port B

	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;

	GPIOB->MODER|=GPIO_MODER_MODER5_0|GPIO_MODER_MODER6_0|GPIO_MODER_MODER7_0;
	GPIOB->OTYPER&=~GPIO_OTYPER_OT_5|~GPIO_OTYPER_OT_6|~GPIO_OTYPER_OT_7;
	GPIOB->PUPDR|=GPIO_PUPDR_PUPDR5_1|GPIO_PUPDR_PUPDR6_1|GPIO_PUPDR_PUPDR7_1;

	//Port A

	RCC->AHBENR|=RCC_AHBENR_GPIOAEN;

	GPIOA->MODER|=GPIO_MODER_MODER15_0;
	GPIOA->OTYPER|=GPIO_OTYPER_OT_15;
	GPIOA->PUPDR|=GPIO_PUPDR_PUPDR15_0;

	//Work with p-channel transistor
	GPIOA->BSRR|=GPIO_BSRR_BS_15;

	}


void Init_Show(void)
{
	//Creeping column

	GPIOB->BSRR|=GPIO_BSRR_BS_5;
	LL_mDelay(500);
	GPIOB->BSRR|=GPIO_BSRR_BR_5;
	GPIOB->BSRR|=GPIO_BSRR_BS_6;
	LL_mDelay(500);
	GPIOB->BSRR|=GPIO_BSRR_BR_6;
	GPIOB->BSRR|=GPIO_BSRR_BS_7;
	LL_mDelay(500);
	GPIOB->BSRR|=GPIO_BSRR_BR_7;
	LL_mDelay(500);

	//Blinky

	for (uint8_t n = 0; n < 6; ++n)
		{
	GPIOB->ODR^=GPIO_ODR_5|GPIO_ODR_6|GPIO_ODR_7;
	LL_mDelay(350);
		}

	//Transistor part

	for (uint8_t n = 0; n < 10; ++n)
	{
		GPIOA->ODR ^= GPIO_ODR_15;
		LL_mDelay(50);
	}
}

void Init_Button(void)
{
	//INIT BUTTON 1//

	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;

	GPIOB->MODER&=~GPIO_MODER_MODER4;
	GPIOB->PUPDR&=~GPIO_PUPDR_PUPDR4;

	SYSCFG->EXTICR[1]|=SYSCFG_EXTICR2_EXTI4_PB;

	EXTI->EMR|=EXTI_EMR_EM4;
	EXTI->IMR|=EXTI_IMR_IM4;
	EXTI->FTSR|=EXTI_FTSR_FT4;

	NVIC_EnableIRQ(EXTI4_15_IRQn);

	//INIT BUTTON 2//

	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;

	GPIOB->MODER&=~GPIO_MODER_MODER3;
	GPIOB->PUPDR&=~GPIO_PUPDR_PUPDR3;

	SYSCFG->EXTICR[0]|=SYSCFG_EXTICR1_EXTI3_PB;

	EXTI->EMR|=EXTI_EMR_EM3;
	EXTI->IMR|=EXTI_IMR_IM3;
	EXTI->FTSR|=EXTI_FTSR_FT3;

	NVIC_EnableIRQ(EXTI2_3_IRQn);

	//INIT BUTTON 3//

	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;

	GPIOB->MODER&=~GPIO_MODER_MODER2;
	GPIOB->PUPDR&=~GPIO_PUPDR_PUPDR2;

	SYSCFG->EXTICR[0]|=SYSCFG_EXTICR1_EXTI2_PB;

	EXTI->EMR|=EXTI_EMR_EM2;
	EXTI->IMR|=EXTI_IMR_IM2;
	EXTI->FTSR|=EXTI_FTSR_FT2;

	NVIC_EnableIRQ(EXTI2_3_IRQn);

}

//Interrupt Handling Button 1

void EXTI4_15_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PR4)
	{
	GPIOB->ODR^=GPIO_ODR_5;
	EXTI->PR |= EXTI_PR_PR4;
	}
}

//Interrupt Handling Button 2/3

void EXTI2_3_IRQHandler(void)
{

	//BUT2
if(EXTI->PR & EXTI_PR_PR3)
{
	GPIOB->ODR^=GPIO_ODR_6;
	EXTI->PR |= EXTI_PR_PR3;
}
	//BUT3
if(EXTI->PR & EXTI_PR_PR2)
{
	GPIOB->ODR^=GPIO_ODR_7;
	EXTI->PR |= EXTI_PR_PR2;
}
}

