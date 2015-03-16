#include "stdafx.h"
#include "Задание 6 (экзамен).h"
using namespace std;
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void setValue(int val);
void changeNum(HMENU hMenu, HWND hWnd, int wmId);
void result();
void keyDown(HMENU hMenu, HWND hWnd, WPARAM wParam);
Calculator * calculate = new Calculator();

HWND htextbox;
char stringBuffer[MAXSTR];
int nFunc = 0;     // Function in use: 0=None, 1=Add, 2=Subtract, 3=Multiply, 4=Divide
int newNumber = 1;	   // New/continuing digits
double dTotal = 0; // Current buffer total
int calcSystem = 10;
int prevCalcSystem = 10;
int lastOperand;
char cBufNum[MAXSTR];
bool setMemory = false;
static bool dot, backsp;
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY6, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY6));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY6));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_GRADIENTINACTIVECAPTION);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MY6);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, 550, 270, NULL, NULL, hInstance, NULL);
	htextbox = CreateWindow("EDIT", "", WS_BORDER | ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_READONLY, 10, 0,
		420, 45, hWnd, (HMENU)11111, hInst, NULL);
	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu = GetMenu(hWnd);
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	char Symbols[17][2] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "." };
	char Operators[5][2] = { "+", "-", "*", "/", "=" };
	char cTemp[MAXSTR];
	switch (message)
	{
	case WM_CREATE:
	{
		int idButton;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				idButton = SYMBUTTON(j + 4 * i);
				CreateWindowEx(NULL, "BUTTON", (LPCSTR)Symbols[j + 4 * i], WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
					10 + (j * 70), 50 + (i * 30), 70, 24, hWnd, (HMENU)idButton, GetModuleHandle(NULL), NULL);
			}
		}
		CreateWindowEx(NULL, "BUTTON", "<-", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			440, 0, 80, 45, hWnd, (HMENU)OPBCKSP, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, "BUTTON", ".", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 170, 70, 24, hWnd, (HMENU)SYMDOT, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, "BUTTON", "Sqr()", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			150, 170, 70, 24, hWnd, (HMENU)OPSQR, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, "BUTTON", "Rev()", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			80, 170, 70, 24, hWnd, (HMENU)OPREV, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, "BUTTON", "C", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			440, 50, 80, 34, hWnd, (HMENU)OPC, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, "BUTTON", "CE", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			440, 90, 80, 34, hWnd, (HMENU)OPCE, GetModuleHandle(NULL), NULL);
		for (int i = 0; i < 4; i++)
		{
			idButton = OPBUTTON(i);
			CreateWindowEx(NULL, "BUTTON", (LPCSTR)Operators[i], WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				300, 50 + (i * 30), 70, 24, hWnd, (HMENU)idButton, GetModuleHandle(NULL), NULL);
		}
		idButton = OPBUTTON(4);
		CreateWindowEx(NULL, "BUTTON", (LPCSTR)Operators[4], WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			220, 170, 150, 24, hWnd, (HMENU)idButton, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, "BUTTON", "MC", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			380, 50, 50, 24, hWnd, (HMENU)MEMMC, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, "BUTTON", "MS", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			380, 80, 50, 24, hWnd, (HMENU)MEMMS, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, "BUTTON", "MR", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			380, 110, 50, 24, hWnd, (HMENU)MEMMR, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, "BUTTON", "M+", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			380, 140, 50, 24, hWnd, (HMENU)MEMMPL, GetModuleHandle(NULL), NULL);
		changeNum(hMenu, hWnd, IDM_10);
		calculate->setSystem(calcSystem);
		lastOperand = 0;
		break;
	}
	case WM_KEYDOWN:
	{
		keyDown(hMenu, hWnd, wParam);
		backsp = true;
		break;
	}
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		if (wmId >= SYMBUTTON(0) && wmId <= SYMBUTTON(17))
		{
			if (wmId == SYMDOT)
			{
				if (newNumber)
				{
					stringBuffer[0] = '0';
					newNumber = 0;
				}
				if ((!dot) && (strlen(stringBuffer) < MAXSTR))
				{
					strcat(&stringBuffer[1], ".");
					dot = true;
				}
				SetWindowText(htextbox, stringBuffer);
				backsp = true;
			}
			else{
				setValue(wmId - SYMBUTTON(0));
				backsp = true;
			}
		}
		else
			if (wmId >= OPBUTTON(0) && wmId <= OPBUTTON(9))
			{
				switch (wmId)
				{
				case OPBCKSP:					// Backspace clicked
				{
					if (backsp)
					{
						if (strlen(stringBuffer) > 1)
						{
							char lastSymbol[1];
							int stringLength = strlen(stringBuffer);
							lastSymbol[0] = stringBuffer[stringLength - 1];
							if (strncmp(lastSymbol, ".", 1) == 0)
							{
								dot = false;
							}
							stringBuffer[stringLength - 1] = '\0';
							SetWindowText(htextbox, stringBuffer);
						}
						else
						{
							if (strlen(stringBuffer) == 1)
							{
								strcpy(stringBuffer, "0\0");		// Clear char buffer
								SetWindowText(htextbox, stringBuffer);
								newNumber = 1;			// Set new digit
								dot = false;
							}
						}
					}
					break;
				}
				case OPC:					// BUTTON C clicked
				{
					strcpy(stringBuffer, "0\0");
					calculate->~Calculator();
					calculate = new Calculator();
					SetWindowText(htextbox, stringBuffer);
					newNumber = 1;					// Set new digit
					nFunc = 0;					// set function to none
					dot = false;
					lastOperand = 0;
					backsp = false;
					break;
				}
				case OPCE:				// BUTTON CE clicked
				{
					strcpy(stringBuffer, "0\0");
					calculate->setOperand(stringBuffer, calcSystem, 1);
					SetWindowText(htextbox, stringBuffer);
					newNumber = 1;			// Set new digit
					dot = false;
					lastOperand = OPCE;
					break;
				}
				case OPPLUS:				// '+' clicked
				{
					if (lastOperand == OPBUTTON(0))
					{
						calculate->setOperand(stringBuffer, calcSystem, 2);
						result();
					}
					nFunc = 0;				// Set function to add
					dot = false;
					backsp = false;
					calculate->setOperation(nFunc);
					calculate->setOperand(stringBuffer, calcSystem, 1);
					strcpy(stringBuffer, "\0");
					lastOperand = OPBUTTON(0);
					break;
				}
				case OPMINUS:				// '-' clicked
				{
					if (lastOperand == OPBUTTON(0))
					{
						calculate->setOperand(stringBuffer, calcSystem, 2);
						result();
					}
					nFunc = 1;				// Set function to subtract
					dot = false;
					backsp = false;
					calculate->setOperation(nFunc);
					calculate->setOperand(stringBuffer, calcSystem, 1);
					strcpy(stringBuffer, "\0");
					lastOperand = OPBUTTON(0);
					break;
				}
				case OPMUL:				// 'x' clicked
				{
					if (lastOperand == OPBUTTON(0))
					{
						calculate->setOperand(stringBuffer, calcSystem, 2);
						result();
					}
					nFunc = 2;				// Set function to multiply
					dot = false;
					backsp = false;
					calculate->setOperation(nFunc);
					calculate->setOperand(stringBuffer, calcSystem, 1);
					strcpy(stringBuffer, "\0");
					lastOperand = OPBUTTON(0);
					break;
				}
				case OPDEL:				// '/' clicked
				{
					if (lastOperand == OPBUTTON(0))
					{
						calculate->setOperand(stringBuffer, calcSystem, 2);
						result();
					}
					nFunc = 3;				// Set function to divide
					dot = false;
					backsp = false;
					calculate->setOperation(nFunc);
					calculate->setOperand(stringBuffer, calcSystem, 1);
					strcpy(stringBuffer, "\0");
					lastOperand = OPBUTTON(0);

					break;
				}
				case OPEQ:				// '=' clicked
				{
					if (lastOperand != 0)
					{
						if (lastOperand == OPBUTTON(0) && backsp == false)
							calculate->setOperand(stringBuffer, calcSystem, 0);
						else if (lastOperand == OPCE)
							calculate->setOperand(stringBuffer, calcSystem, 1);
						else if (lastOperand != OPEQ)
							calculate->setOperand(stringBuffer, calcSystem, 2);
						result();
						lastOperand = OPEQ;
						dot = false;
						backsp = false;
					}
					newNumber = 1;
					break;
				}
				case OPSQR:				// 'sqr' clicked
				{
					double dRoot;
					GetWindowText(htextbox, stringBuffer, MAXSTR);
					dRoot = pow(atof(stringBuffer), 2);
					strcpy(stringBuffer, "\0");			// Empty char buffer
					_gcvt(dRoot, MAXSTR, stringBuffer); // Convert floating point value to string
					SetWindowText(htextbox, stringBuffer);
					newNumber = 1;				// Set new digit
					dot = false;
					backsp = false;
					break;
				}
				case OPREV:				// '1/x' clicked
				{
					float dRecip;
					GetWindowText(htextbox, stringBuffer, MAXSTR);
					dRecip = 1 / atof(stringBuffer);
					strcpy(stringBuffer, "\0");			// Empty char buffer
					_gcvt(dRecip, 7, stringBuffer);
					SetWindowText(htextbox, stringBuffer);
					newNumber = 1;				// Set new digit
					dot = false;
					backsp = false;
					break;
				}
				}
			}
			else
				if (wmId >= IDM_2 && wmId <= IDM_16) // NUMSYSTEM clicked
				{
					changeNum(hMenu, hWnd, wmId);
					SetWindowText(htextbox, stringBuffer);
				}
				else if (wmId >= MEMMC && wmId <= MEMMPL)
				{
					switch (wmId)
					{
					case MEMMC:
					{
						calculate->setMemory(stringBuffer, calcSystem, 0);
						setMemory = false;
						newNumber = 1;				// Set new digit
						dot = false;
						backsp = false;
						break;
					}
					case MEMMS:
					{
						calculate->setMemory(stringBuffer, calcSystem, 1);
						newNumber = 1;				// Set new digit
						dot = false;
						setMemory = true;
						backsp = false;
						break;
					}
					case MEMMR:
					{
						strcpy(stringBuffer, calculate->getMemory());
						SetWindowText(htextbox, stringBuffer);
						newNumber = 1;				// Set new digit
						dot = false;
						backsp = false;
						break;
					}
					case MEMMPL:
					{
						if (setMemory == false)
						{
							calculate->setMemory(stringBuffer, calcSystem, 0);
							setMemory = true;
						}
						calculate->setMemory(stringBuffer, calcSystem, 2);
						newNumber = 1;				// Set new digit
						dot = false;
						backsp = false;
						break;
					}
					default:
						break;
					}
				}
				else switch (wmId)
				{
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
				break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	SetFocus(hWnd);
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void setValue(int val)
{
	char str[2];
	_itoa(val, str, 16);
	strcpy(str, calculate->low_upper(str));
	if (newNumber)
	{
		strcpy(stringBuffer, "\0");
		SetWindowText(htextbox, stringBuffer);
		if (val != 0) newNumber = 0;
	}
	if (strlen(stringBuffer) < MAXSTR)
	{
		strcat(&stringBuffer[0], str);
		SetWindowText(htextbox, stringBuffer);
	}
}


void changeNum(HMENU hMenu, HWND hWnd, int wmId){
	DWORD check;
	check = GetMenuState(hMenu, wmId, MF_BYCOMMAND);
	if (!(check & MF_CHECKED))
	{
		for (int i = IDM_2; i <= 3016; i++)
			CheckMenuItem(hMenu, i, MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(hMenu, wmId, MF_BYCOMMAND | MF_CHECKED);
		for (int i = (wmId - 2000); i <= 1016; i++)
			EnableWindow(GetDlgItem(hWnd, i), false);
		for (int i = 1000; i < (wmId - 2000); i++)
			EnableWindow(GetDlgItem(hWnd, i), true);
		prevCalcSystem = calcSystem;
		calcSystem = wmId - 3000;
		double buffer = calculate->doubleConverttoDec(stringBuffer, prevCalcSystem);
		strcpy(stringBuffer, "\0");
		char sign = (stringBuffer[0] == '-') ? '-' : '0';
		strcat(&stringBuffer[0], calculate->doubleConvertfromDec(buffer, calcSystem, sign));
	}
}

void result()
{
	calculate->Calculate();
	strcpy(stringBuffer, calculate->getResult());
	SetWindowText(htextbox, stringBuffer);
}

void keyDown(HMENU hMenu, HWND hWnd, WPARAM wParam)
{
	unsigned int key = wParam;
	DWORD check;
	if (key == 190)
	{
		if (newNumber)
		{
			stringBuffer[0] = '0';
			newNumber = 0;
		}
		if ((!dot) && (strlen(stringBuffer) < MAXSTR))
		{
			strcat(&stringBuffer[1], ".");
			dot = true;
		}
		SetWindowText(htextbox, stringBuffer);
		backsp = true;
	}
	if (key >= 48 && key <= 57)
	{
		key -= 48;
		if (key < calcSystem)
			setValue(key);
	}
	if (key >= 65 && key <= 70)
	{
		key -= 55;
		if (key < calcSystem)
			setValue(key);
	}
}