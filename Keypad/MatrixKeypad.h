
#ifndef __MATRIX_KEYPAD__
#define __MATRIX_KEYPAD__

#include "types.h"


// PortPtr_t - указатель на порт. Тип указателя должен соответствовать
#define PortPtr_t unsigned int volatile* // Для 8-разрядного порта.

// PortOutPtr_t - указатель на выходной порт.
#define PortOutPtr_t PortPtr_t


// PortInPtr_t - константный указатель на входной порт. Разрешено только чтение.
#define PortInPtr_t const PortPtr_t

// Перечисление определяет уровни на выходе порта.
typedef enum
{
	LOW_LVL = 0, // Низкий (лог. 0).
	HIGH_LVL = 1  // Высокий (лог. 1).
} PinLevel_t;

// Структура определяет выходной вывод порта.
// Поля структуры:
// - portPtr типа PortPtr_t определяет порт, которому принадлежит вывод;
// - pinNum типа U8 определяет номер вывода порта.
typedef struct
{
	PortOutPtr_t portPtr;
	U8 pinNum;
} PinOut_t;

// Структура определяет входной вывод порта. Разрешено только чтение.
// Поля структуры:
// - portPtr типа PortPtr_t определяет порт, которому принадлежит вывод;
// - pinNum типа U8 определяет номер вывода порта.
typedef struct
{
	PortInPtr_t portPtr;
	U8 pinNum;
} PinIn_t;

// Перечисление определяет тип события клавиши.
// Значения перечисления:
// - KEY_UP - клавиша отжата;
// - KEY_DOWN - клавиша нажата.
typedef enum
{
	KEY_UP,  // Клавиша отжата.
	KEY_DOWN // Клавиша нажата.
} KeyEvent_t;

// Стркутура содержит настройки и состояния матричной клавиатуры.
typedef struct
{
	// Строки должны быть настроены на выход.
	PinOut_t* rows; // Массив выводов строк;
	U8 rowsLen; // Размер массива.
	PinLevel_t scanLevel; // Выходной уровень, подаваемый при сканировании.

	// Столбцы должны быть настроены на вход.
	PinIn_t* columns; // Массив выводов столбцов.
	U8 columnsLen; // Размер масива.
	PinLevel_t activeLevel; // Входной уровень, получаемый при нажатой клавиши.
    // Буфер, содержащий состояние клавиш при сканировании.
	// РАЗМЕР БУФЕРА = rowsLen * columnsLen.
	U8* keyScanBuff;
	U16 keyScanBuff_Len; // Размер буфера. Равен количеству клавиш клавиатуры.
    // Кол-во циклов, при котором клавиша считается нажатой.
	// Принимает значения 1...126.
	U8 scanCycles;

	// Обработчик нажатия / отпускания клавиши.
	// Обработчик запускается из основного цикла функцией MatrixKeypad_ExecKeyHandlers().
	// Если указано NULL, то данный обработчик не вызывается.
		// keyCode - код клавиши;
		// ev - тип события клавиши (нажатие / отпускание).
	void(*KeyEventCb)(U16 keyCode, KeyEvent_t ev);
	// Состояния и счетчики. Изменяются автоматичеки.
	struct
	{
		U8 scanStep;
	} _servData;
} MatrixKeypad_t;

// Инициализация клавиатуры. Вызывается первой перед началом сканирования.
// Поля структуры keypad должны быть заранее установлены.
//  keypad - сканируемая клавиатура.
void MatrixKeypad_Init(MatrixKeypad_t* keypad);


// Сканировать клавиатуру. Вызывается циклически через заданный промежуток времени.
//  keypad - сканируемая клавиатура.
void MatrixKeypad_ScanProc(MatrixKeypad_t* keypad);

// Возвращает TRUE, если нажата указанная клавиша.
//  keypad - сканируемая клавиатура
//  keyCode - код клавиши
Bool MatrixKeypad_IsKeyPressed(const MatrixKeypad_t* keypad, U16 keyCode);


// Возвращает кол-во одноврменно нажатых клавиш.
//  keypad - сканируемая клавиатура.
U16 MatrixKeypad_PressedKeysNum(const MatrixKeypad_t* keypad);

// Получить код N-ной нажатой клавиши.
// Пример: нажаты клавиши 5 и 10. При вызове ф-ии с num == 0,
// existsKey устанавливается в TRUE,
// а keyCode в 5. Если num == 1,
// то existsKey устанавливается в TRUE, а keyCode в 10.
// keypad - сканируемая клавиатура.
// num - порядковый номер нажатой клавиши.
// existsKey - клавиша найдена.
// keyCode - код нажатой клавиши.
void MatrixKeypad_PressedKeyCode(const MatrixKeypad_t* keypad, U16 num, Bool* existsKey, U16* keyCode);


// Ф-я выполняет из основного цикла запуск обработчика событий клавиатуры для клавиш
// нажатия / отжатия которых определены.
void MatrixKeypad_ExecKeyHandlers(const MatrixKeypad_t* keypad);

#endif
