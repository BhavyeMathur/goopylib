#pragma once

#include <Python.h>
#include <iostream>

#include <memory>
#include <string>
#include <stdexcept>

#define EXPOSE_PYOBJECT_CLASS(ObjectType, name) \
if (PyType_Ready(&(ObjectType)) < 0) return nullptr; \
Py_INCREF(&(ObjectType)); \
if (PyModule_AddObject(m, name, (PyObject *) &(ObjectType)) < 0) { \
    Py_DECREF(&(ObjectType)); \
    Py_DECREF(m); \
    return nullptr; }

#define INITIALIZE_PYOBJECT(variable, value) Py_INCREF((value)); (variable) = value
#define SET_PYOBJECT(variable, value) PyObject *tmp = variable; INITIALIZE_PYOBJECT(variable, value); Py_XDECREF(tmp)
#define RETURN_PYOBJECT(name) Py_INCREF(name); return name
#define PYOBJECT_NAME(object) object->ob_type->tp_name

#define GETTER_SETTER(name) {#name, (getter) get_##name, (setter) set_##name, #name, nullptr}

#undef Py_VISIT
#define Py_VISIT(op)                                                    \
    do {                                                                \
        if (op) {                                                       \
            const int vret = visit(_PyObject_CAST(op), arg);            \
            if (vret)                                                   \
                return vret;                                            \
        }                                                               \
    } while (0)

#define isinstance(object, type) PyObject_IsInstance(object, (PyObject *) (type))
