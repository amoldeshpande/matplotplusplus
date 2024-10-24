#include <matplot/matplot.h>
#include <matplot/backend/windows_gdiplus.h>

using namespace matplot;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

figure_handle default_figure = nullptr;

void setup(HWND hwnd) { 
    default_figure = figure<backend::windows_gdiplus>(true,hwnd);
    //default_figure = figure<backend::gnuplot>(true);


    figure(default_figure);
}
void stem_it() {

    vector_1d data = {2, 4, 6, 7, 8, 7, 5, 2};
    stem(data);
    /*auto r1 = rectangle(2.5, 5.5, 2, 2);
    r1->color("red");

    auto r2 = rectangle(6.5, 4.5, 1, 1);
    r2->fill(true);
    r2->color({0.8f, 0.f, 0.f, 1.f});*/

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR pCmdLine, int nCmdShow) {
    // Register the window class.
    const char CLASS_NAME[] = "Matplot++ Windows GDI+ example";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowExA(0,          // Optional window styles.
                               CLASS_NAME, // Window class
                               "Matplot++ Example", // Window text
                               WS_OVERLAPPEDWINDOW,         // Window style

                               // Size and position
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT,

                               NULL,      // Parent window
                               NULL,      // Menu
                               hInstance, // Instance handle
                               NULL       // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    setup(hwnd);
    stem_it();

    // Run the message loop.

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

VOID OnPaint(HWND hdc) {
    RECT rect;
    Gdiplus::Graphics graphics(hdc);
    Gdiplus::Pen pen(Gdiplus::Color(255, 255, 255, 255));
    graphics.Clear(Gdiplus::Color(255, 0xef, 0xef, 0xef));
    graphics.DrawLine(&pen, 0, 0, 200, 100);
    GetWindowRect(hdc, &rect);
    Gdiplus::REAL width = 1158 - 166.39, height = 679.87 - 80.849;
    graphics.DrawRectangle(&pen, (Gdiplus::REAL)166.39, 1158.39, width, height);
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {

    PAINTSTRUCT ps;

    switch (uMsg) {
        case WM_CREATE:

        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT: {
            HDC hdc = BeginPaint(hwnd, &ps);
            //OnPaint(hwnd);
            show();
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}