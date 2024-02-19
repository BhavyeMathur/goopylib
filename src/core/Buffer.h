#pragma once

#include "gp.h"
#include "BufferLayout.h"


namespace gp {
    class GPAPI Buffer {
    public:
        Buffer(int32_t length, const BufferLayout &layout = {});

        Buffer(const Buffer &) = delete;

        Buffer(Buffer &&other) noexcept;

        virtual ~Buffer() noexcept;

        void init();

        void bind() const;

        void unbind() const;

        [[nodiscard]] int32_t length() const;

        virtual void setData(const void *data, int32_t length);

        void setData(const void *data, int32_t length, int32_t offset) const;

        [[nodiscard]] const BufferLayout &getLayout() const;

    protected:
        int32_t m_Length = 0;
        uint32_t m_RendererID = 0;

        BufferLayout m_Layout;

        [[nodiscard]] virtual uint32_t _getBufferTarget() const = 0;

        [[nodiscard]] virtual uint32_t _getBufferUsage() const;

        [[nodiscard]] virtual std::string _getClassString() const;
    };
}