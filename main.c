#include "stm32f10x.h"
#include "MatrixKeypad.h"
#include "stdio.h" // для printf().
#include "types.h"

#define ARRLEN(a) sizeof(a)/sizeof(*a) // Количество элементов в массиве. 

static void InitHardware(void);
static void PrintKey(U16 keyCode, KeyEvent_t ev);

// Входы.
PinIn_t keypadIn[] = 
{
	{&GPIOB->IDR, /*адрес порта*/ 12 /*номер пина порта*/},
	{&GPIOB->IDR, 13},
	{&GPIOB->IDR, 5},
	{&GPIOB->IDR, 6},
};

// Выходы.
PinOut_t keypadOut[] = 
{
	{&GPIOB->ODR,/*адрес порта*/ 7 /*номер пина порта*/},
	{&GPIOB->ODR, 8},
	{&GPIOB->ODR, 9},
};
// Размер массива  = количество клавиш клавиатуры = кол-во строк * кол-ко столбцов.
U8 keyScanBuff[ARRLEN(keypadOut) * ARRLEN(keypadIn)];

MatrixKeypad_t keypad =
{
	keypadOut,
	ARRLEN(keypadOut),
	HIGH_LVL,
	
	keypadIn,
	ARRLEN(keypadIn),
	HIGH_LVL,
	
	keyScanBuff,
	ARRLEN(keyScanBuff),
	4, // количество циклов сканирования.
	
	PrintKey
};

int main()
{
	InitHardware();
	MatrixKeypad_Init(&keypad);
	
	while(1)
	{
		MatrixKeypad_ExecKeyHandlers(&keypad);
	}
}

void InitHardware()
{
	// Используется тактовая частота 72 МГц от рехонатора 8 МГц.
	// Настройка частоты происходит в SystemInit().
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
	// GPIOB4, 5, 6, 12 - input with pull-down;
	// GPIOB7, 8, 9 - Outputs mode, max speed 2 MHz.
	GPIOB->ODR = 0;
	GPIOB->CRL = GPIO_CRL_MODE7_1  |
								GPIO_CRL_CNF5_1  |
								GPIO_CRL_CNF6_1; 	
	GPIOB->CRH = GPIO_CRH_MODE8_1  |
								GPIO_CRH_MODE9_1 |
								GPIO_CRH_CNF12_1 |
								GPIO_CRH_CNF13_1;
	
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
  if(TIM1->SR & TIM_SR_UIF)
	{
		TIM1->SR = 0;
		MatrixKeypad_ScanProc(&keypad);
	}
}

void PrintKey(U16 keyCode, KeyEvent_t ev)
{
	if(ev == KEY_DOWN)
		printf("Key #%d pressed.\n", keyCode);
	else
		printf("Key #%d released.\n", keyCode);
	return;
}
