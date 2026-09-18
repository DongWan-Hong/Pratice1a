#include <Windows.h>
#include "resource.h"

HBITMAP hBit;
HBITMAP MemBit;
HWND hWndMain;
#define Radius 20
int x, y;
int xi, yi;


LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT Message,
	WPARAM wParam,
	LPARAM lParam
);



void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC hMemDC;
	HBITMAP OldBitmap;
	int Width, Height;
	BITMAP bit;

	hMemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(hMemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	Width = bit.bmWidth;
	Height = bit.bmHeight;

	BitBlt(hdc, x, y, Width, Height, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, OldBitmap);
	DeleteDC(hMemDC);
}

//가상 메모리 비트맵에 그림을 미리 그려둔다.
void Ready(HWND hWnd)
{
	int i;
	HBITMAP hOldBmp;
	HDC hdc, MemDC;
	hdc = GetDC(hWnd);
	HBRUSH MyBrush, OldBrush;
	int x, y;

	// 이미 비트맵이 만들어져 있으면 지운다
	if (MemBit)
		DeleteObject(MemBit);

	MemBit = CreateCompatibleBitmap(hdc, 800, 600);
	MemDC = CreateCompatibleDC(hdc);
	hOldBmp = (HBITMAP)SelectObject(MemDC, MemBit);

	// 메모리 비트맵을 흰색으로 채운 후 무작위로 타원 5000개를 그린다.

	PatBlt(MemDC, 0, 0, 800, 600, WHITENESS);
	for (i = 0; i < 5000; i++)
	{
		MyBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		OldBrush = (HBRUSH)SelectObject(MemDC, MyBrush);
		x = rand() % 720;
		y = rand() % 520;
		Ellipse(MemDC, x, y, x + rand() % 50 + 30, y + rand() % 50 + 30);
		SelectObject(MemDC, OldBrush);
		DeleteObject(MyBrush);
	}

	SelectObject(MemDC, hOldBmp);
	DeleteObject(MemDC);
	ReleaseDC(hWnd, hdc);
}
int WINAPI WinMain( //
	HINSTANCE hInstance, // 현재 실행중인 프로그램의 인스턴스
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, // 프로그램 실행시 전달된 명령줄 문자열
	int nCmdShow) // 처음 윈도우 킬때 창 어떻게 할래?
{
	const char* className = "BitmapTest";

	WNDCLASSA wc = {};

	wc.lpfnWndProc = WndProc; // 메세지 처리 방식

	wc.hInstance = hInstance; // 지금 만드는 윈도우는 이 프로그램의 소속이다

	wc.lpszClassName = className; // 윈도우 클래스의 이름

	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 마우스 커서 방식

	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 윈도우 배경 설정

	RegisterClassA(&wc); // 작성한 윈도우 클래스 정보를 윈도우에 등록

	HWND hWnd = CreateWindowA(wc.lpszClassName,// 사용할 윈도우 클래스 ㅇ름
		"Test", // 윈도우 제목 표시줄에 나올 문자열
		WS_OVERLAPPEDWINDOW, // 윈도우 스타일 기본 포맷
		100, //윈도우 시작 X좌표
		100, //윈도우 시작 Y좌표
		800, //윈도우 전체 가로 크기
		600, //윈도우 전체 세로 크기
		NULL,//부모 윈도우, NULL이면 최상위 윈도우
		NULL,//메뉴핸들
		wc.hInstance,// 현재 프로그램의 인스턴스
		NULL); // 추가로 넘겨줄 사용자 데이터

	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	hWndMain = hWnd;

	MSG msg = {};

	while (GetMessage(
		&msg,  // 메세지를 저장할 곳
		NULL, // 어느 윈도우의 메세지를 받을지, 지정안함
		0,    // 받을 메세지 범위의 시작
		0))   // 받을 메세지 범위의 끝
	{
		TranslateMessage(&msg); //입력받은 메세지를 문자 메세지로
		DispatchMessage(&msg); //메세지를 WndProc로 전달
	}
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SYSTEM_INFO si;
	TCHAR str[128];
	TCHAR Arch[100], Level[100];
	TCHAR KeyType[100];
	static int arFunc[] = { 0,10,12,10,12,10,24,7,8,9,10,11,12 };

	switch (iMessage)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps{};                    
		HDC hdc = BeginPaint(hWnd, &ps);      
		wchar_t text[128];                   

		int width = GetSystemMetrics(SM_CXSCREEN);   // 주 모니터 너비
		int height = GetSystemMetrics(SM_CYSCREEN); // 주 모니터 높이
		int buttons = GetSystemMetrics(SM_CMOUSEBUTTONS); // 마우스 버튼 개수
		bool swapped = GetSystemMetrics(SM_SWAPBUTTON) != 0; // 버튼 교체 여부

		wsprintfW(text, L"화면 크기: %d x %d", width, height); 
		TextOutW(hdc, 10, 10, text, lstrlenW(text));           

		wsprintfW(text, L"마우스 버튼: %d개", buttons);         
		TextOutW(hdc, 10, 40, text, lstrlenW(text));          

		const wchar_t* state = swapped ? L"교체됨" : L"교체 안 됨"; // 상태에 맞는 문자열
		wsprintfW(text, L"좌우 버튼: %s", state);               // 상태 안내 문자열
		TextOutW(hdc, 10, 70, text, lstrlenW(text));          

		EndPaint(hWnd, &ps);                  
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}


	

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}