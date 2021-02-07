#include "main.h"


void GpioInit(void);
void RccInit(void);


int main(void)
{

	RccInit();

	GpioInit();



	while(1)
	{

		GPIOA->BSRR|=GPIO_BSRR_BR7;
		GPIOA->BSRR|=GPIO_BSRR_BS6;

	}
}



void GpioInit(void)
{
RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;

	/*PIN6*/
	GPIOA->MODER|=GPIO_MODER_MODE6_0;
	GPIOA->OTYPER|=GPIO_OTYPER_OT6;
	GPIOA->PUPDR|=GPIO_PUPDR_PUPD6_0;
	GPIOA->BSRR|=GPIO_BSRR_BS6;

	/*PIN7*/

	GPIOA->MODER|=GPIO_MODER_MODE7_0;
	GPIOA->OTYPER|=GPIO_OTYPER_OT7;
	GPIOA->PUPDR|=GPIO_PUPDR_PUPD7_0;
	GPIOA->BSRR|=GPIO_BSRR_BS7;

}


void RccInit(void)
{

RCC->CR|=RCC_CR_HSEON;                   //включение внешнего кварца
while(!(RCC->CR & RCC_CR_HSERDY));       //флаг готовности HSERDY
FLASH->ACR|=FLASH_ACR_LATENCY_5WS;     	 //задержка для флэш памяти
RCC->CFGR|=RCC_CFGR_SW_HSE;			     //Внешний кварц выбран в качестве системной частоты
while(!(RCC->CFGR &= RCC_CFGR_SWS_HSE)); //Проверка тактирования от внешнего кварца


//Вывод частот на пины
//MCO1-HSE (PA8)
RCC->CFGR|=RCC_CFGR_MCO1_1; //Выбор HSE

//MCO2-System clock (PC9)
RCC->CFGR&= ~RCC_CFGR_MCO2; //Выбор SYSCLK

}







