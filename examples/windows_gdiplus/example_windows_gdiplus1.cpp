#include <matplot/matplot.h>
#include <matplot/backend/windows_gdiplus.h>

using namespace matplot;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

figure_handle default_figure = nullptr;

void setup(HWND hwnd) { 
    default_figure = figure<backend::windows_gdiplus>(true,hwnd);

    figure(default_figure);
}
void stem_it() {
    std::vector<double> x = linspace(0, 2 * pi, 50);

    std::vector<std::vector<double>> Y(2);
    Y[0] = transform(x, [](auto x) { return cos(x); });
    Y[1] = transform(x, [](auto x) { return 0.5 * sin(x); });

    stem(Y, "-o");

    show();
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
            default_figure->draw();
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}