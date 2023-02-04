#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("closewithmiddlebutton");

LRESULT CALLBACK LowLevelMouseProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	if (wParam == WM_MBUTTONUP) {
		POINT point = {};
		GetCursorPos(&point);
		HWND hWnd = WindowFromPoint(point);
		DWORD dwHitTest = (DWORD)SendMessage(hWnd, WM_NCHITTEST, 0, MAKELPARAM(point.x, point.y));
		if (hWnd &&
			(
				HTCAPTION == dwHitTest ||
				HTSYSMENU == dwHitTest ||
				HTMINBUTTON == dwHitTest ||
				HTMAXBUTTON == dwHitTest ||
				HTCLOSE == dwHitTest ||
				HTHELP == dwHitTest
			))
		{
			if(!(GetWindowLongW(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)) {
				hWnd = GetAncestor(hWnd, GA_ROOT);
			}
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK WndProc(
	_In_ HWND hWnd,
	_In_ UINT msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	static HHOOK hook;
	switch (msg)
	{
	case WM_CREATE:
		hook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
		break;
	case WM_DESTROY:
		UnhookWindowsHookEx(hook);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
) {
	HWND hWnd = FindWindow(szClassName, 0);
	if (hWnd) {
		return 0;
	}
	MSG msg = {};
	WNDCLASS wndclass = {
		0,
		WndProc,
		0,
		0,
		hInstance,
		0,
		0,
		0,
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	hWnd = CreateWindow(
		szClassName,
		0,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		0,
		0,
		0,
		0,
		hInstance,
		0
	);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
