#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <GLFW/glfw3.h>

#include "pch.h"
#include "util.h"
#include "easing.h"
#include "window.h"

// TODO monitor getter functions
// TODO mouse events
// TODO implement window icons
// TODO window pointer
// TODO make window key callback compatible with Python lambda functions (raises SIGSEGV error right now)
// TODO window maximize callback does not work

namespace core {
    static PyObject *init(PyObject *Py_UNUSED(self)) {
        if (gp::Initialize()) {
            PyErr_SetString(PyExc_ImportError, "Failed to initialise goopylib!");
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject *terminate(PyObject *Py_UNUSED(self)) {
        gp::Terminate();
        Py_RETURN_NONE;
    }

    static PyObject *update(PyObject *Py_UNUSED(self)) {
        gp::Update();
        Py_RETURN_NONE;
    }

    #if GP_USING_GLFW

    static PyObject *update_on_event(PyObject *Py_UNUSED(self), PyObject *Py_UNUSED(args)) {
        gp::UpdateOnEvent();
        Py_RETURN_NONE;
    }

    static PyObject *update_timeout(PyObject *Py_UNUSED(self), PyObject *arg) {
        #if GP_ERROR_CHECKING
        if (!PyFloat_Check(arg)) {
            RAISE_TYPE_ERROR(nullptr, "float", arg)
        }
        #endif

        double timeout = PyFloat_AsDouble(arg);
        gp::UpdateTimeout(timeout);

        Py_RETURN_NONE;
    }

    static PyObject *glfw_compiled_version(PyObject *Py_UNUSED(self)) {
        return PyUnicode_FromString(gp::GLFWCompiledVersion().c_str());
    }

    static PyObject *glfw_current_version(PyObject *Py_UNUSED(self)) {
        return PyUnicode_FromString(gp::GLFWCurrentVersion().c_str());
    }

    static PyObject *get_refresh_rate(PyObject *Py_UNUSED(self)) {
        CHECK_GLFW_INITIALIZED(nullptr)
        return PyLong_FromLong(gp::GetRefreshRate());
    }

    static PyObject *get_screen_width(PyObject *Py_UNUSED(self)) {
        CHECK_GLFW_INITIALIZED(nullptr)
        return PyLong_FromLong(gp::GetScreenWidth());
    }

    static PyObject *get_screen_height(PyObject *Py_UNUSED(self)) {
        CHECK_GLFW_INITIALIZED(nullptr)
        return PyLong_FromLong(gp::GetScreenHeight());
    }

    static PyObject *number_of_monitors(PyObject *Py_UNUSED(self)) {
        CHECK_GLFW_INITIALIZED(nullptr)
        return PyLong_FromLong(gp::GetNumberOfMonitors());
    }

    #endif

    #if GP_USING_OPENGL

    static PyObject *opengl_version(PyObject *Py_UNUSED(self)) {
        CHECK_GLFW_CONTEXT(nullptr)
        return PyUnicode_FromString(gp::OpenGLVersion().c_str());
    }

    #endif
}

namespace window {
    #if GP_USING_GLFW

    static PyObject *set_buffer_swap_interval(PyObject *Py_UNUSED(self), PyObject *arg) {
        #if GP_ERROR_CHECKING
        if (!PyLong_Check(arg)) {
            RAISE_TYPE_ERROR(nullptr, "integer", arg)
        }
        #endif

        gp::SetBufferSwapInterval((int) PyLong_AsLong(arg));
        Py_RETURN_NONE;
    }

    #endif
}

static PyMethodDef CoreMethods[] = {
        {"init", (PyCFunction) core::init, METH_NOARGS, "Initialise goopylib internally"},
        {"terminate", (PyCFunction) core::terminate, METH_NOARGS, "Terminates goopylib internally"},

        {"update", (PyCFunction) core::update, METH_NOARGS, "Updates and processes events"},

        #if GP_USING_GLFW

        {"update_on_event", (PyCFunction) core::update_on_event, METH_NOARGS, "Updates and processes events"},
        {"update_timeout", (PyCFunction) core::update_timeout, METH_O, "Updates and processes events"},

        {"glfw_compiled_version", (PyCFunction) core::glfw_compiled_version, METH_NOARGS,
         "Returns the version of GLFW goopylib was compiled with"},
        {"glfw_current_version", (PyCFunction) core::glfw_current_version, METH_NOARGS,
         "Returns the version of GLFW currently being used"},

        {"get_refresh_rate", (PyCFunction) core::get_refresh_rate, METH_NOARGS,
         "Returns the refresh rate of the primary monitor"},
        {"get_screen_width", (PyCFunction) core::get_screen_width, METH_NOARGS,
         "Returns the width of the primary monitor"},
        {"get_screen_height", (PyCFunction) core::get_screen_height, METH_NOARGS,
         "Returns the height of the primary monitor"},
        {"number_of_monitors", (PyCFunction) core::number_of_monitors, METH_NOARGS,
         "Returns the number of monitors connected"},

        {"set_buffer_swap_interval", (PyCFunction) window::set_buffer_swap_interval, METH_O,
         "Sets the swap interval for the window buffers"},

        #endif

        #if GP_USING_OPENGL

        {"opengl_version", (PyCFunction) core::opengl_version, METH_NOARGS, "Returns the version of OpenGL being used"},

        #endif

        {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef coremodule = {
        PyModuleDef_HEAD_INIT,
        .m_name = "core",
        .m_size = -1,
        .m_methods = CoreMethods,
};

PyMODINIT_FUNC PyInit_core(void) {
    PyObject *m;
    m = PyModule_Create(&coremodule);
    if (m == nullptr) { return nullptr; }

    if (PyInit_easing(m)) { return nullptr; }
    if (PyInit_window(m)) { return nullptr; }

    return m;
}


