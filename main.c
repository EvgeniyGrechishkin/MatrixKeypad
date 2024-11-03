#include "stm32f10x.h"
#include "MatrixKeypad.h"
#include "stdio.h"

static void InitHardware(void);
static void PrintKey(U32_t keyCode, KeyEvent_t ev);

Pin_t keypadIn[] = 
{
	{(PortPtr_t)GPIOB, 12},
	{(PortPtr_t)GPIOB, 4},
	{(PortPtr_t)GPIOB, 5},
	{(PortPtr_t)GPIOB, 6},
};

Pin_t keypadOut[] = 
{
	{(PortPtr_t)GPIOB, 7},
	{(PortPtr_t)GPIOB, 8},
	{(PortPtr_t)GPIOB, 9},
};

MatrixKeypad_t keypad =
{
	keypadOut,
	ARRLEN(keypadOut),
	HIGH_LVL,
	
	keypadIn,
	ARRLEN(keypadIn),
	HIGH_LVL,
	
	2,
	
	PrintKey
};

int main()
{
	InitHardware();
	//MatrixKeypad_Init(&keypad);
	while(1)
	{
		__NOP();
	}
}

void InitHardware()
{
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
	// GPIOB4, 5, 6, 12 - input with pull-down;
	// GPIOB7, 8, 9 - Outputs mode, max speed 2 MHz.
	GPIOB->ODR = 0;
	GPIOB->CRL |= GPIO_CRL_MODE7_1|
								GPIO_CRL_CNF4_1 |
								GPIO_CRL_CNF5_1 |
								GPIO_CRL_CNF6_1; 	
	GPIOB->CRH |= GPIO_CRH_MODE8_1|
								GPIO_CRH_MODE9_1|
								GPIO_CRH_CNF12_1;
	
	#define TIM1_CLK 2000
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);
	TIM1->CR1 = 0; // Off timer. Reset settings.
	TIM1->PSC = 0;
	TIM1->ARR = SystemCoreClock / TIM1_CLK-1;
	TIM1->DIER = TIM_DIER_UIE;
	TIM1->CR1 |= TIM_CR1_CEN; // Counter enable.
	TIM1->EGR |= TIM_EGR_UG; // Generate an update of the registers.
	TIM1->SR = 0; // Clear all flags.
	
	NVIC_EnableIRQ(TIM1_UP_IRQn);
	__enable_irq();
}

void TIM1_UP_IRQHandler(void)
{
	static U32_t cnt = 0;
  if(TIM1->SR & TIM_SR_UIF)
	{
		TIM1->SR = 0;
		//MatrixKeypad_Scan(&keypad);
		
		if(cnt == 2000)
		{
			const char * str = "aaa";
			printf("%sHello world %d\n", str, 50);
			cnt = 0;
		} 
		cnt++;
	}
}

void PrintKey(U32_t keyCode, KeyEvent_t ev)
{
	if(ev == KEY_DOWN)
		printf("Key #%d pressed. ", keyCode);
	else
		printf("Key #%d released. ", keyCode);
}
