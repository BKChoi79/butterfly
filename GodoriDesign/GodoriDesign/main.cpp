#include <stdio.h>
#include <windows.h>
#include <excpt.h>
#include <iostream>
#include <string>
#include <memory>
#include <vector>  
#include <algorithm>
#include <functional>
#include <DbgHelp.h>
#include "AppMain.h"
#include <tchar.h>

#include "GameScene.h"

#define _CRTDBG_MAP_ALLOC
void make_minidump(EXCEPTION_POINTERS* e)
{
	auto hDbgHelp = LoadLibraryA("dbghelp");
	if (hDbgHelp == nullptr)
		return;

	auto pMiniDumpWriteDump = (decltype(&MiniDumpWriteDump))GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	if (pMiniDumpWriteDump == nullptr)
		return;

	char name[MAX_PATH];
	{
		auto nameEnd = name + GetModuleFileNameA(GetModuleHandleA(0), name, MAX_PATH);
		SYSTEMTIME t;
		GetSystemTime(&t);
		wsprintfA(nameEnd - strlen(".exe"),
			"_%4d%02d%02d_%02d%02d%02d.dmp",
			t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	}

	auto hFile = CreateFileA(name, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
	exceptionInfo.ThreadId = GetCurrentThreadId();
	exceptionInfo.ExceptionPointers = e;
	exceptionInfo.ClientPointers = FALSE;

	auto dumped = pMiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		hFile,
		MINIDUMP_TYPE(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory),
		e ? &exceptionInfo : nullptr,
		nullptr,
		nullptr);

	CloseHandle(hFile);

	return;
}

LONG WINAPI UnhandledExcepFilter(PEXCEPTION_POINTERS pExcepPointers)
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)pExcepPointers->ContextRecord->Eax);
	pExcepPointers->ContextRecord->Eip += 2;

	/// dump file ³²±âÀÚ.
	make_minidump(pExcepPointers);

	return EXCEPTION_CONTINUE_EXECUTION;
}

/**

*/
void Start()
{
	AppMain* pApp = AppMain::Instance();

	GameScene* pScene = GameScene::create();
	pApp->Run(pScene);
}

/**

*/
HINSTANCE hInstance;
LPCWSTR lpszClass = (LPCWSTR)"First";
TCHAR szWindowClass[] = _T("win32app");
TCHAR szTitle[] = _T("Win32 Guided Application");

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

void OnExit()
{
	AppMain::Instance()->Destroy();
	printf("OnExit.");
}

void main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SetUnhandledExceptionFilter(UnhandledExcepFilter);

	/*
	// 
	HWND hWnd;
	WNDCLASS wndClass;
	static MSG msg;

	hInstance = GetModuleHandle(NULL);

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = NULL;
	wndClass.hCursor = NULL;
	wndClass.hIcon = NULL;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = NULL;
	RegisterClass(&wndClass);

	hWnd = CreateWindow(lpszClass, szTitle, 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
		NULL, 
		NULL, 
		hInstance, 
		NULL);
	
	if (!hWnd)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Win32 Guided Tour"), NULL);
	}
	else
	{
		ShowWindow(hWnd, 1);
	
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			//std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	
		
		auto FnGetMessageThread = []() -> void
		{
			while (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				//std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		};
		
		std::thread tfn(FnGetMessageThread);
		tfn.join();
		
	}
	*/

	// set exit callback.
	atexit(OnExit);

	// start App.
	Start();

	_CrtDumpMemoryLeaks();
	system("pause");
}
/*
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static bool MYSELLECT = false;
	const RECT r = { 0, 0, 500, 500 };

	enum COMMANDS
	{
		COMMAND_1,
		COMMAND_2,
	};

	switch (iMessage) {
	case WM_CREATE:
	{
		printf("========================================================\n");
		printf("window is now ready.\n");
		printf(".\n");
		printf(".\n");
		printf("\n\n\n");

		HWND h;
		h = CreateWindow(TEXT("button"), TEXT("Click Me"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 20, 100, 25, hWnd, (HMENU)COMMAND_1, hInstance, NULL);
		if (false == h)
		{
			MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("hBtn"), NULL);
		}

		h = CreateWindow(TEXT("button"), TEXT("TextOut"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 50, 100, 25, hWnd, (HMENU)COMMAND_2, hInstance, NULL);
		if (false == h)
		{
			MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("hBtn"), NULL);
		}
		return 0;
	}

	case WM_DESTROY:
	{
		AppMain::Instance()->Destroy();
		exit(0);

		PostQuitMessage(0);
		return 0;
	}

	case WM_COMMAND:
		printf("WM_COMMAND - %d.\n", wParam);

		switch (LOWORD(wParam))
		{
		case COMMAND_1:
			MYSELLECT = false;
			SendMessage(hWnd, WM_PAINT, COMMAND_1, 0);
			//InvalidateRect(hWnd, &r, TRUE);
			break;
		case COMMAND_2:
			MYSELLECT = true;
			SendMessage(hWnd, WM_PAINT, COMMAND_2, 0);
			//InvalidateRect(hWnd, &r, TRUE);
			break;
		}

		return 0;

	case WM_KEYDOWN:
		std::cout << wParam << std::endl;
		switch (wParam)
		{
		default:
			break;
		}

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (MYSELLECT == TRUE)
		{
			TextOut(hdc, 200, 200, TEXT("^_^"), 3);
		}
		else
		{
			TextOut(hdc, 200, 200, TEXT("T_T"), 3);
		}
			
		EndPaint(hWnd, &ps);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
*/