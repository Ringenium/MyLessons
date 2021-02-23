#include "main.h"


void GpioInit(void);
void RccInit(void);
void TimInit(void);
void ButtonInit(void);


volatile uint8_t LoadMCU=0;
volatile uint8_t But=0;


int main(void)
{

	RccInit();
	GpioInit();
	TimInit();
	ButtonInit();

	uint32_t st=0;

	while(1)
	{
st=0;
if(But==0)
{
while(!(GPIOE->IDR & GPIO_IDR_ID3))
{
	st++;
	if(st>=1000)
	{
	But=1;
	}
}
}else
{
	while(!(GPIOE->IDR & GPIO_IDR_ID3))
	{
		But=0;
	}
}



	}
}



void GpioInit(void)
{
RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;

	/*PINA6*/
	GPIOA->MODER|=GPIO_MODER_MODE6_0;
	GPIOA->OTYPER|=GPIO_OTYPER_OT6;
	GPIOA->PUPDR|=GPIO_PUPDR_PUPD6_0;
	GPIOA->BSRR|=GPIO_BSRR_BS6;

	/*PINA7*/

	GPIOA->MODER|=GPIO_MODER_MODE7_0;
	GPIOA->OTYPER|=GPIO_OTYPER_OT7;
	GPIOA->PUPDR|=GPIO_PUPDR_PUPD7_0;
	GPIOA->BSRR|=GPIO_BSRR_BS7;

	/*PINA8*/

	GPIOA->MODER|=GPIO_MODER_MODE8_1;
	GPIOA->OTYPER &=~ GPIO_OTYPER_OT8;
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFRH0;

	/*PINC9*/
	GPIOC->MODER|=GPIO_MODER_MODE9_1;
	GPIOC->OTYPER &=~ GPIO_OTYPER_OT9;
	GPIOC->AFR[1] &= ~GPIO_AFRH_AFRH1;


}

void RccInit(void)
{

				/*Системная частота от HSE */
/*
RCC->CR|=RCC_CR_HSEON;                   //включение внешнего кварца
while(!(RCC->CR & RCC_CR_HSERDY));       //флаг готовности HSERDY
FLASH->ACR|=FLASH_ACR_LATENCY_5WS;     	 //задержка для флэш памяти
RCC->CFGR|=RCC_CFGR_SW_HSE;			     //Внешний кварц выбран в качестве системной частоты
while(!(RCC->CFGR &= RCC_CFGR_SWS_HSE)); //Проверка тактирования от внешнего кварца
*/
				/*Системная частота от PLL*/
RCC->CR|=RCC_CR_HSEON;                   //включение внешнего кварца
while(!(RCC->CR & RCC_CR_HSERDY));       //флаг готовности HSERDY
FLASH->ACR|=FLASH_ACR_LATENCY_5WS;     	 //задержка для флэш памяти


RCC->PLLCFGR|=RCC_PLLCFGR_PLLSRC;  //HSE на вход PLL

RCC->PLLCFGR|=RCC_PLLCFGR_PLLM_2;  // /M (Поделил на 4, получил 2 МГц) frequency is between 1 and 2 MHz.
RCC->PLLCFGR|=RCC_PLLCFGR_PLLN_1|RCC_PLLCFGR_PLLN_4|RCC_PLLCFGR_PLLN_5; // *N  (Умножил на 50 получил 100МГц) frequency is between 100 and 432 MHz
RCC->PLLCFGR|=RCC_PLLCFGR_PLLP_0;  // /P (Поделил на 4, получил 25 МГц)





RCC->CR|=RCC_CR_PLLON;				     //включение PLL
while(!(RCC->CR & RCC_CR_PLLRDY));	     //флаг готовности PLLRDY
RCC->CFGR|=RCC_CFGR_SW_PLL; 			 //PLL в качестве SYSCLK
RCC->CFGR|=RCC_CFGR_HPRE_3;				 // /2 получаю 12.5MHz настройка AHB делителя
while(!(RCC->CFGR &= RCC_CFGR_SWS_PLL)); //Проверка тактирования от PLL

//Вывод частот на пины
//MCO1-HSE (PA8)
RCC->CFGR|=RCC_CFGR_MCO1_1; //Выбор HSE

//MCO2-System clock (PC9)
RCC->CFGR&= ~RCC_CFGR_MCO2; //Выбор SYSCLK


RCC->CFGR|= RCC_CFGR_MCO2_0|RCC_CFGR_MCO2_1; //Выбор PLL
}

void TimInit(void)
{
				/*Настройка прерывания от таймера по переполнению*/

RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;			//тактирование таймера
TIM2->PSC=25000-1;							//PSC=(тактовая частота/1000)-1     (сколько отсчетов в секунду он сделает.Пример частота 25МГц PSC=(25МГц/1000)-1 делим 25МГц на получившиеся 25КГц получаем 1К-за секунду таймер досчитает до 1К)
TIM2->ARR=1000-1;							//Значение до которого считает таймер
TIM2->DIER|=TIM_DIER_UIE;					//прерывание
TIM2->CR1|=TIM_CR1_CEN;						//statrt timer
NVIC_EnableIRQ(TIM2_IRQn);					//разрешение прерывания	глобально

}

void ButtonInit(void)
{

	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOEEN;

	GPIOE->MODER&=~GPIO_MODER_MODE3;
	//GPIOE->OTYPER |= GPIO_OTYPER_OT3;
	GPIOE->PUPDR|=GPIO_PUPDR_PUPD3_0;

}

void TIM2_IRQHandler(void){
	//Первое включение//
	if(LoadMCU<4)
	{
	LoadMCU++;
	GPIOA->ODR ^= ~GPIO_ODR_OD7;
	if(LoadMCU==4)
	{
	TIM2->ARR=100-1;							//Значение до которого считает таймер
	//TIM2->CR1&=~TIM_CR1_CEN;						//stop timer
	}
	}

	//Отработка кнопки //
	if(But==1)
	{
		GPIOA->BSRR|=GPIO_BSRR_BR7;
		But=2;
	}
	if(But==0)
	{
		GPIOA->BSRR|=GPIO_BSRR_BS7;
	}

TIM2->SR&= ~TIM_SR_UIF; //сброс прерывания
}
