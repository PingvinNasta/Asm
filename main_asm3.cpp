/* 
   Задача: 
   Среди множества точек (заданного массивом) указать все тройки без учета
   порядка, которые лежат на одной прямой. Определение того, что три точки 
   лежат на одной прямой, выделить в отдельную подпрограмму.
*/
#include <conio.h>
#include <stdio.h>
#include <malloc.h>
#include <iostream>
using namespace std;

const int QUANTITY = 15;

struct Point
{
    double x;
    double y;
    double z;
};

bool ReadInt(int &x)
// Чтение целого числа
{
	bool f = 1;
	cin >> x;
	if (cin.fail())
	{
		f=0;
	    cin.clear();
	    while (cin.peek() != '\n') cin.ignore();
	    if (cin.peek()=='\n') cin.ignore(); 		
	}
	return f;
}

bool ReadDouble(double &x)
// Чтение вещественного числа
{
	bool f = 1;
	cin >> x;
	if (cin.fail())
	{
		f=0;
	    cin.clear();
	    while (cin.peek() != '\n') cin.ignore();
	    if (cin.peek()=='\n') cin.ignore(); 		
	}
	return f;
}

Point* CreatePoint(double a, double b, double c)
// Создание Point
{
	Point* P = 0;
	__asm{
		mov		eax, SIZE Point
		push	eax
		call	dword ptr malloc
		add		esp, 4
		cmp		eax, 0
		je END 
		finit
		fld		a
		fstp	qword ptr [eax]
		fld		b
		add		eax, 8
		fstp	qword ptr [eax]
		fld		c
		add		eax, 8
		fstp	qword ptr [eax]
		sub		eax, 16
END:
		mov P,	eax
	}
	return P;
}

void DeletePoint(Point** P)
// Удаление Point 
{
	__asm{
		mov		eax, P
		push	eax
		mov		ebx, [eax]
		push	ebx
		call	dword ptr free
		pop		ebx
		pop		eax
		mov		dword ptr [eax], 0
	}
}

void PrintPoint(Point* P)
// Печать Point
{
	char sPoint[] = "Point: %lf, %lf, %lf. \n";
	char sNo[] = "No point! \n";
	__asm{
		mov		eax, P
		cmp		eax, 0
		je NO
		finit
		sub		esp, 16 
		fld		qword ptr[eax+16]
		fstp	qword ptr[esp+16]
		
		fld		qword ptr[eax+8]
		fstp	qword ptr[esp+8]

		fld		qword ptr[eax]
		fstp	qword ptr[esp]

		lea		eax, sPoint
		push	eax
		call	dword ptr printf
		add		esp, 20 
		jmp END
NO:
		lea		eax, sNo
		push	eax
		call	dword ptr printf
		add		esp, 4
END:
	}
}

double dLength(Point* A, Point* B)
// Расстояние между точками A и B
{
	__asm{
		mov		eax, A
		mov		ebx, B
		finit
		fld		qword ptr[eax] 
		fsub	qword ptr[ebx]
		fabs
		fld		st(0)
		fmulp	st(1), st(0)
		
		fld		qword ptr[eax+8]
		fsub	qword ptr[ebx+8]
		fabs	
		fld		st(0)
		fmulp	st(1), st(0)
		faddp	st(1), st(0) // x2+y2
		
		fld		qword ptr[eax+16]
		fsub	qword ptr[ebx+16]
		fabs
		fld		st(0)
		fmulp	st(1), st(0)
		faddp	st(1), st(0)
		
		fsqrt
	}
}

bool bLine(Point* A, Point* B, Point* C)
// Проверка принадлежности точек A, B, C одной прямой
{
	double tmp, eps = 0.000000001;
	__asm{
		mov		edx, A
		cmp		edx, 0
		je NO
		mov		ebx, B
		cmp		ebx, 0
		je NO
		mov		ecx, C
		cmp		ecx, 0
		je NO
	
		finit
		// AB + BC = AC
			push	ebx
		push	edx
		push	ecx
		call	dword ptr dLength // st(0) = AC
		pop		ecx
		pop		edx	
			pop		ebx
		
			push	ecx
		push	edx
		push	ebx
		fst		tmp // +
		call	dword ptr dLength // st(0) = AB, // - st(1) = AC
		fld		tmp // +
		pop		ebx
		pop		edx
			pop		ecx
		fsubp	st(1), st(0) // st(0) = AC - AB
		fabs // +
			push	edx
		push	ebx
		push	ecx
		fst tmp
		call	dword ptr dLength // st(0) = BC, st(1) = AC - AB
		fld tmp
		pop		ecx
		pop		ebx
			pop		edx
		fsubp	st(1), st(0) // st(0) = AC - AB - BC
		fabs
		fld eps
		fcom	st(1)
		fstsw	ax
		sahf
		jnc TRUE
		
		// AC + CB = AB
			push	ecx
		push	edx
		push	ebx
		call	dword ptr dLength // st(0) = AB
		pop		ebx
		pop		edx
			pop		ecx
		
			push	ebx
		push	edx
		push	ecx
		fst tmp
		call	dword ptr dLength // st(0) = AC, st(1) = AB
		fld tmp
		pop		ecx
		pop		edx
			pop		ebx
		fsubp	st(1), st(0) // st(0) = AB - AC
		fabs

			push	edx
		push	ebx
		push	ecx
		fst tmp
		call	dword ptr dLength // st(0) = CB, st(1) = AB - AC
		fld tmp
		pop		ecx
		pop		ebx
			pop		edx
		fsubp	st(1), st(0) // st(0) = AB - AC - CB
		fld eps
		fcom	st(1)
		fstsw	ax
		sahf
		jnc TRUE

		// BA + AC = BC
			push	edx
		push	ebx
		push	ecx
		call	dword ptr dLength // st(0) = BC
		pop		ecx
		pop		ebx
			pop		edx
		
			push	ebx
		push	edx
		push	ecx
		fst tmp
		call	dword ptr dLength // st(0) = AC, st(1) = BC
		fld tmp
		pop		ecx
		pop		edx
			pop		ebx
		fsubp	st(1), st(0) // st(0) = BC - AC
		fabs
			
			push	ecx
		push	ebx
		push	edx
		fst tmp
		call	dword ptr dLength // st(0) = BA, st(1) = BC - AC
		fld tmp
		pop		edx
		pop		ebx
			pop		ecx
		fsubp	st(1), st(0) // st(0) = BC - AC - BA
		fld eps
		fcom	st(1)
		fstsw	ax
		sahf
		jnc TRUE
NO:
		xor		eax, eax
		jmp END
TRUE:
		mov		eax, 1
END:
	}
}

void LineInArr(Point** Arr)
{
	
	char sLine[] = "Line consists of point %i, %i, %i.\n";
	int i, j, k, count;
	__asm{
		// счётчики
		mov i, 0
		mov j, 1
		mov k, 2
		mov ecx, QUANTITY
		sub ecx, 2
		mov count, ecx 
		
		// циклы
CYCLE_I:
		mov esi, i
		mov edi, count
		cmp esi, edi
		je END

		//
		mov esi, i
		inc esi
		mov j, esi
		//
CYCLE_J:
		mov esi, j
		mov edi, count
		inc edi
		cmp esi, edi
		pushf
		mov edx, i
		inc edx
		mov i, edx
		popf
		jz CYCLE_I
		mov edx, i
		dec edx
		mov i, edx

		//
		mov esi, j
		inc esi
		mov k, esi
		//

CYCLE_K:
		mov esi, k
		mov edi, count
		add edi, 2
		cmp esi, edi
		pushf
		mov edx, j
		inc edx
		mov j, edx
		popf
		je CYCLE_J 
		// k = QUAN
		
		mov edx, j
		dec edx
		mov j, edx

		// bLine
		mov esi, Arr

		mov ecx, k
		mov eax, [esi + 4*ecx]
		mov ecx, eax
		push ecx
		
		mov ebx, j
		mov eax, [esi + 4*ebx]
		mov ebx, eax
		push ebx

		mov eax, i
		mov ebx, [esi + 4*eax]
		mov eax, ebx
		push eax

		call dword ptr bLine
		add esp, 12
		mov ecx, k
		inc ecx
		mov k, ecx
		cmp eax, 0
		je CYCLE_K
		// bLine == 1
		
		mov ecx, k
		dec ecx
		push ecx

		mov ebx, j
		push ebx

		mov eax, i
		push eax

		lea edi, sLine
		push edi
		call dword ptr printf
		add esp, 16


		jmp CYCLE_K
END:
	}

}


int main1()
{
	Point *A = CreatePoint(0, 0, 0);
	Point *B = CreatePoint(1, 1, 1);
	Point *C = CreatePoint(2, 2, 2);
	cout << dLength(A,B) << " " << dLength(B, C) << " " << dLength(A, C) << endl;
	cout << bLine(A, B, C);
	return 0;
}

int main()
{
	bool menu;
	char key;
	int n;
	double x, y, z;

	Point *Arr[QUANTITY] = {0};
	menu = true;

	while (menu)
	{
	system("cls");
	cout << "1. Create point" << endl;
	cout << "2. Delete point" << endl;
	cout << "3. Print point" << endl;
	cout << "4. Print all" << endl; 
	cout << "5. Task" << endl;
	cout << "6. Exit" << endl;
    key = _getch();
	switch(key)
	{
		case '1': 
			{
				cout << endl;
				do{
					cout << "Enter number of point from 0 to " << QUANTITY - 1 << ": ";
				}while (!ReadInt(n) || (n < 0) || (n > QUANTITY - 1));
				
				do{
					cout << "Enter x of point: ";
				}while(!ReadDouble(x));

				do{
					cout << "Enter y of point: ";
				}while(!ReadDouble(y));

				do{
					cout << "Enter z of point: ";
				}while(!ReadDouble(z));

				if (Arr[n]) DeletePoint(&Arr[n]);
				Arr[n] = CreatePoint(x, y, z);
				break;
			}
		case '2': 
			{
				cout << endl;
				do{
					cout << "Enter number of point from 0 to " << QUANTITY - 1 << ": ";
				}while (!ReadInt(n) || (n < 0) || (n > QUANTITY - 1));

				if (Arr[n]) DeletePoint(&Arr[n]);
				break;
            }
		case '3': 
			{
				cout << endl;
				do{
					cout << "Enter number of human from 0 to " << QUANTITY - 1 << ": ";
				}while (!ReadInt(n) || (n < 0) || (n > QUANTITY - 1));

				PrintPoint(Arr[n]);
				_getch();
				break;
            }
		case '4':
			{
				cout << endl;
				for (int i = 0; i < QUANTITY; i++)
				{
					cout << i << ". "; PrintPoint(Arr[i]);
				}
				_getch();
				break;
			}
		case '5': 
			{
				cout << endl;
				LineInArr(Arr);
				_getch();
				break;
			}
		case '6': 
			{
				menu = false;
				for (int i = 0; i < QUANTITY; i++)
					DeletePoint(&Arr[i]);
				break;
			}
		default: 
			{
				cout << "\nIncorrect! Try again!" << endl;
				_getch();
				break;
            }
	} // switch
	} // while

	return 0; 
}