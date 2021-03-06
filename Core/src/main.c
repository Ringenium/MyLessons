#include "main.h"


void GpioInit(void);
void RccInit(void);
void TimInit(void);
void ExtIntInit(void);
void UartInit(void);

volatile uint8_t But=0;


int main(void)
{

	RccInit();
	GpioInit();
	TimInit();
	ExtIntInit();
	UartInit();

	while(1)
	{

		while(!(USART2->SR & USART_SR_TC));
			USART2->DR=0xAA;


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

RCC->PLLCFGR|=RCC_PLLCFGR_PLLSRC; 		 //HSE на вход PLL

RCC->PLLCFGR|=RCC_PLLCFGR_PLLM_2;  		// /M (Поделил на 4, получил 2 МГц) frequency is between 1 and 2 MHz.
RCC->PLLCFGR|=RCC_PLLCFGR_PLLN_1|RCC_PLLCFGR_PLLN_4|RCC_PLLCFGR_PLLN_5; // *N  (Умножил на 50 получил 100МГц) frequency is between 100 and 432 MHz
RCC->PLLCFGR|=RCC_PLLCFGR_PLLP_0;  		// /P (Поделил на 4, получил 25 МГц)

RCC->CR|=RCC_CR_PLLON;				     //включение PLL
while(!(RCC->CR & RCC_CR_PLLRDY));	     //флаг готовности PLLRDY
RCC->CFGR|=RCC_CFGR_SW_PLL; 			 //PLL в качестве SYSCLK
RCC->CFGR|=RCC_CFGR_HPRE_DIV2;			 // /2 получаю 12.5MHz настройка AHB делителя
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
//TIM2->PSC=25000-1;							//PSC=(тактовая частота/1000)-1     (сколько отсчетов в секунду он сделает.Пример частота 25МГц PSC=(25МГц/1000)-1 делим 25МГц на получившиеся 25КГц получаем 1К-за секунду таймер досчитает до 1К)
TIM2->PSC=8000-1;
TIM2->ARR=1000-1;							//Значение до которого считает таймер
TIM2->DIER|=TIM_DIER_UIE;					//прерывание
TIM2->CR1|=TIM_CR1_CEN;						//statrt timer
NVIC_EnableIRQ(TIM2_IRQn);					//разрешение прерывания	глобально

}

void ExtIntInit(void)
{
	/*Настраиваю кнопку на внешнее прерывание*/
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOEEN;
	RCC->APB2ENR|=RCC_APB2ENR_SYSCFGEN;
	/*Плавающий вход*/
	GPIOE->MODER&=~GPIO_MODER_MODE3;
	GPIOE->PUPDR&=~GPIO_PUPDR_PUPD3;

	/*Внешнее прерывание*/
	SYSCFG->EXTICR[0]|=SYSCFG_EXTICR1_EXTI3_PE;
	EXTI->IMR|=EXTI_IMR_MR3;
	EXTI->FTSR|=EXTI_FTSR_TR3;
	NVIC_EnableIRQ(EXTI3_IRQn);
}

void UartInit(void)
{
			/*USART1*/
/*
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;

	GPIOA->AFR[1]|=GPIO_AFRH_AFSEL9_0|GPIO_AFRH_AFSEL9_1|GPIO_AFRH_AFSEL9_2; 		//AF7
	GPIOA->AFR[1]|=GPIO_AFRH_AFSEL10_0|GPIO_AFRH_AFSEL10_1|GPIO_AFRH_AFSEL10_2;		//AF7

	GPIOA->MODER|=GPIO_MODER_MODE9_1; //AF
	GPIOA->MODER|=GPIO_MODER_MODE10_1;//AF

	RCC->APB2ENR|=RCC_APB2ENR_USART1EN;


	USART1->CR1 &= ~USART_CR1_M; 	//Word length
	USART1->CR2 &= ~USART_CR2_STOP; //stop bits
	//USART1->BRR=0x517;				//9600 ((fck+br/2))/9600
	USART1->BRR=0x341;
	USART1->CR1|=USART_CR1_TE;		//transmitter enable
	USART1->CR1|=USART_CR1_RE;		//receiver enable
	USART1->CR1 |= USART_CR1_RXNEIE;   //Interrupt RX
	NVIC_EnableIRQ(USART1_IRQn);       //vector interrupt
	USART1->CR1|=USART_CR1_UE;   	//usart enable
*/


		/*USART2*/
	//PA2-TX
	//PA3-RX
/*
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
	GPIOA->AFR[0]|=GPIO_AFRL_AFSEL2_0|GPIO_AFRL_AFSEL2_1|GPIO_AFRL_AFSEL2_2; 		//AF7
	GPIOA->AFR[0]|=GPIO_AFRL_AFSEL3_0|GPIO_AFRL_AFSEL3_1|GPIO_AFRL_AFSEL3_2;		//AF7

	GPIOA->MODER|=GPIO_MODER_MODE2_1; //AF
	GPIOA->MODER|=GPIO_MODER_MODE3_1;//AF

	RCC->APB1ENR|=RCC_APB1ENR_USART2EN;

	USART2->CR1 &= ~USART_CR1_M; 	//Word length
	//USART2->CR2 &= ~USART_CR2_STOP; //stop bits
	//USART2->BRR=0x517;				//9600 ((fck+br/2))/9600
	USART2->BRR=0x341;
	USART2->CR1|=USART_CR1_TE;		//transmitter enable
	USART2->CR1|=USART_CR1_RE;		//receiver enable
	USART2->CR1 |= USART_CR1_RXNEIE;   //Interrupt RX
	NVIC_EnableIRQ(USART2_IRQn);       //vector interrupt
	USART2->CR1|=USART_CR1_UE;     	   //usart enable
*/


}







void TIM2_IRQHandler(void)
{
	//Отработка прерывания//
GPIOA->ODR ^= ~GPIO_ODR_OD7;

TIM2->SR&= ~TIM_SR_UIF; //сброс прерывания
}

void EXTI3_IRQHandler(void)
{
	while(GPIOE->IDR & GPIO_IDR_ID3);
	if(!(GPIOE->IDR & GPIO_IDR_ID3))
	{
	EXTI->PR = EXTI_PR_PR3;
	GPIOA->ODR ^= ~GPIO_ODR_OD6;
	}

}

void USART2_IRQHandler(void)
{
	if(USART1->SR & USART_SR_RXNE)
	{
		USART1->SR &= ~USART_SR_RXNE;
	}
}
