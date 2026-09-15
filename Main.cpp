#include <Windows.h>
#include "resource.h"


LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT Message,
	WPARAM wParam,
	LPARAM lParam
	);



int WINAPI WinMain(
	HINSTANCE hInstance, // 현재 실행중인 프로그램의 인스턴스
	HINSTANCE hPrevInstance, // 사용 안함
	LPSTR ipCmdLine, // 프로그램 실행 시 전달된 명령중 문자열
	int nCmdShow) // 처음 윈도우 킬때 창 어떻게 할 것인지
{

	const char* className = "API Test"; // 만들 윈도우의 클래스 이름

	WNDCLASSA wc = {}; // 만들 윈도우의 정보를 담을 구조체

	wc.lpfnWndProc = WndProc; // 메세지 어떻게 처리할래

	wc.hInstance = hInstance; // 이 윈도우 클래스가 어느 프로그램의 소속인지

	wc.lpszClassName = className; // 클래스 이름 

	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 커서는 어떻게 할래

	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	RegisterClassA(&wc);

	HWND hWnd = CreateWindowA(
		className,
		"Test",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);

		DispatchMessage(&msg);
		
	}

}

LRESULT WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	switch (Message)
	{

	case WM_PAINT:
	{
		PAINTSTRUCT ps;


		HDC hdc = BeginPaint(hWnd, &ps); // 그리기 상태 세팅하고 hWnd에 그릴 준비

		HDC MemDC = CreateCompatibleDC(hdc); // hdc랑 호환하는 메모리 DC

		HBITMAP hBitmap = LoadBitmap( // 비트맵 로드해서 객체 만들고
			GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));

		BITMAP bitmap = {}; // 구조체 만들어

		GetObject(hBitmap, sizeof(BITMAP), &bitmap); // 비트맵 객체 정보 담을 구조체

		HBITMAP OldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap);
		// 화면에 넣을 준비 MeMDC 쓸꺼고 hBitmap 객체를 그릴꺼임

		BitBlt(hdc, 0, 0,
			bitmap.bmWidth,
			bitmap.bmHeight,
			MemDC,
			0, 0,
			SRCCOPY);

		SelectObject(MemDC, OldBitmap); // MemDC를 원래 비트맵으로 복구

		DeleteObject(hBitmap);

		DeleteDC(MemDC);

		EndPaint(hWnd, &ps);

		return 0;
	}
	case WM_DESTROY:

		PostQuitMessage(0);

		return 0;
	}


	return DefWindowProc(hWnd, Message, wParam, lParam);
}
