#ifndef __MATRIX_KEYPAD__
#define __MATRIX_KEYPAD__

#include "stdint.h"

// Размер массива
#define ARRLEN(a) sizeof(a)/sizeof(*a)

// Типы
typedef uint8_t  U8_t;
typedef uint16_t U16_t;
typedef uint32_t U32_t;
typedef uint32_t Size_t;
typedef uint8_t Bool;

// Указатель на порт. Изменить при другой разрядности порта
typedef uint32_t *PortPtr_t;


// Уровни на пинах
typedef enum
{
	LOW_LVL =  0, // Низкий (лог. 0)
	HIGH_LVL = 1  // Высокий (лог. 1)
} PinLevel;

// Описание пина
typedef struct
{
	PortPtr_t portPtr; // Указатель на порт, которому принадлежит пин
	U8_t pinNum;       // Номер пина порта (начиная с 0) 
} Pin_t;

// Тип события клавиши
typedef enum
{
	KEY_UP,  // Клавиша отжата
	KEY_DOWN // Клавиша нажата
} KeyEvent_t;

// Сканируемая матричная клавиатура
typedef struct
{
	// Строки должны быть настроены на выход
	Pin_t *rows; // Массив пинов строк
	U8_t rowsLen; // Размер массива
	PinLevel scanLevel; // Уровень, подаваемый при сканировании

	// Столбцы должны быть настроены на входы
	const Pin_t *columns; // Массив пинов столбцов
	U8_t columnsLen; // Размер масива
	PinLevel activeLevel; // Уровень при нажатой клавиши

	U8_t scanCycles; // Кол-во циклов, при котором клавиша считается нажатой

	// Вызывается при обнаружении нажатия / отпускания клавиши
		// keyCode - код клавиши
		// ev - тип события клавиши (нажатие / отпускание)
	void(*KeyEventCb)(U32_t keyCode, KeyEvent_t ev);

	// Состояния и счетчики. Изменяются автоматичеки. Вручную не менять!
	struct
	{
		U32_t scanStep;
	} _servData;

} MatrixKeypad_t;

//----------------------------------

void MatrixKeypad_Init(MatrixKeypad_t *keypad);

void MatrixKeypad_Scan(MatrixKeypad_t *keypad);


#endif // !__MATRIX_KEYPAD__

