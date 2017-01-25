/**
 * @file winmain.c
 * @desc Main program.
 */

#include <windows.h>
#include <commctrl.h>
#include "resource.h"

/**
 * About dialog box function.
 */
BOOL CALLBACK
AboutDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {

	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case IDOK:
			EndDialog(hwnd, IDOK);
			return TRUE;

		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			return TRUE;

		}
		return FALSE;

	}
	return FALSE;
}

static void
Paint(HWND hwnd, PAINTSTRUCT *ps)
{
	HWND hStatusBar;
	RECT rcStatusBar, rc;
	int iStatusHeight;
	HFONT hOldFont, hFont;
	LPCTSTR message = TEXT("Hello, world :)");

	hStatusBar = GetDlgItem(hwnd, IDW_STATUSBAR);
	GetWindowRect(hStatusBar, &rcStatusBar);
	iStatusHeight = rcStatusBar.bottom - rcStatusBar.top;

	GetClientRect(hwnd, &rc);
	rc.bottom -= iStatusHeight;

	hFont = CreateFont(-MulDiv(32, GetDeviceCaps(ps->hdc, LOGPIXELSY), 72), 0,
		GM_ADVANCED, 0,
		FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_DONTCARE,
		TEXT("Tahoma"));
	hOldFont = (HFONT) SelectObject(ps->hdc, hFont);
	DrawText(ps->hdc, message, lstrlen(message), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	SelectObject(ps->hdc, hOldFont);
	DeleteObject(hFont);
}

static void
Size(HWND hwnd)
{
	HWND hStatusBar;
	RECT rc;
	int widths[] = {-1, -1};

	hStatusBar = GetDlgItem(hwnd, IDW_STATUSBAR);

	GetClientRect(hwnd, &rc);
	widths[0] = rc.right - rc.left - 50;
	SendMessage(hStatusBar, SB_SETPARTS, sizeof(widths) / sizeof(int), (LPARAM)widths);
	SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM) TEXT("Hello :)"));
	SendMessage(hStatusBar, WM_SIZE, 0, 0);

	InvalidateRect(hwnd, NULL, TRUE);
}

/**
 * The main window function.
 */
LRESULT CALLBACK
WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (message) {

	case WM_PAINT:
		BeginPaint(hwnd, &ps);
		Paint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_FILE_MESSAGEBOX:
			MessageBox(hwnd, TEXT("Testing a Message Box."), TEXT("Test"), MB_OK | MB_ICONINFORMATION);
			return 0;

		case IDM_FILE_TEST:
			MessageBox(hwnd, TEXT("No testing."), TEXT("Error"), MB_OK | MB_ICONSTOP);
			return 0;

		case IDM_FILE_EXIT:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;

		case IDM_HELP_ABOUT:
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, (DLGPROC) AboutDlgProc);
			return 0;
		}
		return 0;

	case WM_SIZE:
		Size(hwnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:
		CreateWindowEx(0, STATUSCLASSNAME, TEXT(""),
			WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
			hwnd, (HMENU) IDW_STATUSBAR, (HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
		return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

/**
 * Main program for Windows.
 */
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;

	InitCommonControls();

	// registers window class
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDM_MAINMENU);
	wc.lpszClassName = TEXT("hello");
	if (!RegisterClass(&wc))
		return 0;

	// creates window
	hwnd = CreateWindow(TEXT("hello"), TEXT("Hello"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		HWND_DESKTOP, NULL, hInstance, NULL);
	if (!hwnd)
		return 0;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// message loop
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

