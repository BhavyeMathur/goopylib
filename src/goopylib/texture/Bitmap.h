#pragma once

#include "gp.h"
#include "TextureType.h"


namespace gp {
    class Bitmap {

    public:
        GPAPI ~Bitmap();

        GPAPI uint32_t getWidth() const;

        GPAPI uint32_t getHeight() const;

        GPAPI uint32_t getChannels() const;

        GPAPI uint8_t* getData() const;

        GPAPI void setTextureType(TextureType value);

        GPAPI TextureType getTextureType() const;

        GPAPI void saveBitmap(const std::string& filepath) const;

        GPAPI static Ref<Bitmap> create(uint32_t width, uint32_t height, uint32_t channels, uint8_t *data);

        GPAPI static Ref<Bitmap> create(const char* filepath);

    private:
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        uint32_t m_Channels = 0;

        uint8_t *m_Data = nullptr;
        bool m_IsImage = false;

        TextureType m_TextureType = TextureType::None;

        GPAPI Bitmap() = default;

        GPAPI Bitmap(uint32_t width, uint32_t height, uint32_t channels, uint8_t *data);

        GPAPI Bitmap(const char* filepath);
    };
}
