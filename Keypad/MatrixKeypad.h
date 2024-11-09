
#ifndef __MATRIX_KEYPAD__
#define __MATRIX_KEYPAD__

#include "types.h"


// PortPtr_t - ��������� �� ����. ��� ��������� ������ ���������������
#define PortPtr_t unsigned int volatile* // ��� 8-���������� �����.

// PortOutPtr_t - ��������� �� �������� ����.
#define PortOutPtr_t PortPtr_t


// PortInPtr_t - ����������� ��������� �� ������� ����. ��������� ������ ������.
#define PortInPtr_t const PortPtr_t

// ������������ ���������� ������ �� ������ �����.
typedef enum
{
	LOW_LVL = 0, // ������ (���. 0).
	HIGH_LVL = 1  // ������� (���. 1).
} PinLevel_t;

// ��������� ���������� �������� ����� �����.
// ���� ���������:
// - portPtr ���� PortPtr_t ���������� ����, �������� ����������� �����;
// - pinNum ���� U8 ���������� ����� ������ �����.
typedef struct
{
	PortOutPtr_t portPtr;
	U8 pinNum;
} PinOut_t;

// ��������� ���������� ������� ����� �����. ��������� ������ ������.
// ���� ���������:
// - portPtr ���� PortPtr_t ���������� ����, �������� ����������� �����;
// - pinNum ���� U8 ���������� ����� ������ �����.
typedef struct
{
	PortInPtr_t portPtr;
	U8 pinNum;
} PinIn_t;

// ������������ ���������� ��� ������� �������.
// �������� ������������:
// - KEY_UP - ������� ������;
// - KEY_DOWN - ������� ������.
typedef enum
{
	KEY_UP,  // ������� ������.
	KEY_DOWN // ������� ������.
} KeyEvent_t;

// ��������� �������� ��������� � ��������� ��������� ����������.
typedef struct
{
	// ������ ������ ���� ��������� �� �����.
	PinOut_t* rows; // ������ ������� �����;
	U8 rowsLen; // ������ �������.
	PinLevel_t scanLevel; // �������� �������, ���������� ��� ������������.

	// ������� ������ ���� ��������� �� ����.
	PinIn_t* columns; // ������ ������� ��������.
	U8 columnsLen; // ������ ������.
	PinLevel_t activeLevel; // ������� �������, ���������� ��� ������� �������.
    // �����, ���������� ��������� ������ ��� ������������.
	// ������ ������ = rowsLen * columnsLen.
	U8* keyScanBuff;
	U16 keyScanBuff_Len; // ������ ������. ����� ���������� ������ ����������.
    // ���-�� ������, ��� ������� ������� ��������� �������.
	// ��������� �������� 1...126.
	U8 scanCycles;

	// ���������� ������� / ���������� �������.
	// ���������� ����������� �� ��������� ����� �������� MatrixKeypad_ExecKeyHandlers().
	// ���� ������� NULL, �� ������ ���������� �� ����������.
		// keyCode - ��� �������;
		// ev - ��� ������� ������� (������� / ����������).
	void(*KeyEventCb)(U16 keyCode, KeyEvent_t ev);
	// ��������� � ��������. ���������� ������������.
	struct
	{
		U8 scanStep;
	} _servData;
} MatrixKeypad_t;

// ������������� ����������. ���������� ������ ����� ������� ������������.
// ���� ��������� keypad ������ ���� ������� �����������.
//  keypad - ����������� ����������.
void MatrixKeypad_Init(MatrixKeypad_t* keypad);


// ����������� ����������. ���������� ���������� ����� �������� ���������� �������.
//  keypad - ����������� ����������.
void MatrixKeypad_ScanProc(MatrixKeypad_t* keypad);

// ���������� TRUE, ���� ������ ��������� �������.
//  keypad - ����������� ����������
//  keyCode - ��� �������
Bool MatrixKeypad_IsKeyPressed(const MatrixKeypad_t* keypad, U16 keyCode);


// ���������� ���-�� ����������� ������� ������.
//  keypad - ����������� ����������.
U16 MatrixKeypad_PressedKeysNum(const MatrixKeypad_t* keypad);

// �������� ��� N-��� ������� �������.
// ������: ������ ������� 5 � 10. ��� ������ �-�� � num == 0,
// existsKey ��������������� � TRUE,
// � keyCode � 5. ���� num == 1,
// �� existsKey ��������������� � TRUE, � keyCode � 10.
// keypad - ����������� ����������.
// num - ���������� ����� ������� �������.
// existsKey - ������� �������.
// keyCode - ��� ������� �������.
void MatrixKeypad_PressedKeyCode(const MatrixKeypad_t* keypad, U16 num, Bool* existsKey, U16* keyCode);


// �-� ��������� �� ��������� ����� ������ ����������� ������� ���������� ��� ������
// ������� / ������� ������� ����������.
void MatrixKeypad_ExecKeyHandlers(const MatrixKeypad_t* keypad);

#endif
