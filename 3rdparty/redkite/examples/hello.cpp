#include "RkMain.h"
#include "RkWidget.h"
#include "RkLog.h"

#include <windows.h>

/*int main(int arc, char **argv)
{
    RK_LOG_INFO("called");
    RkMain app(arc, argv);

    auto widget = new RkWidget(&app);
    widget->setTitle("Hello!");
	widget->setBackgroundColor({80, 80, 80});
    widget->show();

    int res = app.exec();
    return res;
    }*/

// Function to handle window messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CLOSE:
            PostQuitMessage(0); // Exit the application when the window is closed
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register a window class
    WNDCLASSEXW wc = {0}; // Use WNDCLASSEXW for wide character strings
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"MyWindowClass"; // Use L-prefixed wide character string

    if (!RegisterClassExW(&wc)) // Use RegisterClassExW for wide character strings
    {
        MessageBoxW(NULL, L"Window Registration Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create the window
    HWND hwnd = CreateWindowExW(
        WS_EX_CLIENTEDGE,             // Extended window style
        L"MyWindowClass",             // Class name
        L"MinGW WinAPI Example",      // Window title
        WS_OVERLAPPEDWINDOW,          // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, // X and Y position
        400, 300,                     // Width and height
        NULL,                         // Parent window
        NULL,                         // Menu
        hInstance,                    // Instance handle
        NULL                          // Additional application data
    );

    if (hwnd == NULL)
    {
        MessageBoxW(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Show and update the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}



