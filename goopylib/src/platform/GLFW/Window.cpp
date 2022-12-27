#include "src/goopylib/core/Window.h"
#include <GLFW/glfw3.h>

#if !GP_LOG_WINDOW
#undef GP_LOGGING
#endif

#include "src/goopylib/debug/LogMacros.h"

#if !GP_VALUE_CHECK_WINDOW
#undef GP_VALUE_CHECKING
#endif

#include "src/goopylib/debug/Error.h"

#if GP_USING_GLFW

namespace gp {
    enum class CursorMode {
        Normal = GLFW_CURSOR_NORMAL,
        Disabled = GLFW_CURSOR_DISABLED,
        Hidden = GLFW_CURSOR_HIDDEN
    };
}

namespace gp {
    Window::Window(int width, int height, const char *title)
            : m_Width(width),
              m_Height(height),
              m_Title(title),
              m_Background(Color(255, 255, 255)),

              m_xPos(0),
              m_yPos(0),
              m_WindowedXPos(m_xPos),
              m_WindowedYPos(m_yPos),

              m_WindowedWidth(m_Width),
              m_WindowedHeight(m_Height),

              m_Renderer((float) m_Width, (float) m_Height) {
        GP_CORE_INFO("Creating window '{0}' ({1}, {2})", m_Title, m_Width, m_Height);

        m_Window = glfwCreateWindow(m_Width,
                                    m_Height,
                                    m_Title, nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);

        glfwSetWindowUserPointer(m_Window, this);

        super();

        s_Instances.push_back(this);
    }

    Window::~Window() {
        GP_CORE_INFO("Deallocating window '{0}'", m_Title);
        destroy();

        s_Instances.erase(std::remove(s_Instances.begin(), s_Instances.end(), this), s_Instances.end());
    }

    GLFWwindow *Window::getWindowGLFW() {
        return m_Window;
    }
}

// Window attribute methods
namespace gp {
    bool Window::isResizable() const {
        return glfwGetWindowAttrib(m_Window, GLFW_RESIZABLE);
    }

    void Window::setResizable(bool value) {
        GP_CORE_TRACE_ALL("Set '{0}' resizable -> {1}", m_Title, value);
        glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, value ? GLFW_TRUE : GLFW_FALSE);
    }

    bool Window::isDecorated() const {
        return glfwGetWindowAttrib(m_Window, GLFW_DECORATED);
    }

    void Window::setDecorated(bool value) {
        GP_CORE_TRACE_ALL("Set '{0}' decorated -> {1}", m_Title, value);
        glfwSetWindowAttrib(m_Window, GLFW_DECORATED, value ? GLFW_TRUE : GLFW_FALSE);
    }

    bool Window::isFloating() const {
        return glfwGetWindowAttrib(m_Window, GLFW_FLOATING);
    }

    void Window::setFloating(bool value) {
        GP_CORE_TRACE_ALL("Set '{0}' floating -> {1}", m_Title, value);
        glfwSetWindowAttrib(m_Window, GLFW_FLOATING, value ? GLFW_TRUE : GLFW_FALSE);
    }

    bool Window::isAutoMinimized() const {
        return glfwGetWindowAttrib(m_Window, GLFW_AUTO_ICONIFY);
    }

    void Window::setAutoMinimized(bool value) {
        GP_CORE_TRACE_ALL("Set '{0}' auto minimized -> {1}", m_Title, value);
        glfwSetWindowAttrib(m_Window, GLFW_AUTO_ICONIFY, value ? GLFW_TRUE : GLFW_FALSE);
    }

    bool Window::isFocusedOnShow() const {
        return glfwGetWindowAttrib(m_Window, GLFW_FOCUS_ON_SHOW);
    }

    void Window::setFocusedOnShow(bool value) {
        GP_CORE_TRACE_ALL("Set '{0}' focused on show -> {1}", m_Title, value);
        glfwSetWindowAttrib(m_Window, GLFW_FOCUS_ON_SHOW, value ? GLFW_TRUE : GLFW_FALSE);
    }

    WindowFrame Window::getFrameSize() const {
        WindowFrame value{};
        glfwGetWindowFrameSize(m_Window, &value.left, &value.top, &value.right, &value.bottom);
        return value;
    }

    ContentScale Window::getContentScale() const {
        ContentScale value{};
        glfwGetWindowContentScale(m_Window, &value.xScale, &value.yScale);
        return value;
    }

    FramebufferSize Window::getFramebufferSize() const {
        FramebufferSize value{};
        glfwGetFramebufferSize(m_Window, &value.width, &value.height);
        return value;
    }
}

// Window state methods
namespace gp {
    bool Window::isFullscreen() const {
        return (bool) glfwGetWindowMonitor(m_Window);
    }

    bool Window::isMaximized() const {
        return (bool) glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED);
    }

    bool Window::isMinimized() const {
        return (bool) glfwGetWindowAttrib(m_Window, GLFW_ICONIFIED);
    }

    bool Window::isVisible() const {
        return (bool) glfwGetWindowAttrib(m_Window, GLFW_VISIBLE);
    }

    bool Window::hasFocus() const {
        return (bool) glfwGetWindowAttrib(m_Window, GLFW_FOCUSED);
    }

    void Window::requestAttention() const {
        glfwRequestWindowAttention(m_Window);
    }
}

// Window callbacks
namespace gp {
    void Window::_setResizeCallback() const {
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onResize(width, height);
        });
    }

    void Window::_setCloseCallback() const {
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onClose();
        });
    }

    void Window::_setPositionCallback() const {
        glfwSetWindowPosCallback(m_Window, [](GLFWwindow *window, int xPos, int yPos) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onMove(xPos, yPos);
        });
    }

    void Window::_setMinimizeCallback() const {
        glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow *window, int iconified) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onMinimize(iconified == GLFW_TRUE);
        });
    }

    void Window::_setMaximizeCallback() const {
        // TODO fix issue with maximize callback not working. Tested on M1 MacOS Monterey 12.4, 3.3.8 Cocoa NSGL EGL OSMesa dynamic, OpenGL 4.1
        glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow *window, int maximized) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onMaximize(maximized == GLFW_TRUE);
        });
    }

    void Window::_setFocusedCallback() const {
        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow *window, int focused) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onFocus(focused == GLFW_TRUE);
        });
    }

    void Window::_setRefreshCallback() const {
        glfwSetWindowRefreshCallback(m_Window, [](GLFWwindow *window) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onRefreshRequired();
        });
    }

    void Window::_setContentScaleCallback() const {
        glfwSetWindowContentScaleCallback(m_Window, [](GLFWwindow *window, float xScale, float yScale) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onContentScale(xScale, yScale);
        });
    }

    void Window::_setFramebufferSizeCallback() const {
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onFramebufferSize(width, height);
        });
    }

    void Window::_setMouseMotionCallback() const {
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onMouseMotion((float) xPos, (float) yPos);
        });
    }

    void Window::_setMouseEnterCallback() const {
        glfwSetCursorEnterCallback(m_Window, [](GLFWwindow *window, int entered) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onMouseEnter((bool) entered);
        });
    }

    void Window::_setScrollCallback() const {
        glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xScroll, double yScroll) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onScroll((float) xScroll, (float) yScroll);
        });
    }

    void Window::_setKeyCallback() const {
        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onKeyPress(key, scancode, action, mods);
        });
    }

    void Window::_setMouseButtonCallback() const {
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
            Window *windowObject = (Window *) glfwGetWindowUserPointer(window);
            windowObject->onMousePress(button, action, mods);
        });
    }
}

// Window input events

namespace gp {
    bool Window::isMouseHovering() const {
        return (bool) glfwGetWindowAttrib(m_Window, GLFW_HOVERED);
    }

    Point Window::getMousePosition() const {
        double xPos, yPos;
        glfwGetCursorPos(m_Window, &xPos, &yPos);

        return {(float) xPos, (float) yPos};
    }

    void Window::setCursorMode(CursorMode mode) const {
        glfwSetInputMode(m_Window, GLFW_CURSOR, (int) mode);
    }

    bool Window::checkShiftKey() const {
        return m_KeyModifiers & (1 << 0);
    }

    bool Window::checkControlKey() const {
        return m_KeyModifiers & (1 << 1);
    }

    bool Window::checkAltKey() const {
        return m_KeyModifiers & (1 << 2);
    }

    bool Window::checkSuperKey() const {
        return m_KeyModifiers & (1 << 3);
    }

    int Window::checkKey(int key) const {
        return glfwGetKey(m_Window, key);
    }

    bool Window::checkMouseButton(int button) const {
        return glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
    }
}

// Window private methods
namespace gp {
    bool Window::_isClosed() const {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::_update() const {
        GP_CORE_TRACE_ALL("gp::Window::_update() - {0}", m_Title);

        glfwSwapBuffers(m_Window);
    }

    void Window::_destroy() const {
        GP_CORE_TRACE_ALL("_destroy() '{0}'", m_Title);
        glfwDestroyWindow(m_Window);
    }

    void Window::_fullscreen() const {
        if (!isFullscreen()) {
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
    }

    void Window::_unfullscreen(int width, int height, int xPos, int yPos) const {
        glfwSetWindowMonitor(m_Window, nullptr, xPos, yPos, width, height, 0);
    }

    void Window::_restore() const {
        glfwRestoreWindow(m_Window);
    }

    void Window::_maximize() const {
        glfwMaximizeWindow(m_Window);
    }

    void Window::_minimize() const {
        glfwIconifyWindow(m_Window);
    }

    void Window::_show() const {
        glfwShowWindow(m_Window);
    }

    void Window::_hide() const {
        glfwHideWindow(m_Window);
    }

    void Window::_focus() const {
        glfwFocusWindow(m_Window);
    }
}

// Window private update methods
namespace gp {
    void Window::_updateSize() const {
        glfwSetWindowSize(m_Window, m_Width, m_Height);
    }

    void Window::_updateTitle() const {
        glfwSetWindowTitle(m_Window, m_Title);
    }

    void Window::_updatePosition() const {
        glfwSetWindowPos(m_Window, m_xPos, m_yPos);
    }

    void Window::_updateBackground() const {
        GP_CORE_TRACE_ALL("gp::Window::_updateBackground() - {0}", m_Title);

        glfwMakeContextCurrent(m_Window);
        glClearColor(m_Background.getRedf(),
                     m_Background.getGreenf(),
                     m_Background.getBluef(), 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Window::_updateSizeLimits() const {
        glfwSetWindowSizeLimits(m_Window, m_MinWidth, m_MinHeight,
                                m_MaxWidth, m_MaxHeight);
    }

    void Window::_updateAspectRatio(int numerator, int denominator) const {
        glfwSetWindowAspectRatio(m_Window, numerator, denominator);
    }
}

#endif