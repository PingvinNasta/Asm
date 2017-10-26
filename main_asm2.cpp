/*
   Задача: 
   struct Human //Человек
   {
      char Name[30];
      int  date;  //количество  дней от 1  января  1900  года  до  момента рождения
   };
   Вывести на экран список людей, у которых дни рождения в указанном месяце.
*/
#include <stdio.h>
#include <conio.h>
#include <iostream>
using namespace std;

const int SIZEARRAY = 20;
const int Days = 365;
const int Months[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 , 31};


int Month(int date)
{
	int d, m, y;
	// 1900 год - ВИСОКОСНЫЙ! 
	d = m = y = 0;
	int y1, day1 = 0, i;

	y = date / (Days * 4 + 1); // цикл из 4 лет
	y = y * 4;
	if ( date - y / 4 * (Days * 4 + 1) >= (Days * 3 + 1) ) y = y + 3; 
	else if ( date - y / 4 * (Days * 4 + 1) >= (Days * 2 + 1) ) y = y + 2;
	else if ( date - y / 4 * (Days * 4 + 1) >= (Days * 1 + 1) ) y = y + 1;
	if (y % 4 == 3) 
	{
	
		y1 = y - 3;
		day1 = date - y1 / 4 * (Days * 4 + 1) - (Days * 3 + 1);
		for ( i = 1; i < 13; i++ )
		{
			day1 = day1 - Months[i];
			if (day1 < 0) break;
			if (day1 == 0)
			{
				i++;
				break;
			}
		}
		m = i;
		if (day1 < 0) d = day1 + Months[i] + 1;
		if (day1 == 0) d = 1;
	}
	if (y % 4 == 2) 
	{
		y1 = y - 2;
		day1 = date - y1 / 4 * (Days * 4 + 1) - (Days * 2 + 1);
		for ( i = 1; i < 13; i++ )
		{
			day1 = day1 - Months[i];
			if (day1 < 0) break;
			if (day1 == 0)
			{
				i++;
				break;
			}
		}
		m = i;
		if (day1 < 0) d = day1 + Months[i] + 1;
		if (day1 == 0) d = 1;
	}
	if (y % 4 == 1) 
	{
		y1 = y - 1;
		day1 = date - y1 / 4 * (Days * 4 + 1) - (Days * 1 + 1);
		for ( i = 1; i < 13; i++ )
		{
			day1 = day1 - Months[i];
			if (day1 < 0) break;
			if (day1 == 0)
			{
				i++;
				break;
			}
		}
		m = i;
		if (day1 < 0) d = day1 + Months[i] + 1;
		if (day1 == 0) d = 1;
	}
	if (y % 4 == 0) // год ВИСОКОСНЫЙ
	{
		y1 = y;
		day1 = date - y1 / 4 * (Days * 4 + 1);
		for ( i = 1; i < 13; i++ )
		{
			if (i == 2) day1 = day1 - 1;
			day1 = day1 - Months[i];
			if (day1 < 0) break;
			if (day1 == 0) 
			{
				i++;
				break;
			}
		}
		m = i;
		if (day1 < 0 && i == 2) d = day1 + Months[i] + 1 + 1;
        if (day1 < 0 && i != 2) d = day1 + Months[i] + 1;
		if (day1 == 0) d = 1;
	}
	y = y + 1900;
	return m;
}

////////////////////////////////////////////
bool ReadInt(int &x)
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
////////////////////////////////////////////
struct Human
{
	char Name[30];
    int  date;
}; 


Human* CreateHuman(char *theName, int thedate)
// Создание экземляра структуры Human
{
	Human *Hum = 0;
	__asm{
		// выделяем память для структуры		
		mov		eax, SIZE Human // SIZE == 36! Поэтому date начинается с 32!
		push	eax
		call	dword ptr malloc
		add		esp, 4
		cmp		eax, 0
		je END 
		
		mov		ebx, theName
		mov		esi, eax // начало структуры
		mov		edx, eax // ещё одно
CYCLENAME:
		cmp		byte ptr [ebx], 0
		je ENDNAME
		mov		cl, [ebx]
		mov		[eax], cl
		inc		eax
		inc		ebx
		jmp		CYCLENAME
ENDNAME:
		mov		byte ptr [eax], 0 // конец строчки
		// копируем date
		mov		edi, thedate
		mov		[esi+32], edi 
END:
		mov Hum, edx
	}
	return Hum;
}

void DeleteHuman(Human **Hum) // Указатель на *Hum 
// Удаление экземляра структуры Human
{
	__asm{		
		mov eax, Hum // Загружаем указатель на указатель  
		push eax
		mov ebx, [eax] // В [eax] адрес *Hum
		push ebx
		call dword ptr free
		pop ebx
		pop eax
		xor ecx, ecx
		mov [eax], ecx // Обнуляем *Hum
	}
}

void PrintHuman(Human *Hum)
// Печать экземпляра структуры Human
{
	char strHuman[] = "Name: %s, Date: %i\n";
	char strNo[] = "This human dont create!\n";
	_asm{
		mov eax, Hum
		cmp eax, 0
		je NO
		add eax, 32
		mov ebx, [eax]
		push ebx
		sub eax, 32
		push eax
		lea eax, strHuman
		push eax
		call dword ptr printf
		add esp, 12
		jmp END
NO:
		lea eax, strNo
		push eax
		call dword ptr printf 
		add esp, 4
END:
	}
}

void Task(Human *Hum[], int mon) // Массив указателей *Hum
{
	char f[] = "%i\n";
	char strHuman[] = "Name: %s, Date: %i\n";
	__asm{
		// mov ecx, SIZEARRAY
		mov esi, mon
		lea eax, Hum // по адресу eax, то есть в [eax] лежит адрес начала Hum[]
		mov ebx, [eax] // в ebx адрес начала Hum[]
		xor edx, edx // счётчик по массиву
		sub edx, 1
CYCLE:		
		add edx, 1
		cmp edx, SIZEARRAY
		je NEXT
		mov ecx, [ebx + 4*edx] // в ecx начало H[0+edx]
		cmp ecx, 0
		je CYCLE
		add ecx, 32
		mov edi, dword ptr [ecx] // в edi поле date
		// save registers
		push eax
		push ebx
		push ecx
		push edx
		
		push edi		
		call dword ptr Month
		add esp, 4
		cmp eax, esi
		
		// return registers
		pop edx
		pop ecx
		pop ebx
		pop eax

		jne CYCLE
		
		
		// save registers
		push eax
		push ebx
		push ecx
		push edx

		// print humans
		sub ecx, 32
		push ecx
		call dword ptr PrintHuman
		add esp, 4
		
		// print numbers of humans
		//push edx
		//lea edx, f
		//push edx
		//call dword ptr printf
		//add esp, 4
		//pop edx

		// return registers
		pop edx
		pop ecx
		pop ebx
		pop eax
		jmp CYCLE
NEXT:
	}
}

int main()
{
	//Human *H = 0;
	//cout << H;
	//H = CreateHuman("1234567890 0987654321", 1990);
	//cout << H->Name << " " << H->date << endl;
	//PrintHuman(H);
	//DeleteHuman(&H);
	
	//PrintHuman(H);


	//Human *H[2]={0};
	//H[0] = CreateHuman("1234567890", 1990);
	//H[1] = CreateHuman("0987654321", 1999);
	//Task(H, 1995);
	//PrintHuman(H[0]);
	//PrintHuman(H[1]);

	bool menu;
	char key; int n;
	char Name[30]; int date;

	Human *H[SIZEARRAY] = {0};
	menu = true;

	while (menu)
	{
	system("cls");
	cout << "1. Create human" << endl;
	cout << "2. Delete human" << endl;
	cout << "3. Print human" << endl;
	cout << "4. Task" << endl;
	cout << "5. Exit" << endl;
    key = _getch();
	switch(key)
	{
		case '1': 
			{
				do{
					cout << "Enter number of human from 0 to " << SIZEARRAY - 1 << ": ";
				}while (!ReadInt(n) || (n < 0) || (n > SIZEARRAY - 1));
				
				cin.ignore();				
				cout << "Enter name of human: ";
				cin.getline(Name, 29);
				
				do{
					cout << "Enter date of human: ";
				}while(!ReadInt(date) || (date < 0));

				if (H[n]) DeleteHuman(&H[n]);
				H[n] = CreateHuman(Name, date);
				break;
			}
		case '2': 
			{
				do{
					cout << "Enter number of human from 0 to " << SIZEARRAY - 1 << ": ";
				}while (!ReadInt(n) || (n < 0) || (n > SIZEARRAY - 1));

				if (H[n]) DeleteHuman(&H[n]);
				break;
            }
		case '3': 
			{
				do{
					cout << "Enter number of human from 0 to " << SIZEARRAY - 1 << ": ";
				}while (!ReadInt(n) || (n < 0) || (n > SIZEARRAY - 1));

				PrintHuman(H[n]);
				system("PAUSE");
				break;
            }
		case '4': 
			{
				do{
					cout << "Enter month from 1 to 12: ";
				}while(!ReadInt(n) || (n < 1) || (n > 12));
				Task(H, n);
				_getch();
				break;
			}
		case '5': 
			{
				menu = false;
				for (int i = 0; i < SIZEARRAY; i++)
					DeleteHuman(&H[i]);
				break;
			}
		default: 
			{
				cout << "Incorrect! Try again!" << endl;
				_getch();
				break;
            }
	} // switch
	} // while

	system("PAUSE");
	return 0; 
}