#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) //Функция обработки
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (hdc)
        {
            const int cellSize = 30;
            const int startX = 30;
            const int startY = 30;
            RECT rect;

            //цикл для создания 8x8 клеток
            for (int r = 0; r < 8; ++r)
            {
                for (int c = 0; c < 8; ++c)
                {
                    HBRUSH brush = CreateSolidBrush(((r + c) % 2 == 0) ? RGB(255, 255, 255) : RGB(0, 0, 0));

                    //координаты текущей клетки
                    rect.left = startX + c * cellSize;
                    rect.top = startY + r * cellSize;
                    rect.right = startX + (c + 1) * cellSize;
                    rect.bottom = startY + (r + 1) * cellSize;

                    FillRect(hdc, &rect, brush);
                    DeleteObject(brush);
                }
            }
            EndPaint(hwnd, &ps);
        }
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASS wc = { 0 }; 
    wc.lpfnWndProc = WndProc; //Указатель на функцию обработки сообщений
    wc.hInstance = hInstance; //Дескриптор экземпляра приложения
    wc.lpszClassName = L"ChessWindow"; //Имя класса окна (Unicode)

    RegisterClass(&wc); 

    HWND hwnd = CreateWindow(
        L"ChessWindow",
        L"Шахматная Доска", // Unicode строка
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 350, 370,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}