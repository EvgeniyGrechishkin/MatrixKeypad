#include "MatrixKeypad.h"

// RDY_FLAG_MASK - маска для флага готовности события клавиши к обработке.
#define RDY_FLAG_MASK (1 << 7)

// CYCLES_MASK - маска для получения кол-ва циклов сканирования клавиш.
#define CYCLES_MASK ~RDY_FLAG_MASK


// SET_RDY для установки флага готовности события клавиши к обработке.
#define SET_RDY(b) ((b) |= (1 << 7))


// CLR_RDY для сброса флага готовности события клавиши к обработке.
#define CLR_RDY(b) ((b) &= ~(1 << 7))

// IS_RDY для проверки флага готовности события клавиши к обработке.
#define IS_RDY(b) ((b) & (1 << 7))

// Прототипы функций:
static inline void _SetPin(MatrixKeypad_t* keypad, U8 num);
static inline void _ClearPin(MatrixKeypad_t* keypad, U8 num);
static inline Bool _IsActivePin(const MatrixKeypad_t* keypad, U8 num);


// Установить на выходном сканирающем пине активный уровень.
	// keypad - обрабатываемая клавиатура;
	// num - Абсолютный номер пина (в массиве сканирующих выходов).
void _SetPin(MatrixKeypad_t* keypad, U8 num)
{
	if (keypad->scanLevel == HIGH_LVL)
	{
		// Активный уровень - высокий (установить "1" на указанном пине).
		*keypad->rows[num].portPtr |= (1UL << keypad->rows[num].pinNum);
	}
	else
	{
		// Активный уровень - низкий (установить "0" на указанном пине).
		*keypad->rows[num].portPtr &= ~(1UL << keypad->rows[num].pinNum);
	}
}

// Функция должна получать следующие аргументы:
// - keypad типа указатель на MatrixKeypad_t содержит настройки сканируемой клавиатуры;
// - num типа U8 сожертит номер выходного вывода (в массиве keypad->rowLen),
// который нажно установить в неактивное состояние.
// Сбросить на выходном сканирающем пине активный уровень.
	// keypad - обрабатываемая клавиатура;
	// num - абсолютный номер пина (в массиве сканирующих выходов).
void _ClearPin(MatrixKeypad_t* keypad, U8 num)
{
	if (keypad->scanLevel == HIGH_LVL)
	{
		// Активный уровень - высокий (установить "0" на указанном пине).
		*keypad->rows[num].portPtr &= ~(1UL << keypad->rows[num].pinNum);
	}
	else
	{
		// Активный уровень - низкий (установить "1" на указанном пине).
		*keypad->rows[num].portPtr |= (1UL << keypad->rows[num].pinNum);
	}
}

// Проверить, активен ли указанный входной пин. Возвращает TRUE, если указанный пин активен.
	// keypad - обрабатываемая клавиатура;
	// num - абсолютный номер пина (в массиве сканирующих входов).
Bool _IsActivePin(const MatrixKeypad_t* keypad, U8 num)
{
	if (keypad->activeLevel == HIGH_LVL)
	{
		// Активный уровень - высокий. Поиск "1" на указанном входе.
		return (*keypad->columns[num].portPtr & (1UL << keypad->columns[num].pinNum));
	}
	else
	{
		// Активный уровень - низкий. Поиск "0" на указанном входе.
		return !(*keypad->columns[num].portPtr & (1UL << keypad->columns[num].pinNum));
	}
}

// Функция производит сканировкание столбцов при установившемся активном уровне на текущей строке.
//  keypad - сканируемая клавиатура.
void _ScanRow(const MatrixKeypad_t* keypad)
{
	for (U8 i = 0; i < keypad->columnsLen; i++)
	{
		U16 keyInd = keypad->columnsLen * keypad->_servData.scanStep + i;
		U8 rdyFlag = keypad->keyScanBuff[keyInd] & RDY_FLAG_MASK;
		keypad->keyScanBuff[keyInd] &= CYCLES_MASK;
		if (_IsActivePin(keypad, i))
		{
			// Клавиша нажата.
			if (keypad->keyScanBuff[keyInd] < keypad->scanCycles)
			{
				keypad->keyScanBuff[keyInd]++;
				if (keypad->keyScanBuff[keyInd] == keypad->scanCycles)
				{
					SET_RDY(rdyFlag); // Установить флаг готовности к обработке.
				}
			}
		}
		else
		{
			// Клавиша не нажата.
			if (keypad->keyScanBuff[keyInd] >= keypad->scanCycles)
			{
				SET_RDY(rdyFlag); // Клавиша отжаата. Установить флаг готовности.
			}
			keypad->keyScanBuff[keyInd] = 0;
		}
		keypad->keyScanBuff[keyInd] |= rdyFlag;
	}
}

// Инициализировать матричную клавиатуру. Вызывается первой.
void MatrixKeypad_Init(MatrixKeypad_t* keypad)
{
	if (!keypad)
	{
		return;
	}

	if (keypad->scanCycles > RDY_FLAG_MASK - 1)
	{
		// Слишком большое значение попыток сканирования. Установлено значение по умолчанию.
		keypad->scanCycles = RDY_FLAG_MASK - 1;
	}

	for (U8 i = 0; i < keypad->rowsLen; i++)
	{
		_ClearPin(keypad, i); // Сбросить все пины в неактивное состояние.
	}

	for (U16 i = 0; i < keypad->keyScanBuff_Len; i++)
	{
		keypad->keyScanBuff[i] = 0; // Очистить буфер.
	}
    // Установить последний скан. пин, чтобы начать первое сканирование с 0.
	keypad->_servData.scanStep = keypad->rowsLen - 1;
}

// Сканирование матричной клавиатуры.
// Вызывается через определенный промежуток времени (Например, в обработчике таймера).
void MatrixKeypad_ScanProc(MatrixKeypad_t* keypad)
{
	if (!keypad)
	{
		return;
	}
	_ScanRow(keypad);
	_ClearPin(keypad, keypad->_servData.scanStep);
	keypad->_servData.scanStep++;
	if (keypad->_servData.scanStep == keypad->rowsLen)
	{
		// Окончено сканирование всех рядов клавиатуры.
		keypad->_servData.scanStep = 0;
	}
	_SetPin(keypad, keypad->_servData.scanStep);
}

// Возвращает TRUE, если нажата указанная клавиша.
	// keypad - обрабатываемая клавиатура;
	// keyCode - код клавиши, нажатие которое проверяется.
Bool MatrixKeypad_IsKeyPressed(const MatrixKeypad_t* keypad, U16 keyCode)
{
	if (!keypad)
	{
		return FALSE;
	}

	if (keyCode >= keypad->rowsLen * keypad->columnsLen)
	{ // Указан неверный код клавиши.
		return FALSE;
	}
	return ((keypad->keyScanBuff[keyCode] & CYCLES_MASK) >= keypad->scanCycles);
}

U16 MatrixKeypad_PressedKeysNum(const MatrixKeypad_t* keypad)
{
	if (!keypad)
	{
		return 0;
	}

	U16 keyNum = 0;
	for (U16 i = 0; i < keypad->keyScanBuff_Len; i++)
	{
		if((keypad->keyScanBuff[i] & CYCLES_MASK) >= keypad->scanCycles)
		{
			keyNum++;
		}
	}
	return keyNum;
}

void MatrixKeypad_PressedKeyCode(const MatrixKeypad_t* keypad, U16 num, Bool* existsKey, U16* keyCode)
{
	if (!keypad || !existsKey || !keyCode || num >= keypad->keyScanBuff_Len)
	{
		return;
	}
	*existsKey = FALSE;
	*keyCode = 0;

	U16 curNum = 0;
	for (U16 i = 0; i < keypad->keyScanBuff_Len; i++)
	{
		if ((keypad->keyScanBuff[i] & CYCLES_MASK) >= keypad->scanCycles)
		{
			if (num == curNum)
			{
				*existsKey = TRUE;
				*keyCode = i;
				return;
			}
			else
			{
				curNum++;
			}
		}
	}
}

void MatrixKeypad_ExecKeyHandlers(const MatrixKeypad_t* keypad)
{
	if (!keypad)
	{
		return;
	}

	if (!keypad->KeyEventCb) // Обработчик не установлен.
	{
		return;
	}

	for (U16 i = 0; i < keypad->keyScanBuff_Len; i++)
	{
		if (IS_RDY(keypad->keyScanBuff[i])) // Есть флаг готовности к обработке.
		{
			CLR_RDY(keypad->keyScanBuff[i]); // Для исключения повторной обработки.
			if (keypad->keyScanBuff[i] >= keypad->scanCycles)
			{
				keypad->KeyEventCb(i, KEY_DOWN); // Клавиша нажата.
			}
			else if(keypad->keyScanBuff[i] == 0)
			{
				keypad->KeyEventCb(i, KEY_UP); // Клавиша отжата.
			}
		}
	}
}
