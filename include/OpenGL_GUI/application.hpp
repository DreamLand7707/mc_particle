#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <unordered_map>
#include <list>
#include <utility>

#include "widget.hpp"

namespace universal_gui
{
    class application : public window_widget {
     private:
        static std::unordered_map<GLFWwindow *, application *> win2app_look_up_table;

     private:
        GLFWwindow *window_;
        std::list<widget *> widgets_;
        int width_;
        int height_;

     public:
        static application *found_app(GLFWwindow *win) { return win2app_look_up_table.at(win); }
        // window
        static void pos_call_impl(GLFWwindow *window, int xpos, int ypos);
        static void size_call_impl(GLFWwindow *window, int width, int height);
        static void close_call_impl(GLFWwindow *window);
        static void refresh_call_impl(GLFWwindow *window);
        static void focus_call_impl(GLFWwindow *window, int focused);
        static void iconify_call_impl(GLFWwindow *window, int iconified);
        static void maximize_call_impl(GLFWwindow *window, int maximized);
        static void frameBufferSize_call_impl(GLFWwindow *window, int width, int height);
        static void contentScale_call_impl(GLFWwindow *window, float xscale, float yscale);
        static void cursorEnter_call_impl(GLFWwindow *window, int entered);
        // for widgets
        static void key_call_impl(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void char_call_impl(GLFWwindow *window, unsigned int codepoint);
        static void charMod_call_impl(GLFWwindow *window, unsigned int codepoint, int mods);
        static void cursorPos_call_impl(GLFWwindow *window, double xpos, double ypos);
        static void mouseBtn_call_impl(GLFWwindow *window, int button, int action, int mods);
        static void scroll_call_impl(GLFWwindow *window, double xoffset, double yoffset);

        static void drop_call_impl(GLFWwindow *window, int path_count, const char *paths[]);

        application();
        application(int width, int height, const char *title);
        application(const application &) = delete;
        application(application &&) noexcept;

        application &operator=(const application &) = delete;
        application &operator=(application &&) noexcept;

        ~application() override;

        void add_widget(widget &sub_widget) { widgets_.push_back(&sub_widget); }

        void pos_call(GLFWwindow *window, int xpos, int ypos) override;
        void size_call(GLFWwindow *window, int width, int height) override;
        void close_call(GLFWwindow *window) override;
        void refresh_call(GLFWwindow *window) override;
        void focus_call(GLFWwindow *window, int focused) override;
        void iconify_call(GLFWwindow *window, int iconified) override;
        void maximize_call(GLFWwindow *window, int maximized) override;
        void frameBufferSize_call(GLFWwindow *window, int width, int height) override;
        void contentScale_call(GLFWwindow *window, float xscale, float yscale) override;
        void cursorEnter_call(GLFWwindow *window, int entered) override;

        int execute();

     private:
        void register_function_to_window() const;
    };

} // namespace universal_gui

#undef GLFW_INCLUDE_NONE
