#pragma once

#include "BaseBuffer.h"
#include "src/goopylib/Core/Buffer.h"

namespace gp {
    class BaseVertexArray {
    public:
        BaseVertexArray();

        virtual ~BaseVertexArray();

        virtual void bind() const = 0;

        virtual void unbind() const = 0;

        virtual void draw(int32_t count = 0) const = 0;

        virtual void setVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) = 0;

        const Ref<VertexBuffer> &getVertexBuffer() const;

        void setIndexBuffer(const Ref<IndexBuffer> &indexBuffer);

        void setIndexBuffer(std::initializer_list<uint32_t> indices);

        const Ref<IndexBuffer> &getIndexBuffer() const;

    protected:
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
    };
}
