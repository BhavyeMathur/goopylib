#define GP_LOGGING_LEVEL 3
#include "src/goopylib/core/VertexArray.h"


namespace gp {
    void VertexArray::setIndexBuffer(std::initializer_list<uint32_t> indices) {
        GP_CORE_TRACE("VertexArray::setIndexBuffer()");

        bind();
        m_IndexBuffer = Ref<IndexBuffer>(new IndexBuffer(indices));
    }

    void VertexArray::setIndexBuffer(uint32_t count, uint32_t *indices) {
        GP_CORE_TRACE("VertexArray::setIndexBuffer({0})", count);

        bind();
        m_IndexBuffer = Ref<IndexBuffer>(new IndexBuffer(count, indices));
    }

    const Ref<VertexBuffer> &VertexArray::getVertexBuffer() const {
        GP_CORE_TRACE_ALL("VertexArray::getVertexBuffer()");
        return m_VertexBuffer;
    }

    const Ref<IndexBuffer> &VertexArray::getIndexBuffer() const {
        GP_CORE_TRACE_ALL("VertexArray::getIndexBuffer()");
        return m_IndexBuffer;
    }
}
