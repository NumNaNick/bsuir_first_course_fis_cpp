#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = _T("ChessBoard");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, _T("This program requires Windows NT!"),
            szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName,
        _T("Шахматная доска"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static int cxClient, cyClient;

    switch (message)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        // Создаем TCanvas
        HBRUSH hBrushWhite = CreateSolidBrush(RGB(255, 255, 255)); // Белый цвет
        HBRUSH hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));       // Черный цвет
        HBRUSH hBrushBrown = CreateSolidBrush(RGB(139, 69, 19));   // Коричневый для альтернативы

        // Размеры доски
        int boardSize = min(cxClient, cyClient) * 0.8; // 80% от минимального размера окна
        int cellSize = boardSize / 8;

        // Начальные координаты для центрирования доски
        int startX = (cxClient - boardSize) / 2;
        int startY = (cyClient - boardSize) / 2;

        // Рисуем шахматную доску
        for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 8; col++)
            {
                // Выбираем цвет клетки
                HBRUSH currentBrush;
                if ((row + col) % 2 == 0)
                {
                    currentBrush = hBrushWhite; // Белые клетки
                }
                else
                {
                    currentBrush = hBrushBlack; // Черные клетки
                }

                // Рисуем клетку
                RECT cellRect;
                cellRect.left = startX + col * cellSize;
                cellRect.top = startY + row * cellSize;
                cellRect.right = cellRect.left + cellSize;
                cellRect.bottom = cellRect.top + cellSize;

                FillRect(hdc, &cellRect, currentBrush);

                // Рисуем границу клетки
                Rectangle(hdc, cellRect.left, cellRect.top, cellRect.right, cellRect.bottom);
            }
        }

        // Добавляем подпись
        HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));

        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
        SetTextColor(hdc, RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);

        RECT textRect;
        textRect.left = startX;
        textRect.top = startY + boardSize + 10;
        textRect.right = startX + boardSize;
        textRect.bottom = textRect.top + 30;

        DrawText(hdc, _T("Шахматная доска 8x8"), -1, &textRect, DT_CENTER);

        // Восстанавливаем старый шрифт и удаляем созданные объекты
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
        DeleteObject(hBrushWhite);
        DeleteObject(hBrushBlack);
        DeleteObject(hBrushBrown);

        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}