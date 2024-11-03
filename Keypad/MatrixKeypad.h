#ifndef __MATRIX_KEYPAD__
#define __MATRIX_KEYPAD__

#include "stdint.h"

// ������ �������
#define ARRLEN(a) sizeof(a)/sizeof(*a)

// ����
typedef uint8_t  U8_t;
typedef uint16_t U16_t;
typedef uint32_t U32_t;
typedef uint32_t Size_t;
typedef uint8_t Bool;

// ��������� �� ����. �������� ��� ������ ����������� �����
typedef uint32_t *PortPtr_t;


// ������ �� �����
typedef enum
{
	LOW_LVL =  0, // ������ (���. 0)
	HIGH_LVL = 1  // ������� (���. 1)
} PinLevel;

// �������� ����
typedef struct
{
	PortPtr_t portPtr; // ��������� �� ����, �������� ����������� ���
	U8_t pinNum;       // ����� ���� ����� (������� � 0) 
} Pin_t;

// ��� ������� �������
typedef enum
{
	KEY_UP,  // ������� ������
	KEY_DOWN // ������� ������
} KeyEvent_t;

// ����������� ��������� ����������
typedef struct
{
	// ������ ������ ���� ��������� �� �����
	Pin_t *rows; // ������ ����� �����
	U8_t rowsLen; // ������ �������
	PinLevel scanLevel; // �������, ���������� ��� ������������

	// ������� ������ ���� ��������� �� �����
	const Pin_t *columns; // ������ ����� ��������
	U8_t columnsLen; // ������ ������
	PinLevel activeLevel; // ������� ��� ������� �������

	U8_t scanCycles; // ���-�� ������, ��� ������� ������� ��������� �������

	// ���������� ��� ����������� ������� / ���������� �������
		// keyCode - ��� �������
		// ev - ��� ������� ������� (������� / ����������)
	void(*KeyEventCb)(U32_t keyCode, KeyEvent_t ev);

	// ��������� � ��������. ���������� ������������. ������� �� ������!
	struct
	{
		U32_t scanStep;
	} _servData;

} MatrixKeypad_t;

//----------------------------------

void MatrixKeypad_Init(MatrixKeypad_t *keypad);

void MatrixKeypad_Scan(MatrixKeypad_t *keypad);


#endif // !__MATRIX_KEYPAD__

