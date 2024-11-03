#include "MatrixKeypad.h"

//
static void _OnOutput(MatrixKeypad_t * keypad, Size_t num);
static void _OffOutput(MatrixKeypad_t * keypad, Size_t num);

void _OnOutput(MatrixKeypad_t * keypad, Size_t num)
{
	if (keypad->activeLevel == HIGH_LVL)
	{
		*keypad->rows[0].portPtr |= (1UL << keypad->rows[num].pinNum);
	}
	else
	{
		*keypad->rows[0].portPtr &= ~(1UL << keypad->rows[num].pinNum);
	}
}

void _OffOutput(MatrixKeypad_t * keypad, Size_t num)
{
	if (keypad->activeLevel == HIGH_LVL)
	{
		*keypad->rows[0].portPtr &= ~(1UL << keypad->rows[num].pinNum);
	}
	else
	{
		*keypad->rows[0].portPtr |= (1UL << keypad->rows[num].pinNum);
	}
}

void MatrixKeypad_Scan(MatrixKeypad_t * keypad)
{
	//keypad->rowsLen
   // Прочитать предыдущий
	_OffOutput(keypad, keypad->_servData.scanStep);
	keypad->_servData.scanStep++;
	if (keypad->_servData.scanStep == keypad->rowsLen)
	{
		// Окончено сканирование всех рядов клавиатуры
		keypad->_servData.scanStep = 0;
	}
	_OnOutput(keypad, keypad->_servData.scanStep);


}
