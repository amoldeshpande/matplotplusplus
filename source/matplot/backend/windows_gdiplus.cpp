#include "windows_gdiplus.h"
#include <array>
#include <stdexcept>

using namespace Gdiplus;

namespace matplot::backend {

    windows_gdiplus::windows_gdiplus(HWND hwnd) : window_handle_(hwnd) {
        Gdiplus::GdiplusStartupInput startup_input;

        GdiplusStartup(&gdiplus_token_,&startup_input,NULL);
    }
    windows_gdiplus::~windows_gdiplus() {
        GdiplusShutdown(gdiplus_token_);
    }

    unsigned int windows_gdiplus::width() {
        RECT rect;
        if (GetWindowRect(window_handle_, &rect)) {
            return rect.right - rect.left;
        }
        return 0;
    }
    unsigned int windows_gdiplus::height() {
        RECT rect;
        if (GetWindowRect(window_handle_, &rect)) {
            return rect.top - rect.bottom;
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
       return true; }
    void windows_gdiplus::show(matplot::figure_type *) {}

    bool windows_gdiplus::should_close() {
        return false; 
    }
    void windows_gdiplus::draw_background(const std::array<float, 4> &color) {
       
        Graphics graphics(window_handle_);
        graphics.Clear(Color(floats_to_argb(color)));

    }
    void windows_gdiplus::draw_rectangle(const double x1, const double x2,
                                         const double y1, const double y2,
                                         const std::array<float, 4> &color) {
        Graphics graphics(window_handle_);
        Pen pen(Color(floats_to_argb(color)));
        graphics.DrawRectangle(&pen, (REAL)x1, y1, x2 - x1, y2 - y1);
    }

    void windows_gdiplus::draw_path(const std::vector<double> &x,
                                    const std::vector<double> &y,
                                    const std::array<float, 4> &color) {

        throw std::logic_error("not implemented");        
    }

    ARGB windows_gdiplus::floats_to_argb(const std::array<float, 4> &color) {

        //alpha
        ARGB argb = (BYTE)color[0] * 255;
        argb <<= 8;
        // red
        argb |= (BYTE)color[1] * 255;
        argb <<= 8;
        //green
        argb |= (BYTE)color[2] * 255;
        argb <<= 8;
        //blue
        argb |= (BYTE)color[3] * 255;

        return argb;
    }

} // namespace matplot::backend