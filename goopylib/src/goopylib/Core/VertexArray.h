#pragma once

#include "src/pch.h"
#include "src/goopylib/Core/Buffer.h"
#include "src/Platform/Independent/BaseVertexArray.h"

namespace gp {
    class VertexArray final : public BaseVertexArray {
    public:
        ~VertexArray() override;

        void bind() const override;

        void unbind() const override;

        void draw(int32_t count = 0) const override;

        void setVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;

        // Static Methods

        static Ref<VertexArray> create() {
            return Ref<VertexArray>(new VertexArray());
        }

        static Ref<VertexArray> create(int32_t count, uint32_t *indices) {
            return Ref<VertexArray>(new VertexArray(count, indices));
        }

        static Ref<VertexArray> create(std::initializer_list<uint32_t> indices) {
            return Ref<VertexArray>(new VertexArray(indices));
        }

    private:
        VertexArray();

        VertexArray(int32_t count, uint32_t *indices);

        VertexArray(std::initializer_list<uint32_t> indices);

        uint32_t m_RendererID = 0;
    };
}
