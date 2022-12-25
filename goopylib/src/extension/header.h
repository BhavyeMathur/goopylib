#pragma once

#define GP_PY_LOGGING 3

#include "src/gp.h"
#include <Python.h>
#include <iostream>

#define GP_TYPE_CHECKING true

#if GP_ERROR_CHECKING
#define CHECK_GLFW_INITIALIZED(val) \
if (!gp::glfw_initialized) { \
PyErr_SetString(PyExc_RuntimeError, "goopylib has not been initialised! Use gp.init() first."); \
    return val; \
}

#define CHECK_GLFW_CONTEXT(val) \
CHECK_GLFW_INITIALIZED(val) \
if (glfwGetCurrentContext() == nullptr) { \
    PyErr_SetString(PyExc_RuntimeError, "goopylib Window must be created"); \
    return val; \
}

#define RAISE_TYPE_ERROR(val, type, value) \
PyErr_Format(PyExc_TypeError, #type " argument expected, got %S", PyObject_Type(value)); \
return val

#define RAISE_VALUE_ERROR(val, err) \
PyErr_SetString(PyExc_ValueError, err); \
return val

#else

#define CHECK_GLFW_INITIALIZED(val)
#define CHECK_GLFW_CONTEXT(val)

#define RAISE_TYPE_ERROR(val, type, value)
#define RAISE_VALUE_ERROR(val, err)

#endif

#define EXPOSE_CLASS(ObjectType, name) \
if (PyType_Ready(&(ObjectType)) < 0) { \
    return nullptr; \
} \
Py_INCREF(&(ObjectType)); \
if (PyModule_AddObject(m, name, (PyObject *) &(ObjectType)) < 0) { \
    Py_DECREF(&(ObjectType)); \
    Py_DECREF(m); \
    return nullptr; \
}

#define SET_PYOBJECT(attribute, value) \
PyObject *tmp = attribute; \
Py_INCREF((value)); \
(attribute) = value; \
Py_XDECREF(tmp)

#define RETURN_PYOBJECT(name) \
Py_INCREF(name); \
return name

#define TYPE_NAME(object) object->ob_type->tp_name

namespace gp {
    int checkError() {
        switch (gp::getErrorCode()) {
            case gp::ErrorType::RuntimeError:
                PyErr_SetString(PyExc_RuntimeError, gp::getErrorMessage());
                return -1;

            case gp::ErrorType::ValueError:
                PyErr_SetString(PyExc_ValueError, gp::getErrorMessage());
                return -1;

            default:
                return 0;
        }
    }
}
