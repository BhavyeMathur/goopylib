#include "UniformBuffer.h"
#include <opengl.h>

namespace gp {
    UniformBuffer::UniformBuffer(const BufferLayout &layout)
        : Buffer(0, layout) {
    }

    void UniformBuffer::setBinding(uint32_t binding) const {
        bind();
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
    }

    uint32_t UniformBuffer::_getBufferTarget() const {
        return GL_UNIFORM_BUFFER;
    }
}
