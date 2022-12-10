#include "module.h"

static PyMethodDef BufferMethods[] = {
        {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef buffersmodule = {
        PyModuleDef_HEAD_INIT,
        .m_name = "buffers",
        .m_size = -1,
        .m_methods = BufferMethods,
};


PyMODINIT_FUNC PyInit_buffers() {
    #if GP_LOGGING
    std::cout << "Initializing buffers logger" << std::endl;
    gp::Log::Init();
    #endif

    GP_PY_TRACE("Initializing buffers module");

    PyObject *m;
    m = PyModule_Create(&buffersmodule);
    if (m == nullptr) {
        return nullptr;
    }

    EXPOSE_CLASS(BufferElementType, "BufferElement")
    EXPOSE_CLASS(VertexBufferType, "VertexBuffer")
    EXPOSE_CLASS(IndexBufferType, "IndexBuffer")

    EXPOSE_CLASS(VertexArrayType, "VertexArray")

    return m;
}
