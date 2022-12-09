#include "pch.h"
#include "Base.h"

namespace {
    void OnUpdate() {
        gp::BaseWindow::updateAll();

        #if (GP_DEBUGGING)
        GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR) {
            printf("Unknown Error: 0x%x\n", error);
        }
        #endif
    }
}

namespace gp {
    bool glfw_initialized = false;

    GPAPI int Initialize() {
        #if GP_LOGGING
        gp::Log::Init();
        #endif

        gp::InitializeW3CX11();

        GP_CORE_INFO("Initializing goopylib");

        #if GP_USING_GLFW
        GP_CORE_TRACE("Initializing GLFW");

        glfwSetErrorCallback([](int error, const char *description) {
            std::cout << "ERROR: " << description << std::endl;
        });
        if (!glfwInit()) {
            return -1;
        }
        glfw_initialized = true;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #endif

        return 0;
    }

    GPAPI void Terminate() {
        GP_CORE_INFO("Terminating goopylib");

        BaseWindow::destroyAll();

        DeallocateW3CX11();

        #if GP_USING_GLFW
        GP_CORE_TRACE("Terminating GLFW");
        glfwTerminate();
        glfw_initialized = false;
        #endif
    }

    GPAPI void Update() {
        OnUpdate();
        #if GP_USING_GLFW
        glfwPollEvents();
        #endif
    }

    #if GP_USING_GLFW

    GPAPI void UpdateOnEvent() {
        glfwWaitEvents();
        OnUpdate();
    }

    GPAPI void UpdateTimeout(double timeout) {
        glfwWaitEventsTimeout(timeout);
        OnUpdate();
    }

    GPAPI std::string GLFWCompiledVersion() {
        return {glfwGetVersionString()};
    }

    GPAPI std::string GLFWCurrentVersion() {
        int major, minor, revision;
        glfwGetVersion(&major, &minor, &revision);

        return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(revision);
    }

    GPAPI int GetRefreshRate() {
        return glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
    }

    GPAPI int GetScreenWidth() {
        return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
    }

    GPAPI int GetScreenHeight() {
        return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
    }

    GPAPI int GetNumberOfMonitors() {
        int count;
        glfwGetMonitors(&count);
        return count;
    }

    GPAPI void SetBufferSwapInterval(int interval) {
        glfwSwapInterval(interval);
    }

    #endif

    #if GP_USING_OPENGL

    GPAPI std::string OpenGLVersion() {
        return {(char *) glGetString(GL_VERSION)};
    }

    #endif
}
