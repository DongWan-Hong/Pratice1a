#include "Main_Menu.h"
#include <Windows.h>
#include "resource.h"


//콜백은 운도우가 이 함수를 호출 할때 사용하는 호출 규약
LRESULT CALLBACK WndProc( // 메시지를 처리하고 반환하는 값
    HWND hWnd,            // 메세지가 발생한 윈도우 핸들
    UINT message,         // 어떤 메세지를 받을지
    WPARAM wParam,
    LPARAM IParam
);


int WINAPI WinMain(
    HINSTANCE hInstance, //  현재 실행중인 실행중인 프로그램의 인스턴스
    HINSTANCE hPrevInstance, // 사용 안함
    LPSTR IpCmdLine,// 프로그램 실행 시 전달된 명령줄 문자열
    int nCmdShow) // 처음 윈도우 킬때 창 어떻게 할것인지
{

    const char* className = "BitmapTest";

    WNDCLASSA wc = {};

    wc.lpfnWndProc = WndProc; //  메세지는 어떻게 처리할래?

    wc.hInstance = hInstance; // 이 윈도우 클래스가 어느 프로그램 소속인지 지정

    wc.lpszClassName = className; // 윈도우 클래스 이름 만드는거

    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 커서는 어떻게 할지

    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 윈도우 배경 색은 어떻게 할지

    RegisterClassA(&wc);  // 작성한 윈도우 클래스 정보를 Windows에 등록

    // 이제부터 위에 설계해놓은 정보를 기반으로 실제 윈도우 생성

    HWND hWnd = CreateWindowA(

        className, // 사용할 윈도우 클래스 이름

        "Test", // 윈도우 제목 표시줄에 나올 문자열

        WS_OVERLAPPEDWINDOW, //  윈도우 스타일인데 왠만하면 이거임

        100, //  윈도우의 시작 X 좌표
        100, //  윈도우의 시작 Y 좌표
        800, //  윈도우 전체 가로 크기
        600, //  윈도우 전체 세로 크기
        NULL,// 부모 윈도우 , NULL이면 최상위 윈도우
        NULL,// 메뉴 핸들, 메뉴를 사용하지 않으므로 NUL
        hInstance,//현재 프로그램 인스턴스
        NULL // 추가로 넘겨줄 사용자 데이터


    );

    ShowWindow(hWnd, nCmdShow); //  생성된 윈도우 화면에 표시

    UpdateWindow(hWnd); // 윈도우를 즉시 다시 그리도록 요청

    //이제 메세지 루프

    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0))
    {

        TranslateMessage(&msg); //키보드 메시지 같은 걸 문자 메시지로 변환해줄 수 있음

        DispatchMessage(&msg); // 그리고 그 메세지를 WndProc로 전달



    }
}

//내가 만든 윈도우에서 발생한 각종 메세지 처리 하기
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM IParam)
{
    switch (Message)
    {

        // WM_PAINT 윈도우 화면을 다시 그리는 메세지
        // 창을 처음 보여줄 때나 다른 창에 가렸다가 다시 나타날때 발생
    case WM_PAINT:
    {
        //Paint 작업 정보 담는 구조체
        PAINTSTRUCT ps;

        // 실제 화면 DC 얻기
        // BeginPaint 호출 -> 현재 윈도우의 그림 공간 HDC 반환
        // HDC는 실제화면과 연결
        HDC hdc = BeginPaint(hWnd, &ps);  // 화면 DC 얻고

        HDC memDC = CreateCompatibleDC(hdc); // 메모리 DC 얻고



        //HBITMAP = 실제 비트맵 객체를 가리키는 핸들
        HBITMAP hBitmap = LoadBitmap(
            GetModuleHandle(NULL), // 보통 g_hInst를 넣는데 이 함수를 쓰면  
            MAKEINTRESOURCE(IDB_BITMAP1) //IDB_BITMAP1이라는 정수 리소스 ID를 
            //API에서 사용할 수 있는 형태로 바꿔줌
        );


        //BITMAP = 그 비트맵의 정보를 담는 구조체

        BITMAP bitmap = {};

        GetObject(hBitmap, // 정보를 알고 싶은 비트맵의 핸들
            sizeof(BITMAP),// 받을 구조체의 크기
            &bitmap        // 정보를 저장할 구조체 주소
        );

        //Bitmap을 메모리 DC에 선택
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
        // 아까 CreateCompatibleDC로 얻은 화면 DC인 hDc와 호환되고 있는 memDC 를 hBitmap으로 지정
        // 나중에 복구해야 해서 OldBitmap으로 받아 놓음

        BitBlt(
            hdc,// 최종 출력할 화면 DC
            0,  // 화면에서 출력할 X좌표
            0,  // 화면에서 출력할 Y좌표
            bitmap.bmWidth,  // 비트맵 이미지의 가로 크기
            bitmap.bmHeight, // 비트맵 이미지의 세로 크기
            memDC,  // 메모리 DC
            0,   // 원본 Bitmap에서 시작할 X 좌표
            0,   // 원본 Bitmap에서 시작할 Y 좌표
            SRCCOPY);

        SelectObject(memDC, oldBitmap); // 작업 끝났으니 다시 되돌리기 

        DeleteObject(hBitmap);

        DeleteDC(memDC);

        EndPaint(hWnd, &ps);

        return 0;

    }
    // 창을 닫으면
    case WM_DESTROY:

        //메세지 루프 종료
        PostQuitMessage(0);

        return 0;


    }


    return DefWindowProc(hWnd, Message, wParam, IParam);
}
