#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <matplot/backend/backend_interface.h>

namespace matplot::backend {
    class MATPLOT_EXPORTS windows_gdiplus : public backend_interface {
      public:
        windows_gdiplus() = delete;
        explicit windows_gdiplus(HWND hwnd);
        ~windows_gdiplus();

        virtual bool is_interactive() override { return false; }
        virtual unsigned int width() override;
        virtual unsigned int height() override;
        virtual void width(unsigned int);
        virtual void height(unsigned int);

        virtual unsigned int position_x() override;
        virtual unsigned int position_y() override;

        virtual void window_title(const std::string &title) override;
        virtual std::string window_title() override;

        virtual bool new_frame() override;
        virtual bool render_data() override;

        virtual void show(matplot::figure_type *) override;
        virtual bool should_close() override;
        virtual bool supports_fonts() override { return true; }
        virtual void draw_background(const std::array<float, 4> &color) override;
        virtual void draw_rectangle(const double x1, const double x2,
                                    const double y1, const double y2,
                                    const std::array<float, 4> &color) override;
        virtual void draw_path(const std::vector<double> &x,
                               const std::vector<double> &y,
                               const std::array<float, 4> &color) override;
        virtual void draw_markers(const std::vector<double> &x,
                                  const std::vector<double> &y,
                                  const std::vector<double> &z = {}) override;
        virtual void draw_text(const std::vector<double> &x,
                               const std::vector<double> &y,
                               const std::vector<double> &z = {}) override;
        virtual void draw_image(const std::vector<std::vector<double>> &x,
                                const std::vector<std::vector<double>> &y,
                                const std::vector<std::vector<double>> &z = {}) override;
        virtual void draw_triangle(const std::vector<double> &x,
                                   const std::vector<double> &y,
                                   const std::vector<double> &z = {}) override;
       private:

        HWND window_handle_;

        ULONG_PTR gdiplus_token_;
        Gdiplus::ARGB floats_to_argb(const std::array<float, 4> &color);
    };
}