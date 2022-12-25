#include "core.h"
#include "src/goopylib/core/Core.h"

#include <GLFW/glfw3.h>

// TODO monitor getter functions

namespace core {
    static PyObject *init(PyObject *Py_UNUSED(self)) {
        GP_PY_TRACE("core.init()");

        gp::init();

        if (gp::checkError()) {
            return nullptr;
        }

        Py_RETURN_NONE;
    }

    static PyObject *terminate(PyObject *Py_UNUSED(self)) {
        GP_PY_TRACE("core.terminate()");

        gp::terminate();

        if (gp::checkError()) {
            return nullptr;
        }

        Py_RETURN_NONE;
    }

    static PyObject *update(PyObject *Py_UNUSED(self)) {
        GP_PY_TRACE("core.update()");

        gp::update();

        if (gp::checkError()) {
            return nullptr;
        }

        Py_RETURN_NONE;
    }

    #if GP_USING_GLFW

    static PyObject *update_on_event(PyObject *Py_UNUSED(self), PyObject *Py_UNUSED(args)) {
        GP_PY_TRACE("core.update_on_event()");

        gp::updateOnEvent();

        if (gp::checkError()) {
            return nullptr;
        }

        Py_RETURN_NONE;
    }

    static PyObject *update_timeout(PyObject *Py_UNUSED(self), PyObject *arg) {
        GP_PY_TRACE("core.update_on_timeout()");

        #if GP_TYPE_CHECKING
        if (!PyNumber_Check(arg)) {
            RAISE_TYPE_ERROR(nullptr, "number", arg);
        }
        #endif

        double timeout = PyFloat_AsDouble(arg);
        gp::updateTimeout(timeout);

        if (gp::checkError()) {
            return nullptr;
        }

        Py_RETURN_NONE;
    }

    static PyObject *glfw_compiled_version(PyObject *Py_UNUSED(self)) {
        GP_PY_TRACE("core.glfw_compiled_version()");

        return PyUnicode_FromString(gp::glfwCompiledVersion().c_str());
    }

    static PyObject *glfw_current_version(PyObject *Py_UNUSED(self)) {
        GP_PY_TRACE("core.glfw_current_version()");

        return PyUnicode_FromString(gp::glfwCurrentVersion().c_str());
    }

    static PyObject *get_refresh_rate(PyObject *Py_UNUSED(self)) {
        GP_PY_TRACE("core.get_refresh_rate()");

        CHECK_GLFW_INITIALIZED(nullptr)
        return PyLong_FromLong(gp::getRefreshRate());
    }

    static PyObject *get_screen_width(PyObject *Py_UNUSED(self)) {
        GP_PY_TRACE("core.get_screen_width()");

        CHECK_GLFW_INITIALIZED(nullptr)
        return PyLong_FromLong(gp::getScreenWidth());
    }

    static PyObject *get_screen_height(PyObject *Py_UNUSED(self)) {
        GP_PY_TRACE("core.get_screen_height()");

        CHECK_GLFW_INITIALIZED(nullptr)
        return PyLong_FromLong(gp::getScreenHeight());
    }

    static PyObject *number_of_monitors(PyObject *Py_UNUSED(self)) {
        GP_PY_TRACE("core.number_of_monitors()");

        CHECK_GLFW_INITIALIZED(nullptr)
        return PyLong_FromLong(gp::getNumberOfMonitors());
    }

    static PyObject *set_buffer_swap_interval(PyObject *Py_UNUSED(self), PyObject *arg) {
        GP_PY_TRACE("core.set_buffer_swap_interval()");

        #if GP_TYPE_CHECKING
        if (!PyLong_Check(arg)) {
            RAISE_TYPE_ERROR(nullptr, "integer", arg);
        }
        #endif

        gp::setBufferSwapInterval((int32_t) PyLong_AsLong(arg));
        Py_RETURN_NONE;
    }

    #endif

    #if GP_USING_OPENGL

    static PyObject *opengl_version(PyObject *Py_UNUSED(self)) {
        GP_PY_TRACE("core.opengl_version()");

        CHECK_GLFW_CONTEXT(nullptr)
        return PyUnicode_FromString(gp::openglVersion().c_str());
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

        {"glfw_compiled_version", (PyCFunction) core::glfw_compiled_version, METH_NOARGS, "Returns the version of GLFW goopylib was compiled with"},
        {"glfw_current_version", (PyCFunction) core::glfw_current_version, METH_NOARGS, "Returns the version of GLFW currently being used"},

        {"get_refresh_rate", (PyCFunction) core::get_refresh_rate, METH_NOARGS, "Returns the refresh rate of the primary monitor"},
        {"get_screen_width", (PyCFunction) core::get_screen_width, METH_NOARGS, "Returns the width of the primary monitor"},
        {"get_screen_height", (PyCFunction) core::get_screen_height, METH_NOARGS, "Returns the height of the primary monitor"},
        {"number_of_monitors", (PyCFunction) core::number_of_monitors, METH_NOARGS, "Returns the number of monitors connected"},

        {"set_buffer_swap_interval", (PyCFunction) core::set_buffer_swap_interval, METH_O, "Sets the swap interval for the window buffers"},

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
    #if GP_LOGGING
    std::cout << "PyInit_core()" << std::endl;
    #endif
    gp::init();

    PyObject *m = PyModule_Create(&coremodule);
    if (m == nullptr) {
        return nullptr;
    }

    return m;
}
