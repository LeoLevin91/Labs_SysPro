// TestAsse.cpp: определяет точку входа для консольного приложения.
//
/*
(a-b*4-1)/(c/31+a*b);
*/


#include "stdafx.h"
#include <iostream>
using namespace std;

long CppFunc(long& a, long& b, long& c) {
	long denominator = (c / 31 + a * b);
	if (denominator == 0) {
		cout << "Делеине на ноль" << endl;
		return LONG_MAX;
	}
	return (a - b * 4 - 1) / denominator;
}

long AsmFunc(long a, long b, long c) {
	int zero = 0;
	int over = 0;
	long res;
	//(a-b*4-1)/(c/31+a*b);
	__asm
	{
			mov eax, a;		<eax> = a
			mov ebx, b;		<ebx> = b
			imul ebx;		<eax> = a * b
			jo err_over
			push eax;		В стеке : a * b

			mov eax, c;
			mov ecx, 31
			cdq;
			idiv ecx; < eax >= c / 31
			pop ebx; < ebx >= a * b
			add eax, ebx; < eax >= c / 31 + a * b
			jo err_over
			jz err_zero;

			push eax; pushh знаминатель

			mov eax, b;		<eax>= b
			mov ecx, 4;		ecx = 4
			imul ecx;		eax = eax * ecx

			jo err_over;

			inc eax;		eax = eax + 1
			jo err_over
			mov ebx, a;		ebx = a
			sub ebx, eax;	ebx - eax
			jo err_over
			mov eax, ebx;	eax = ebx

			pop ebx
			cdq
			idiv ebx;	eax= (a - b * 4 - 1) / (c / 31 + a * b);
		
			jmp writeRes

			err_zero:
				mov zero, 1;
				jmp writeRes

			err_over:
				mov over, 1; 
				jmp writeRes

			writeRes:
				mov res, eax
	}
	if (zero)
	{
		cout << "Произошло деление на 0" << endl;
		return LONG_MAX;
	}
	else if (over)
	{
		cout << "Произошло переполнение" << endl;
		return 0;
	}
	else return res;
}


int main()
{
	setlocale(LC_ALL, "RUS");
	long a, b, c = 0;
	cout << "a,b,c: " << endl;
	cin >> a >> b >> c;
	cout << "Res asm: " << AsmFunc(a, b, c) << endl;
	cout << "Res C++: " << CppFunc(a, b, c) << endl;
	system("pause");
	return 0;
}

