#include <Windows.h>
#include "resource.h"
#include <math.h>


HWND hWndMain;

LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT Message,
	WPARAM wParam,
	LPARAM lParam
);



int WINAPI WinMain( //
	HINSTANCE hInstance, // 현재 실행중인 프로그램의 인스턴스
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, // 프로그램 실행시 전달된 명령줄 문자열
	int nCmdShow) // 처음 윈도우 킬때 창 어떻게 할래?
{
	const TCHAR* className = TEXT("BitmapTest");

	WNDCLASSW wc = {};

	wc.lpfnWndProc = WndProc; // 메세지 처리 방식

	wc.hInstance = hInstance; // 지금 만드는 윈도우는 이 프로그램의 소속이다

	wc.lpszClassName = className; // 윈도우 클래스의 이름

	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 마우스 커서 방식

	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 윈도우 배경 설정

	RegisterClassW(&wc); // 작성한 윈도우 클래스 정보를 윈도우에 등록

	HWND hWnd = CreateWindowW(wc.lpszClassName,// 사용할 윈도우 클래스 ㅇ름
		TEXT("Test"), // 윈도우 제목 표시줄에 나올 문자열
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
	POINT ar[] = { 25,10,125,10,140,30,10,30,25,10 };
	HBRUSH BrR, BrB, BrY, OldBr;
	RECT rect;
	

	switch (iMessage)
	{
	case WM_SIZE:
			InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps); // 화면에 다시 그려야 할 때 사용하는 DC를 얻음

		BrR = CreateSolidBrush(RGB(255, 0, 0));     // 빨간 브러시 생성
		BrB = CreateSolidBrush(RGB(0, 0, 255));     // 파란 브러시 생성
		BrY = CreateSolidBrush(RGB(255, 255, 0));   // 노란 브러시 생성

		// 논리 좌표와 화면 좌표의 비율을 직접 지정할 수 있는 맵핑 모드
		SetMapMode(hdc, MM_ANISOTROPIC);


		SetWindowExtEx(hdc, 100, 100, NULL);

		// 현재 창의 실제 클라이언트 영역 크기를 가져옴.
		GetClientRect(hWnd, &rect);


		SetViewportExtEx(hdc, rect.right, rect.bottom, NULL);



		OldBr = (HBRUSH)SelectObject(hdc, BrR); // 빨간 브러시 선택, 이전 브러시 백업

		Rectangle(hdc, 20, 30, 130, 90);

		SelectObject(hdc, BrB); // 파란 브러시 선택

		Polygon(hdc, ar, 5);

		SelectObject(hdc, BrY); 

		
		Rectangle(hdc, 30, 40, 60, 70);
		Rectangle(hdc, 90, 40, 120, 70);
		Ellipse(hdc, 135, 5, 155, 25);


		SelectObject(hdc, OldBr);
		DeleteObject(BrR);
		DeleteObject(BrB);
		DeleteObject(BrY);

		EndPaint(hWnd, &ps); // 그리기 종료
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}