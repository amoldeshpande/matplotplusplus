#include "windows_gdiplus.h"
#include <matplot/core/figure_type.h>
#include <array>
#include <stdexcept>

using namespace Gdiplus;

void dprintf(char *format, ...) { /* } */
    va_list vl;
    char putbuf[2048];
    DWORD err;

    err = GetLastError();
    {
        va_start(vl, format);
#pragma warning(disable : 4995)
        wvsprintf(putbuf, format, vl);
#pragma warning(default : 4995)
        va_end(vl);
        OutputDebugString(putbuf);
    }
    SetLastError(err);
}
namespace matplot::backend {

    windows_gdiplus::windows_gdiplus(HWND hwnd) : window_handle_(hwnd) {
        GdiplusStartupInput startup_input;

        GdiplusStartup(&gdiplus_token_,&startup_input,NULL);
    }
    windows_gdiplus::~windows_gdiplus() {
        GdiplusShutdown(gdiplus_token_);
    }

    unsigned int windows_gdiplus::width() {
        RECT rect;
        if (GetWindowRect(window_handle_, &rect)) {
            dprintf("Returning width %d\n", rect.right - rect.left);
            return rect.right - rect.left;
        }
        return 0;
    }
    unsigned int windows_gdiplus::height() {
        RECT rect;
        if (GetWindowRect(window_handle_, &rect)) {
            dprintf("Returning height %d\n", rect.bottom - rect.top);
            return rect.bottom - rect.top;
        }
        return 0;
    }
    void windows_gdiplus::width(unsigned int width) {
        RECT rect;
        if (GetWindowRect(window_handle_, &rect)) {
            if (!SetWindowPos(window_handle_, NULL, 0, 0, width,
                             rect.top - rect.bottom, SWP_NOMOVE)) {
                ; // TODO log ?
            }
        }
    }
    void windows_gdiplus::height(unsigned int height) {
        RECT rect;
        if (GetWindowRect(window_handle_, &rect)) {
            if (!SetWindowPos(window_handle_, NULL, 0, 0, rect.right - rect.left,
                             height, SWP_NOMOVE)) {
                ; // TODO log ?
            }
        }
    }
    unsigned int windows_gdiplus::position_x() {
        RECT rect;
        if (GetWindowRect(window_handle_, &rect)) {
            return rect.left;
        }
        return 0;
    }
    unsigned int windows_gdiplus::position_y() {
        RECT rect;
        if (GetWindowRect(window_handle_, &rect)) {
            return rect.top;
        }
        return 0;
    }

    void windows_gdiplus::window_title(const std::string &title) {
        SetWindowTextA(window_handle_, title.c_str());
    }
    std::string windows_gdiplus::window_title() {
        int len = GetWindowTextLengthA(window_handle_);
        std::string return_value;
        char *title = new char[len + 1];

        if (GetWindowTextA(window_handle_, title, title ? len + 1 : 0)) {
            return_value = title;
        }
        delete[] title;
        return return_value;
    }
    bool windows_gdiplus::new_frame() { 
        return true;    
    }
    bool windows_gdiplus::render_data() { 
       return true; 
    }
    void windows_gdiplus::show(matplot::figure_type *f) {
        f->draw();
    }

    bool windows_gdiplus::should_close() {
        return false; 
    }
    void windows_gdiplus::draw_background(const std::array<float, 4> &color) {
        Status status;
        Graphics graphics(window_handle_);
        status = graphics.Clear(floats_to_color(color));
    }
    void windows_gdiplus::draw_rectangle(const double x1, const double x2,
                                         const double y1, const double y2,
                                         const std::array<float, 4> &color) {
        Graphics graphics(window_handle_);
        Pen pen(floats_to_color(color));
        transform_coordinates(graphics);
        graphics.DrawRectangle(&pen, (REAL)x1, y1, x2 - x1, y2 - y1);
    }

    void windows_gdiplus::draw_path(const std::vector<double> &x,
                                    const std::vector<double> &y,
                                    const std::array<float, 4> &color) {
        GraphicsPath path;
        Graphics graphics(window_handle_);
        Pen pen(floats_to_color(color));
        Status status;      
        std::vector<Point> points;


        dprintf("draw path\n");

        for (size_t si = 0; si < y.size(); si++) {
            INT xsi = x.empty() ? 0 : (INT)x[si];
            dprintf("\tX %d, Y %d\n", xsi,(INT) y[si]);
            points.emplace_back(xsi, (INT)y[si]);
        }
        status = path.AddLines(points.data(), points.size());

        transform_coordinates(graphics);
        status = graphics.DrawPath(&pen, &path);

    }

    void windows_gdiplus::draw_markers(const std::vector<double> &x,
                                       const std::vector<double> &y,
                                       const std::vector<double> &z) {
        throw std::logic_error("not implemented");        
    }

    void windows_gdiplus::draw_text(const std::vector<double> &x,
                                    const std::vector<double> &y,
                                    const std::vector<double> &z) {
        throw std::logic_error("not implemented");        
    }

    void
    windows_gdiplus::draw_image(const std::vector<std::vector<double>> &x,
                                const std::vector<std::vector<double>> &y,
                                const std::vector<std::vector<double>> &z) {
        throw std::logic_error("not implemented");        
    }

    void windows_gdiplus::draw_triangle(const std::vector<double> &x,
                                        const std::vector<double> &y,
                                        const std::vector<double> &z) {
        throw std::logic_error("not implemented");
    }
    Color windows_gdiplus::floats_to_color(const std::array<float, 4> &color) {

        Color gdi_color((1.0 - color[0])*255,color[1] * 255, color[2] * 255, color[3] * 255);

        return gdi_color;
    }

    void windows_gdiplus::transform_coordinates( Graphics &graphics) {
        RECT rect;
        Status status;
        if (GetWindowRect(window_handle_, &rect)) {
            dprintf("transforming bottom by %d\n", rect.bottom-rect.top);
            status = graphics.TranslateTransform(0, rect.bottom- rect.top);
            status = graphics.ScaleTransform(1.0, -1.0);
        }
    }

} // namespace matplot::backend