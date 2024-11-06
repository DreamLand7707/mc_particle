
#include "OpenGL_GUI/application.hpp"

#include <exception>

namespace universal_gui
{
    std::unordered_map<GLFWwindow *, application *> application::win2app_look_up_table;

    application::application()
        : window_(nullptr), widgets_ {}, width_(0), height_(0) {}

    application::application(int width, int height, const char *title)
        : window_(nullptr), widgets_ {}, width_(width), height_(height) {
        window_ = glfwCreateWindow(width_, height_, title, nullptr, nullptr);
        if (!window_) {
            throw std::bad_alloc {};
        }
        register_function_to_window();
    }

    void application::register_function_to_window() const {
        glfwSetWindowPosCallback(window_, pos_call_impl);
        glfwSetWindowSizeCallback(window_, size_call_impl);
        glfwSetWindowCloseCallback(window_, close_call_impl);
        glfwSetWindowRefreshCallback(window_, refresh_call_impl);
        glfwSetWindowFocusCallback(window_, focus_call_impl);
        glfwSetWindowIconifyCallback(window_, iconify_call_impl);
        glfwSetWindowMaximizeCallback(window_, maximize_call_impl);
        glfwSetFramebufferSizeCallback(window_, frameBufferSize_call_impl);
        glfwSetWindowContentScaleCallback(window_, contentScale_call_impl);
        glfwSetCursorEnterCallback(window_, cursorEnter_call_impl);
        glfwSetKeyCallback(window_, key_call_impl);
        glfwSetCharCallback(window_, char_call_impl);
        glfwSetCharModsCallback(window_, charMod_call_impl);
        glfwSetMouseButtonCallback(window_, mouseBtn_call_impl);
        glfwSetCursorPosCallback(window_, cursorPos_call_impl);
        glfwSetScrollCallback(window_, scroll_call_impl);
        glfwSetDropCallback(window_, drop_call_impl);
    }

    application::application(application &&win2mov) noexcept
        : window_(win2mov.window_),
          widgets_(std::move(win2mov.widgets_)),
          width_(win2mov.width_),
          height_(win2mov.height_) {
        win2mov.window_ = nullptr;
    }

    application &application::operator=(application &&win2mov) noexcept {
        glfwDestroyWindow(window_);
        window_ = win2mov.window_;
        widgets_ = std::move(win2mov.widgets_);
        width_ = win2mov.width_;
        height_ = win2mov.height_;
        win2mov.window_ = nullptr;
        return *this;
    }
} // namespace universal_gui


namespace universal_gui
{
    void application::pos_call_impl(GLFWwindow *window, int xpos, int ypos) {
        application *aptr = found_app(window);
        aptr->pos_call(window, xpos, ypos);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<window_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->pos_call(window, xpos, ypos);
        }
    }
    void application::size_call_impl(GLFWwindow *window, int width, int height) {
        application *aptr = found_app(window);
        aptr->size_call(window, width, height);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<window_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->size_call(window, width, height);
        }
    }
    void application::close_call_impl(GLFWwindow *window) {
        application *aptr = found_app(window);
        aptr->close_call(window);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<window_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->close_call(window);
        }
    }
    void application::refresh_call_impl(GLFWwindow *window) {
        application *aptr = found_app(window);
        aptr->refresh_call(window);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<window_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->refresh_call(window);
        }
    }
    void application::focus_call_impl(GLFWwindow *window, int focused) {
        application *aptr = found_app(window);
        aptr->focus_call(window, focused);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<window_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->focus_call(window, focused);
        }
    }
    void application::iconify_call_impl(GLFWwindow *window, int iconified) {
        application *aptr = found_app(window);
        aptr->iconify_call(window, iconified);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<window_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->iconify_call(window, iconified);
        }
    }
    void application::maximize_call_impl(GLFWwindow *window, int maximized) {
        application *aptr = found_app(window);
        aptr->maximize_call(window, maximized);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<window_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->maximize_call(window, maximized);
        }
    }
    void application::frameBufferSize_call_impl(GLFWwindow *window, int width, int height) {
        application *aptr = found_app(window);
        aptr->frameBufferSize_call(window, width, height);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<window_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->frameBufferSize_call(window, width, height);
        }
    }
    void application::contentScale_call_impl(GLFWwindow *window, float xscale, float yscale) {
        application *aptr = found_app(window);
        aptr->contentScale_call(window, xscale, yscale);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<window_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->contentScale_call(window, xscale, yscale);
        }
    }
    void application::cursorEnter_call_impl(GLFWwindow *window, int entered) {
        application *aptr = found_app(window);
        aptr->cursorEnter_call(window, entered);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<window_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->cursorEnter_call(window, entered);
        }
    }

    void application::key_call_impl(GLFWwindow *window, int key, int scancode, int action, int mods) {
        application *aptr = found_app(window);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<ordinary_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->key_call(window, key, scancode, action, mods);
        }
    }
    void application::char_call_impl(GLFWwindow *window, unsigned int codepoint) {
        application *aptr = found_app(window);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<ordinary_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->char_call(window, codepoint);
        }
    }
    void application::charMod_call_impl(GLFWwindow *window, unsigned int codepoint, int mods) {
        application *aptr = found_app(window);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<ordinary_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->charMod_call(window, codepoint, mods);
        }
    }
    void application::cursorPos_call_impl(GLFWwindow *window, double xpos, double ypos) {
        application *aptr = found_app(window);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<ordinary_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->cursorPos_call(window, xpos, ypos);
        }
    }
    void application::mouseBtn_call_impl(GLFWwindow *window, int button, int action, int mods) {
        application *aptr = found_app(window);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<ordinary_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->mouseBtn_call(window, button, action, mods);
        }
    }
    void application::scroll_call_impl(GLFWwindow *window, double xoffset, double yoffset) {
        application *aptr = found_app(window);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<ordinary_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->scroll_call(window, xoffset, yoffset);
        }
    }
    void application::drop_call_impl(GLFWwindow *window, int path_count, const char *paths[]) {
        application *aptr = found_app(window);
        std::list<widget *> &widgets = aptr->widgets_;
        for (widget *per_widget : widgets) {
            auto *fwd_widget = dynamic_cast<other_widget *>(per_widget);
            if (!fwd_widget)
                continue;
            fwd_widget->drop_call(window, path_count, paths);
        }
    }
} // namespace universal_gui
