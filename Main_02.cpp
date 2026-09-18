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

//LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) // 가상의 MemDC에 그리기
//{
//	HDC hdc;
//	PAINTSTRUCT ps;
//	switch (Message)
//	{
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		DrawBitmap(hdc, 0, 0, MemBit);
//		EndPaint(hWnd, &ps);
//		return 0;
//	case WM_LBUTTONDOWN:
//		Ready(hWnd);
//		InvalidateRect(hWnd, NULL, FALSE);
//		return 0;
//	case WM_DESTROY:
//		if (MemBit)
//			DeleteObject(MemBit);
//		PostQuitMessage(0);
//		return 0;
//	}
//
//
//
//	return DefWindowProc(hWnd, Message, wParam, lParam);
//}

//LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) // 메모리  DC에 그려서 hdc에 출력하기
//{
//	HDC hdc;
//	PAINTSTRUCT ps;
//	int i;
//	HBRUSH Mybrush, OldBrush;
//	int x, y;
//	switch (Message)
//	{
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		for (i = 0; i < 5000; i++)
//		{
//			Mybrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
//			OldBrush = (HBRUSH)SelectObject(hdc, Mybrush);
//			x = rand() % 720;
//			y = rand() % 520;
//			Ellipse(hdc, x, y, x + rand() % 50 + 30, y + rand() % 50 + 30);
//			SelectObject(hdc, OldBrush);
//			DeleteObject(Mybrush);
//		}
//		EndPaint(hWnd, &ps);
//		return 0;
//	case WM_LBUTTONDOWN:
//		InvalidateRect(hWnd, NULL, FALSE);
//		return 0;
//
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	}
//
//
//	return DefWindowProc(hWnd, Message, wParam, lParam);
//}

void OnTimer()
{
	RECT crt;                // 렉트 그리고
	HDC hdc, hMemDC;         // 화면 DC랑 메모리 DC 만들고
	HBITMAP OldBit;          // 비트맵 임시 저장 핸들 만들고
	HPEN hPen, OldPen;       // 펜이랑 임시 저장 펜 만들고
	HBRUSH hBrush, OldBrush; // 브러쉬랑 임시 저장 브러쉬 만들고 
	int i;

	GetClientRect(hWndMain, &crt);     // 윈도우의 창크기를 Rect에 동기화 
	hdc = GetDC(hWndMain);

	if (hBit == NULL)
	{
		hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	}
	hMemDC = CreateCompatibleDC(hdc);
	OldBit = (HBITMAP)SelectObject(hMemDC, hBit);

	FillRect(hMemDC, &crt, GetSysColorBrush(COLOR_WINDOW)); //사각형특정 브러시 색으로 꽉 채우는 함수

	if (x <= Radius || x >= crt.right - Radius)
	{
		xi *= -1;
	}
	if (y <= Radius || y >= crt.bottom - Radius)
	{
		yi *= -1;
	    
	}

	x += xi;
	y += yi;

	for (i = 0; i < crt.right; i += 10)
	{
		MoveToEx(hMemDC, i, 0, NULL);
		LineTo(hMemDC, i, crt.bottom);
	}

	for (i = 0; i < crt.bottom; i += 10)
	{
		MoveToEx(hMemDC, 0, i, NULL);
		LineTo(hMemDC, crt.right, i);
	}

	hPen = CreatePen(PS_INSIDEFRAME, 5, RGB(255, 0, 0));
	OldPen = (HPEN)SelectObject(hMemDC, hPen);
	hBrush = CreateSolidBrush(RGB(0, 0, 255));
	OldBrush = (HBRUSH)SelectObject(hMemDC, hBrush);
	Ellipse(hMemDC, x - Radius, y - Radius, x + Radius, y + Radius);
	DeleteObject(SelectObject(hMemDC, OldPen));
	DeleteObject(SelectObject(hMemDC, OldBrush));

	SelectObject(hMemDC, OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);
	InvalidateRect(hWndMain, NULL, FALSE);

}


//LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
//{
//	HDC hdc;
//	PAINTSTRUCT ps;
//	HPEN hPen, OldPen;
//	HBRUSH hBrush, OldBrush;
//	RECT crt;
//	int i;
//
//	switch (iMessage)
//	{
//	case WM_CREATE:
//		x = 50;// 공의 현재  x 좌표
//		y = 50;// 공의 현재  y 좌표
//		xi = 4;// 공의 수평 이동
//		yi = 5;// 공의 수직 이동
//		SetTimer(hWnd, 1, 25, NULL);// 타이머 설치
//		return 0;
//
//	case WM_TIMER:
//		OnTimer();
//		return 0;
//
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		GetClientRect(hWnd, &crt);
//		for (i = 0; i < crt.right; i += 10)
//		{
//			MoveToEx(hdc, i, 0, NULL);
//			LineTo(hdc,i,crt.bottom);
//		}
//
//		for (i = 0; i < crt.bottom; i += 10)
//		{
//			MoveToEx(hdc, 0, i, NULL);
//			LineTo(hdc, crt.right, i);
//		}
//
//		hPen = CreatePen(PS_INSIDEFRAME, 5, RGB(255, 0, 0));
//		OldPen = (HPEN)SelectObject(hdc, hPen);
//		hBrush = CreateSolidBrush(RGB(0, 0, 255));
//		OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
//		Ellipse(hdc, x - Radius, y - Radius, x + Radius, y + Radius);
//		DeleteObject(SelectObject(hdc, OldPen));
//		DeleteObject(SelectObject(hdc, OldBrush));
//		EndPaint(hWnd, &ps);
//		return 0;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		KillTimer(hWnd, 1);
//		return 0;
//	}
//
//
//
//
//	return DefWindowProc(hWnd, iMessage, wParam, lParam);
//}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;                           
	HDC hdc;

	HRGN hRgn1;
	TCHAR str[128];

	switch (iMessage)
	{
	case WM_CREATE:
		hRgn1 = CreateEllipticRgn(30, 30, 200, 100);
		lstrcpy(str,TEXT( "타원을 누르십시오"));

	}



	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}