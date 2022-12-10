#include "goopylib/Core/Buffer.h"

#include <utility>

namespace gp {
    GLenum shaderOpenGLType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                return GL_FLOAT;

            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool:
                return GL_BOOL;

            case ShaderDataType::None:
                GP_CORE_ERROR("Unrecognised Shader Type");
                return 0;
        }
    }
}

// Vertex Buffer
namespace gp {
    VertexBuffer::VertexBuffer(BufferLayout layout, float *vertices, int count)
            : BaseVertexBuffer(count) {
        glGenBuffers(1, &m_RendererID);

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, (long) (count * sizeof(float)), vertices, GL_STATIC_DRAW);

        setLayout(std::move(layout));
    }

    VertexBuffer::VertexBuffer(float *vertices, int count)
            : BaseVertexBuffer(count) {
        glGenBuffers(1, &m_RendererID);

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, (long) (count * sizeof(float)), vertices, GL_STATIC_DRAW);
    }

    VertexBuffer::VertexBuffer(BufferLayout layout, std::initializer_list<float> vertices)
            : BaseVertexBuffer((int) vertices.size()) {
        glGenBuffers(1, &m_RendererID);

        float bufferData[vertices.size()];
        std::copy(vertices.begin(), vertices.end(), bufferData);

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, (long) (vertices.size() * sizeof(float)), bufferData, GL_STATIC_DRAW);

        setLayout(std::move(layout));
    }

    VertexBuffer::VertexBuffer(std::initializer_list<float> vertices)
            : BaseVertexBuffer((int) vertices.size()) {
        glGenBuffers(1, &m_RendererID);

        float bufferData[vertices.size()];
        std::copy(vertices.begin(), vertices.end(), bufferData);

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, (long) (vertices.size() * sizeof(float)), bufferData, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &m_RendererID);
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void VertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::setData(const void *data, uint32_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}

// Index Buffer
namespace gp {
    IndexBuffer::IndexBuffer(uint32_t *indices, int count)
            : BaseIndexBuffer(count)
    {
        glGenBuffers(1, &m_RendererID);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long) (count * sizeof(uint32_t)), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::IndexBuffer(std::initializer_list<uint32_t> indices)
            : BaseIndexBuffer((int) indices.size())
    {
        glGenBuffers(1, &m_RendererID);

        uint32_t bufferData[indices.size()];
        std::copy(indices.begin(), indices.end(), bufferData);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long) (indices.size() * sizeof(uint32_t)), bufferData, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_RendererID);
    }

    std::shared_ptr<BaseIndexBuffer> IndexBuffer::create(uint32_t *vertices, int count) {
        return std::make_shared<IndexBuffer>(vertices, count);
    }

    std::shared_ptr<BaseIndexBuffer> IndexBuffer::create(std::initializer_list<uint32_t> vertices) {
        return std::make_shared<IndexBuffer>(vertices);
    }

    void IndexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void IndexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
