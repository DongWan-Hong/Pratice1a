#include <Windows.h>
#include "resource.h"

// 하나의 윈도우를 관리하는 C++ 클래스
class BitmapWindow
{
private:

    // 이 클래스가 관리하는 실제 윈도우의 핸들
    HWND m_hWnd = nullptr;

    // 현재 프로그램의 인스턴스 핸들
    HINSTANCE m_hInstance = nullptr;

    // 출력할 비트맵 핸들
    HBITMAP m_hBitmap = nullptr;


public:

    // --------------------------------------------------------
    // 생성자
    // --------------------------------------------------------
    // BitmapWindow 객체를 만들 때
    // 프로그램의 인스턴스 핸들을 전달받음
    BitmapWindow(HINSTANCE hInstance)
        : m_hInstance(hInstance)
    {
    }


    // --------------------------------------------------------
    // 소멸자
    // --------------------------------------------------------
    // 객체가 사라질 때 가지고 있던 비트맵이 있다면 제거
    ~BitmapWindow()
    {
        if (m_hBitmap != nullptr)
        {
            DeleteObject(m_hBitmap);
            m_hBitmap = nullptr;
        }
    }


    // --------------------------------------------------------
    // 윈도우 생성
    // --------------------------------------------------------
    bool Create()
    {
        const char* className = "BitmapTest";


        // 윈도우 클래스 정보
        WNDCLASSA wc = {};

        wc.lpfnWndProc = WindowProc;
        wc.hInstance = m_hInstance;
        wc.lpszClassName = className;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground =
            reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);


        // 윈도우 클래스 등록
        if (RegisterClassA(&wc) == 0)
        {
            return false;
        }


        // 실제 윈도우 생성
        m_hWnd = CreateWindowA(
            className,
            "Bitmap Test",
            WS_OVERLAPPEDWINDOW,

            100,
            100,
            800,
            600,

            nullptr,
            nullptr,
            m_hInstance,

            // 중요
            // 생성되는 윈도우에게
            // "이 BitmapWindow 객체가 주인이다"
            // 라는 정보를 전달
            this
        );


        if (m_hWnd == nullptr)
        {
            return false;
        }


        // 비트맵 리소스는 한 번만 로드
        m_hBitmap = LoadBitmap(
            m_hInstance,
            MAKEINTRESOURCE(IDB_BITMAP1)
        );


        if (m_hBitmap == nullptr)
        {
            return false;
        }


        return true;
    }


    // --------------------------------------------------------
    // 윈도우 화면에 표시
    // --------------------------------------------------------
    void Show(int nCmdShow)
    {
        ShowWindow(m_hWnd, nCmdShow);
        UpdateWindow(m_hWnd);
    }


    // --------------------------------------------------------
    // 메시지 루프
    // --------------------------------------------------------
    int Run()
    {
        MSG msg = {};

        while (GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return static_cast<int>(msg.wParam);
    }


private:

    // ========================================================
    // 실제 WM_PAINT 처리 함수
    // ========================================================
    void OnPaint()
    {
        PAINTSTRUCT ps = {};


        // ----------------------------------------------------
        // 실제 화면 DC 획득
        // ----------------------------------------------------
        HDC hdc = BeginPaint(m_hWnd, &ps);


        // ----------------------------------------------------
        // 화면 DC와 호환되는 메모리 DC 생성
        // ----------------------------------------------------
        HDC memDC = CreateCompatibleDC(hdc);


        // ----------------------------------------------------
        // 비트맵 정보 획득
        // ----------------------------------------------------
        BITMAP bitmap = {};

        GetObject(
            m_hBitmap,
            sizeof(BITMAP),
            &bitmap
        );


        // ----------------------------------------------------
        // 우리가 가진 비트맵을 메모리 DC에 선택
        //
        // 동시에 원래 MemDC에 선택되어 있던
        // 비트맵을 oldBitmap에 저장
        // ----------------------------------------------------
        HBITMAP oldBitmap =
            static_cast<HBITMAP>(
                SelectObject(
                    memDC,
                    m_hBitmap
                )
                );


        // ----------------------------------------------------
        // 메모리 DC의 비트맵을 실제 화면으로 복사
        // ----------------------------------------------------
        BitBlt(
            hdc,

            // 목적지 위치
            0,
            0,

            // 비트맵 원본 크기
            bitmap.bmWidth,
            bitmap.bmHeight,

            // 원본 DC
            memDC,

            // 원본 시작 위치
            0,
            0,

            // 그대로 복사
            SRCCOPY
        );


        // ----------------------------------------------------
        // MemDC 원래 상태 복구
        // ----------------------------------------------------
        SelectObject(
            memDC,
            oldBitmap
        );


        // ----------------------------------------------------
        // CreateCompatibleDC로 만든 DC 제거
        // ----------------------------------------------------
        DeleteDC(memDC);


        // ----------------------------------------------------
        // WM_PAINT 작업 종료
        // ----------------------------------------------------
        EndPaint(m_hWnd, &ps);
    }


    // ========================================================
    // 일반 메시지를 처리하는 클래스 멤버 함수
    // ========================================================
    LRESULT HandleMessage(
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    )
    {
        switch (message)
        {
        case WM_PAINT:
        {
            OnPaint();
            return 0;
        }


        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        }


        return DefWindowProc(
            m_hWnd,
            message,
            wParam,
            lParam
        );
    }


    // ========================================================
    // Windows가 직접 호출하는 윈도우 프로시저
    //
    // 일반 멤버 함수는 그대로 WndProc로 등록할 수 없기 때문에
    // static 함수가 중간에서 연결 역할을 함
    // ========================================================
    static LRESULT CALLBACK WindowProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    )
    {
        BitmapWindow* window = nullptr;


        // ----------------------------------------------------
        // 윈도우가 처음 생성될 때
        // ----------------------------------------------------
        if (message == WM_NCCREATE)
        {
            // CreateWindow에서 마지막 인수로 넘긴
            // this 포인터를 꺼냄
            CREATESTRUCT* createStruct =
                reinterpret_cast<CREATESTRUCT*>(lParam);


            window =
                static_cast<BitmapWindow*>(
                    createStruct->lpCreateParams
                    );


            // 실제 HWND를 객체에 저장
            window->m_hWnd = hWnd;


            // HWND 내부에 이 객체의 주소를 저장
            SetWindowLongPtr(
                hWnd,
                GWLP_USERDATA,
                reinterpret_cast<LONG_PTR>(window)
            );
        }
        else
        {
            // 이전에 저장해놓은 객체 주소를 다시 가져옴
            window =
                reinterpret_cast<BitmapWindow*>(
                    GetWindowLongPtr(
                        hWnd,
                        GWLP_USERDATA
                    )
                    );
        }


        // BitmapWindow 객체를 찾았다면
        // 실제 메시지 처리는 객체에게 맡김
        if (window != nullptr)
        {
            return window->HandleMessage(
                message,
                wParam,
                lParam
            );
        }


        // 아직 객체와 연결되기 전의 메시지는
        // Windows 기본 처리
        return DefWindowProc(
            hWnd,
            message,
            wParam,
            lParam
        );
    }
};


// ============================================================
// 프로그램 시작점
// ============================================================
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    // --------------------------------------------------------
    // 우리가 만든 Window 클래스 객체 생성
    // --------------------------------------------------------
    BitmapWindow window(hInstance);


    // --------------------------------------------------------
    // 실제 Win32 윈도우 생성
    // --------------------------------------------------------
    if (!window.Create())
    {
        return -1;
    }


    // --------------------------------------------------------
    // 화면에 표시
    // --------------------------------------------------------
    window.Show(nCmdShow);


    // --------------------------------------------------------
    // 메시지 루프 실행
    // --------------------------------------------------------
    return window.Run();
}

// ignore 적용