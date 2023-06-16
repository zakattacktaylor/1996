#include <windows.h>

HMODULE g_hModule;

LRESULT CALLBACK WndProc(HWND, UINT, UINT, LONG);

const char* szClassName = "Skeleton";
const char* szWindowTitle = "Skeleton";

int APIENTRY WinMain(HMODULE hModule, HINSTANCE /* hNull */,
                     PSTR /* pszCmdLine */, int nCmdShow)
{
WNDCLASS wc;
MSG msg;
HWND hwnd;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hModule;
    wc.hIcon = LoadIcon(hModule, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szClassName;

    RegisterClass(&wc);

    g_hModule = hModule;

    hwnd = CreateWindow(szClassName,           // Class name
                        szWindowTitle,         // Window name
                        WS_OVERLAPPEDWINDOW,   // Style
                        CW_USEDEFAULT,         // X
                        CW_USEDEFAULT,         // Y
                        CW_USEDEFAULT,         // Width
                        CW_USEDEFAULT,         // Height
                        NULL,                  // Parent window
                        NULL,                  // Menu
                        hModule,               // Module
                        NULL);                 // User-defined data

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, UINT wParam, LONG lParam)
{
//HDC hDC;
//PAINTSTRUCT ps;

    switch(uMsg)
    {
        case WM_CREATE:
            return 0;

//        case WM_PAINT:
//            hDC = BeginPaint(hwnd, &ps);

//            EndPaint(hwnd, &ps);
//            return 0;

//        case WM_COMMAND:
//            switch(LOWORD(wParam))
//            {
//                case CM_FINDPROCESS:
//                FindIt(hwnd);
//                return 0;
//            }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
