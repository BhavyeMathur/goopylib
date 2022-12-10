#include "BaseBuffer.h"

#include <utility>

// Buffer Layout Element
namespace gp {
    int shaderTypeSize(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:
                return 4;
            case ShaderDataType::Float2:
                return 4 * 2;
            case ShaderDataType::Float3:
                return 4 * 3;
            case ShaderDataType::Float4:
                return 4 * 4;
            case ShaderDataType::Mat3:
                return 4 * 3 * 3;
            case ShaderDataType::Mat4:
                return 4 * 4 * 4;
            case ShaderDataType::Int:
                return 4;
            case ShaderDataType::Int2:
                return 4 * 2;
            case ShaderDataType::Int3:
                return 4 * 3;
            case ShaderDataType::Int4:
                return 4 * 4;
            case ShaderDataType::Bool:
                return 1;
            case ShaderDataType::None:
                GP_CORE_ERROR("Unrecognised Shader Type");
                return 0;
        }
    }

    BufferElement::BufferElement(ShaderDataType type, const char *name, bool normalized)
            : m_Name(name),
              m_Type(type),
              m_Size(shaderTypeSize(type)),
              m_Offset(0),
              m_Normalized(normalized) {
    }

    int BufferElement::getCount() const {
        switch (m_Type) {
            case ShaderDataType::Float:
                return 1;
            case ShaderDataType::Float2:
                return 2;
            case ShaderDataType::Float3:
                return 3;
            case ShaderDataType::Float4:
                return 4;
            case ShaderDataType::Mat3:
                return 3; // 3* float3
            case ShaderDataType::Mat4:
                return 4; // 4* float4
            case ShaderDataType::Int:
                return 1;
            case ShaderDataType::Int2:
                return 2;
            case ShaderDataType::Int3:
                return 3;
            case ShaderDataType::Int4:
                return 4;
            case ShaderDataType::Bool:
                return 1;
            case ShaderDataType::None:
                GP_CORE_ERROR("Unrecognised Shader Type");
                return 0;
        }
    }

    const char *BufferElement::getName() const {
        return m_Name;
    }

    ShaderDataType BufferElement::getType() const {
        return m_Type;
    }

    bool BufferElement::isNormalized() const {
        return m_Normalized;
    }
}

// Buffer Layout
namespace gp {
    BufferLayout::BufferLayout(BufferElement *elements, int count) {
        const gp::BufferElement *end = &elements[count];

        for (gp::BufferElement *element = elements; element != end; element++) {
            m_Elements.push_back(*element);
        }

        calculateOffsetAndStride();
    }

    BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
            : m_Elements(elements) {
        calculateOffsetAndStride();
    }

    int BufferLayout::getStride() const {
        return m_Stride;
    }

    const std::vector<BufferElement> &BufferLayout::getElements() const {
        return m_Elements;
    }

    std::vector<BufferElement>::iterator BufferLayout::begin() {
        return m_Elements.begin();
    }

    std::vector<BufferElement>::iterator BufferLayout::end() {
        return m_Elements.end();
    }

    std::vector<BufferElement>::const_iterator BufferLayout::begin() const {
        return m_Elements.begin();
    }

    std::vector<BufferElement>::const_iterator BufferLayout::end() const {
        return m_Elements.end();
    }

    void BufferLayout::calculateOffsetAndStride() {
        m_Stride = 0;
        for (auto &element: m_Elements) {
            element.m_Offset = m_Stride;
            m_Stride += element.m_Size;
        }
    }
}

// Base Vertex Buffer
namespace gp {
    BaseVertexBuffer::BaseVertexBuffer(int count)
            : BaseBuffer(count) {
        GP_CORE_TRACE("Initializing Vertex Buffer");
    }

    BaseVertexBuffer::~BaseVertexBuffer() {
        GP_CORE_TRACE("Deallocating Vertex Buffer");
    }

    const BufferLayout &BaseVertexBuffer::getLayout() const {
        return m_Layout;
    }

    void BaseVertexBuffer::setLayout(BufferLayout layout) {
        m_Layout = std::move(layout);
    }
}

// Base Index Buffer
namespace gp {
    BaseIndexBuffer::BaseIndexBuffer(int count)
            : BaseBuffer(count) {
        GP_CORE_TRACE("Initializing Index Buffer");
    }

    BaseIndexBuffer::~BaseIndexBuffer() {
        GP_CORE_TRACE("Deallocating Index Buffer");
    }
}
