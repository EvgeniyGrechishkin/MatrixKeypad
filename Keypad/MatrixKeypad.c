#include "MatrixKeypad.h"

// RDY_FLAG_MASK - ����� ��� ����� ���������� ������� ������� � ���������.
#define RDY_FLAG_MASK (1 << 7)

// CYCLES_MASK - ����� ��� ��������� ���-�� ������ ������������ ������.
#define CYCLES_MASK ~RDY_FLAG_MASK


// SET_RDY ��� ��������� ����� ���������� ������� ������� � ���������.
#define SET_RDY(b) ((b) |= (1 << 7))


// CLR_RDY ��� ������ ����� ���������� ������� ������� � ���������.
#define CLR_RDY(b) ((b) &= ~(1 << 7))

// IS_RDY ��� �������� ����� ���������� ������� ������� � ���������.
#define IS_RDY(b) ((b) & (1 << 7))

// ��������� �������:
static inline void _SetPin(MatrixKeypad_t* keypad, U8 num);
static inline void _ClearPin(MatrixKeypad_t* keypad, U8 num);
static inline Bool _IsActivePin(const MatrixKeypad_t* keypad, U8 num);


// ���������� �� �������� ����������� ���� �������� �������.
	// keypad - �������������� ����������;
	// num - ���������� ����� ���� (� ������� ����������� �������).
void _SetPin(MatrixKeypad_t* keypad, U8 num)
{
	if (keypad->scanLevel == HIGH_LVL)
	{
		// �������� ������� - ������� (���������� "1" �� ��������� ����).
		*keypad->rows[num].portPtr |= (1UL << keypad->rows[num].pinNum);
	}
	else
	{
		// �������� ������� - ������ (���������� "0" �� ��������� ����).
		*keypad->rows[num].portPtr &= ~(1UL << keypad->rows[num].pinNum);
	}
}

// ������� ������ �������� ��������� ���������:
// - keypad ���� ��������� �� MatrixKeypad_t �������� ��������� ����������� ����������;
// - num ���� U8 �������� ����� ��������� ������ (� ������� keypad->rowLen),
// ������� ����� ���������� � ���������� ���������.
// �������� �� �������� ����������� ���� �������� �������.
	// keypad - �������������� ����������;
	// num - ���������� ����� ���� (� ������� ����������� �������).
void _ClearPin(MatrixKeypad_t* keypad, U8 num)
{
	if (keypad->scanLevel == HIGH_LVL)
	{
		// �������� ������� - ������� (���������� "0" �� ��������� ����).
		*keypad->rows[num].portPtr &= ~(1UL << keypad->rows[num].pinNum);
	}
	else
	{
		// �������� ������� - ������ (���������� "1" �� ��������� ����).
		*keypad->rows[num].portPtr |= (1UL << keypad->rows[num].pinNum);
	}
}

// ���������, ������� �� ��������� ������� ���. ���������� TRUE, ���� ��������� ��� �������.
	// keypad - �������������� ����������;
	// num - ���������� ����� ���� (� ������� ����������� ������).
Bool _IsActivePin(const MatrixKeypad_t* keypad, U8 num)
{
	if (keypad->activeLevel == HIGH_LVL)
	{
		// �������� ������� - �������. ����� "1" �� ��������� �����.
		return (*keypad->columns[num].portPtr & (1UL << keypad->columns[num].pinNum));
	}
	else
	{
		// �������� ������� - ������. ����� "0" �� ��������� �����.
		return !(*keypad->columns[num].portPtr & (1UL << keypad->columns[num].pinNum));
	}
}

// ������� ���������� ������������� �������� ��� �������������� �������� ������ �� ������� ������.
//  keypad - ����������� ����������.
void _ScanRow(const MatrixKeypad_t* keypad)
{
	for (U8 i = 0; i < keypad->columnsLen; i++)
	{
		U16 keyInd = keypad->columnsLen * keypad->_servData.scanStep + i;
		U8 rdyFlag = keypad->keyScanBuff[keyInd] & RDY_FLAG_MASK;
		keypad->keyScanBuff[keyInd] &= CYCLES_MASK;
		if (_IsActivePin(keypad, i))
		{
			// ������� ������.
			if (keypad->keyScanBuff[keyInd] < keypad->scanCycles)
			{
				keypad->keyScanBuff[keyInd]++;
				if (keypad->keyScanBuff[keyInd] == keypad->scanCycles)
				{
					SET_RDY(rdyFlag); // ���������� ���� ���������� � ���������.
				}
			}
		}
		else
		{
			// ������� �� ������.
			if (keypad->keyScanBuff[keyInd] >= keypad->scanCycles)
			{
				SET_RDY(rdyFlag); // ������� �������. ���������� ���� ����������.
			}
			keypad->keyScanBuff[keyInd] = 0;
		}
		keypad->keyScanBuff[keyInd] |= rdyFlag;
	}
}

// ���������������� ��������� ����������. ���������� ������.
void MatrixKeypad_Init(MatrixKeypad_t* keypad)
{
	if (!keypad)
	{
		return;
	}

	if (keypad->scanCycles > RDY_FLAG_MASK - 1)
	{
		// ������� ������� �������� ������� ������������. ����������� �������� �� ���������.
		keypad->scanCycles = RDY_FLAG_MASK - 1;
	}

	for (U8 i = 0; i < keypad->rowsLen; i++)
	{
		_ClearPin(keypad, i); // �������� ��� ���� � ���������� ���������.
	}

	for (U16 i = 0; i < keypad->keyScanBuff_Len; i++)
	{
		keypad->keyScanBuff[i] = 0; // �������� �����.
	}
    // ���������� ��������� ����. ���, ����� ������ ������ ������������ � 0.
	keypad->_servData.scanStep = keypad->rowsLen - 1;
}

// ������������ ��������� ����������.
// ���������� ����� ������������ ���������� ������� (��������, � ����������� �������).
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
		// �������� ������������ ���� ����� ����������.
		keypad->_servData.scanStep = 0;
	}
	_SetPin(keypad, keypad->_servData.scanStep);
}

// ���������� TRUE, ���� ������ ��������� �������.
	// keypad - �������������� ����������;
	// keyCode - ��� �������, ������� ������� �����������.
Bool MatrixKeypad_IsKeyPressed(const MatrixKeypad_t* keypad, U16 keyCode)
{
	if (!keypad)
	{
		return FALSE;
	}

	if (keyCode >= keypad->rowsLen * keypad->columnsLen)
	{ // ������ �������� ��� �������.
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

	if (!keypad->KeyEventCb) // ���������� �� ����������.
	{
		return;
	}

	for (U16 i = 0; i < keypad->keyScanBuff_Len; i++)
	{
		if (IS_RDY(keypad->keyScanBuff[i])) // ���� ���� ���������� � ���������.
		{
			CLR_RDY(keypad->keyScanBuff[i]); // ��� ���������� ��������� ���������.
			if (keypad->keyScanBuff[i] >= keypad->scanCycles)
			{
				keypad->KeyEventCb(i, KEY_DOWN); // ������� ������.
			}
			else if(keypad->keyScanBuff[i] == 0)
			{
				keypad->KeyEventCb(i, KEY_UP); // ������� ������.
			}
		}
	}
}
