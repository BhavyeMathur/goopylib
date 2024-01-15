#define GP_LOGGING_LEVEL 5
#include "src/goopylib/core/Buffer.h"

#if __APPLE__
#include <OpenGL/gl3.h>
#endif


// Vertex Buffer
namespace gp {
    VertexBuffer::VertexBuffer(uint32_t count, void *vertices)
            : Buffer(count) {
        GP_CORE_DEBUG("gp::VertexBuffer::VertexBuffer()");
        glGenBuffers(1, &m_RendererID);

        GP_CORE_DEBUG("Initializing Vertex Buffer {0}, count={1}", m_RendererID, count);

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, (long) (count * sizeof(float)), vertices, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() {
        GP_CORE_DEBUG("Deallocating Vertex Buffer {0}", m_RendererID);
        glDeleteBuffers(1, &m_RendererID);
    }

    void VertexBuffer::bind() const {
        GP_CORE_TRACE_ALL("Binding Vertex Buffer {0}", m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void VertexBuffer::unbind() {
        GP_CORE_WARN("Unbinding Array Buffers");
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::setData(const void *data, uint32_t count) {
        GP_CORE_TRACE("Setting Vertex Buffer {0} Data, count={1}", m_RendererID, count);

        bind();
        glBufferData(GL_ARRAY_BUFFER, count * m_Layout.m_Stride, data, GL_DYNAMIC_DRAW);

        m_Count = count;
    }

    void VertexBuffer::setData(const void *data, uint32_t count, uint32_t offset) const {
        GP_CORE_TRACE("Setting Vertex Buffer {0} Data", m_RendererID);

        bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset * m_Layout.m_Stride,
                        count * m_Layout.m_Stride, data);
    }
}

// Index Buffer
namespace gp {
    IndexBuffer::IndexBuffer(uint32_t count, uint32_t *indices)
            : Buffer(count) {
        GP_CORE_DEBUG("gp::IndexBuffer::IndexBuffer()");

        glGenBuffers(1, &m_RendererID);

        GP_CORE_DEBUG("Initializing Index Buffer {0}, count={1}", m_RendererID, count);

        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long) (count * sizeof(uint32_t)), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::IndexBuffer(std::initializer_list<uint32_t> indices)
            : Buffer(indices.size()) {
        GP_CORE_DEBUG("gp::IndexBuffer::IndexBuffer()");
        glGenBuffers(1, &m_RendererID);

        GP_CORE_DEBUG("Initializing Index Buffer {0}, count={1}", m_RendererID, indices.size());

        // TODO replace with std::array or unique ptr?
        #ifdef _MSC_VER
        auto *bufferData = new uint32_t[indices.size()];
        #else
        uint32_t bufferData[indices.size()];
        #endif

        std::copy(indices.begin(), indices.end(), bufferData);

        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long) (indices.size() * sizeof(uint32_t)), bufferData, GL_STATIC_DRAW);

        #ifdef _MSC_VER
        delete[] bufferData;
        #endif
    }

    IndexBuffer::~IndexBuffer() {
        GP_CORE_DEBUG("Deallocating Index Buffer {0}", m_RendererID);
        glDeleteBuffers(1, &m_RendererID);
    }

    void IndexBuffer::bind() const {
        GP_CORE_TRACE_ALL("Binding Index Buffer {0}", m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void IndexBuffer::unbind() {
        GP_CORE_WARN("Unbinding Element Array Buffers");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

// Uniform Buffer
namespace gp {
    UniformBuffer::UniformBuffer(BufferLayout &&layout)
            : m_Layout(layout) {
        glGenBuffers(1, &m_RendererID);

        GP_CORE_DEBUG("Initializing Uniform Buffer");

        bind();
    }

    UniformBuffer::~UniformBuffer() {
        glDeleteBuffers(1, &m_RendererID);
    }

    void UniformBuffer::bind() const {
        glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
    }

    void UniformBuffer::unbind() {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void UniformBuffer::setData(const void *data, uint32_t count) {
        GP_CORE_TRACE("Setting Uniform Buffer {0} Data, count={1}", m_RendererID, count);

        bind();
        glBufferData(GL_UNIFORM_BUFFER, count * m_Layout.m_Stride, data, GL_DYNAMIC_DRAW);

        m_Count = count;
    }

    void UniformBuffer::setData(const void *data, uint32_t count, uint32_t offset) const {
        GP_CORE_TRACE("Setting Uniform Buffer {0} Data", m_RendererID);

        bind();
        glBufferSubData(GL_UNIFORM_BUFFER, offset * m_Layout.m_Stride,
                        count * m_Layout.m_Stride, data);
    }

    void UniformBuffer::setBinding(uint32_t binding) const {
        bind();
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
    }
}
