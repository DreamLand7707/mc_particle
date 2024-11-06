#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <unordered_map>
#include <list>
#include <utility>
namespace universal_gui
{
    class widget {
     public:
        enum class widget_type : unsigned int {
            ORDINARAY = 1,
            SYSTEM = 2,
            WINDOW = 4,
            OTHER = 8
        };

        virtual ~widget() = 0;
        virtual widget_type widget_class() = 0;
    };

    class ordinary_widget : virtual public widget {
     public:
        virtual void key_call(GLFWwindow *window, int key, int scancode, int action, int mods) = 0;
        virtual void char_call(GLFWwindow *window, unsigned int codepoint) = 0;
        virtual void charMod_call(GLFWwindow *window, unsigned int codepoint, int mods) = 0;
        virtual void cursorPos_call(GLFWwindow *window, double xpos, double ypos) = 0;
        virtual void mouseBtn_call(GLFWwindow *window, int button, int action, int mods) = 0;
        virtual void scroll_call(GLFWwindow *window, double xoffset, double yoffset) = 0;

        widget_type widget_class() override { return widget_type::ORDINARAY; }
    };

    class system_widget : virtual public widget {
     public:
        virtual void error_call(int error_code, const char *description) = 0;
        virtual void monitor_call(GLFWmonitor *monitor, int event) = 0;
        virtual void joystick_call(int jid, int event) = 0;

        widget_type widget_class() override { return widget_type::SYSTEM; }
    };

    class window_widget : virtual public widget {
     public:
        virtual void pos_call(GLFWwindow *window, int xpos, int ypos) = 0;
        virtual void size_call(GLFWwindow *window, int width, int height) = 0;
        virtual void close_call(GLFWwindow *window) = 0;
        virtual void refresh_call(GLFWwindow *window) = 0;
        virtual void focus_call(GLFWwindow *window, int focused) = 0;
        virtual void iconify_call(GLFWwindow *window, int iconified) = 0;
        virtual void maximize_call(GLFWwindow *window, int maximized) = 0;
        virtual void frameBufferSize_call(GLFWwindow *window, int width, int height) = 0;
        virtual void contentScale_call(GLFWwindow *window, float xscale, float yscale) = 0;
        virtual void cursorEnter_call(GLFWwindow *window, int entered) = 0;

        widget_type widget_class() override { return widget_type::WINDOW; }
    };

    class other_widget : virtual public widget {
     public:
        virtual void drop_call(GLFWwindow *window, int path_count, const char *paths[]) = 0;

        widget_type widget_class() override { return widget_type::OTHER; }
    };

} // namespace universal_gui

#undef GLFW_INCLUDE_NONE
