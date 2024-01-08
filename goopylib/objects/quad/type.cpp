#include "quad.h"
#include "methods.h"

namespace quad {
    static PyMethodDef methods[] = {
            {"set_color",        (PyCFunction) set_color,        METH_VARARGS,
                    "Sets the color of the object"},
            {"set_transparency", (PyCFunction) set_transparency, METH_VARARGS,
                    "Sets the transparency of the object"},

            {nullptr}
    };
}

PyTypeObject QuadType = {
        PyVarObject_HEAD_INIT(nullptr, 0)
        "goopylib.Quad",
        sizeof(QuadObject),
        0,
        (destructor) quad::dealloc,
        0,
        nullptr,
        nullptr,
        nullptr,
        (reprfunc) quad::repr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_BASETYPE,
        "",
        (traverseproc) quad::traverse,
        (inquiry) quad::clear,
        nullptr,
        0,
        nullptr,
        nullptr,
        (PyMethodDef *) quad::methods,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        0,
        (initproc) quad::init,
        nullptr,
        quad::new_,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        0,
        nullptr,
        nullptr
};
