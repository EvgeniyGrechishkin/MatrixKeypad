
#ifndef MODULE_T_FILE_TYPES_H_
#define MODULE_T_FILE_TYPES_H_

// Фундаментальные типы.

	// Целочисленные типы с фиксированной разрядностью.
	typedef signed char      SByte;
	typedef signed char      Int8;
	typedef signed short int Int16;
	typedef signed long  int Int32;
	typedef signed long long Int64;
	typedef unsigned char      Byte;
	typedef unsigned char      UInt8;
	typedef unsigned short int UInt16;
	typedef unsigned long  int UInt32;
	typedef unsigned long long UInt64;

	// Целочисленные типы с разрядностью не меньше указанной и наиболее близкой к ней.
	typedef signed char      Least8;
	typedef signed short int Least16;
	typedef signed long  int Least32;
	typedef signed long long Least64;
	typedef unsigned char      ULeast8;
	typedef unsigned short int ULeast16;
	typedef unsigned long  int ULeast32;
	typedef unsigned long long ULeast64;

	typedef signed char      IntL8;
	typedef signed short int IntL16;
	typedef signed long  int IntL32;
	typedef signed long long IntL64;
	typedef unsigned char      UIntL8;
	typedef unsigned short int UIntL16;
	typedef unsigned long  int UIntL32;
	typedef unsigned long long UIntL64;

	// Целочисленные типы с разрядностью не меньше указанной и наиболее быстрые по выполнению.
	typedef int Fast8;
	typedef int Fast16;
	typedef int Fast32;
	typedef long long Fast64;
	typedef unsigned int UFast8;
	typedef unsigned int UFast16;
	typedef unsigned int UFast32;
	typedef unsigned long long UFast64;

	typedef int IntF8;
	typedef int IntF16;
	typedef int IntF32;
	typedef long long IntF64;
	typedef unsigned int UIntF8;
	typedef unsigned int UIntF16;
	typedef unsigned int UIntF32;
	typedef unsigned long long UIntF64;

	//! Псевдонимы для целых чисел с фиксированной разрядностью.
	typedef UInt32 U32;
	typedef UInt16 U16;
	typedef Byte U8;

	// Псевдонимы для типов с разрядностью системы.
	typedef unsigned int UInt;
	typedef signed int Int;


	#ifdef __cplusplus
		#define EXTERN_C extern "C"
		typedef bool Bool;
		#define FALSE false
		#define TRUE true
	#else
		#define EXTERN_C
		typedef _Bool Bool;
		#define FALSE 0
		#define TRUE 1
		#define false 0
		#define true 1
	#endif

#endif
