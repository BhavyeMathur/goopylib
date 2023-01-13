#include "src/goopylib/texture/TextureAtlas.h"

#if GP_USING_OPENGL

#if __APPLE__

#include <OpenGL/gl.h>

#endif

#if GP_USING_GLAD

#include <glad/glad.h>

#endif

namespace gp {
    void TextureAtlas::init() {
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, (int32_t *) &s_Width);
        s_Height = s_Width;
    }
}

#endif
