#include "Renderer.h"

#include "src/goopylib/core/VertexArray.h"
#include "src/goopylib/shader/Shader.h"

#include "src/goopylib/texture/Texture2D.h"
#include "src/goopylib/texture/TextureAtlas.h"

#include "src/goopylib/objects/Line.h"
#include "src/goopylib/objects/Triangle.h"
#include "src/goopylib/objects/Quad.h"
#include "src/goopylib/objects/Ellipse.h"
#include "src/goopylib/objects/TexturedQuad.h"
#include "src/goopylib/objects/text/Text.h"

#include "src/config.h"

#if (GP_LOG_RENDERER != true) and (GP_LOG_RENDERER <= GP_LOGGING_LEVEL)
#undef GP_LOGGING_LEVEL
#define GP_LOGGING_LEVEL GP_LOG_RENDERER
#endif

#if !GP_VALUE_CHECK_RENDERER
#undef GP_VALUE_CHECKING
#undef GP_TYPE_CHECKING
#undef GP_ERROR_CHECKING
#endif

#include "src/goopylib/debug/LogMacros.h"
#include "src/goopylib/debug/Error.h"

const char *solidVertexShader =

        #include "src/goopylib/shader/solid.vert"

const char *solidFragmentShader =

        #include "src/goopylib/shader/solid.frag"

const char *ellipseVertexShader =

        #include "src/goopylib/shader/ellipse.vert"

const char *ellipseFragmentShader =

        #include "src/goopylib/shader/ellipse.frag"

const char *textureVertexShader =

        #include "src/goopylib/shader/texture.vert"

const char *textureFragmentShader =

        #include "src/goopylib/shader/texture.frag"

const char *textBitmapFragmentShader =

        #include "src/goopylib/shader/text.frag"

const char *textSDFFragmentShader =

        #include "src/goopylib/shader/textSDF.frag"

namespace gp {
    Renderer::Renderer(float width, float height)
            : m_Camera(-width / 2, width / 2, -height / 2, height / 2) {

    }

    void Renderer::init() {
        GP_CORE_INFO("Rendering::init() initializing Renderer");

        Line::init();
        Texture2D::init();
        TextureAtlas::init();

        m_AtlasMono = new TextureAtlas(1);
        m_AtlasRGB = new TextureAtlas(3);
        m_AtlasRGBA = new TextureAtlas(4);

        GP_CORE_TRACE("Rendering::init() initializing Solid Shader");
        m_SolidShader = CreateRef<Shader>(solidVertexShader, solidFragmentShader);

        GP_CORE_TRACE("Rendering::init() initializing Ellipse Shader");
        m_EllipseShader = CreateRef<Shader>(ellipseVertexShader, ellipseFragmentShader);

        _createLineBuffer();
        _createTriangleBuffer();
        _createQuadBuffer();
        _createEllipseBuffer();

        GP_CORE_TRACE("Rendering::init() initializing text Shaders");
        m_TextBitmapShader = CreateRef<Shader>(textureVertexShader, textBitmapFragmentShader);
        m_TextSDFShader = CreateRef<Shader>(textureVertexShader, textSDFFragmentShader);

        int32_t samplers[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8,
                                9, 10, 11, 12, 13, 14, 15};
        m_TextBitmapShader->set("Texture", Texture2D::getTextureSlots(), samplers);
        m_TextSDFShader->set("Texture", Texture2D::getTextureSlots(), samplers);

        GP_CORE_TRACE("Rendering::init() initializing texture Shader");
        m_TextureShader = CreateRef<Shader>(textureVertexShader, textureFragmentShader);
        m_TextureShader->set("Texture", Texture2D::getTextureSlots(), samplers);

        m_ShaderUniform = Ref<UniformBuffer>(new UniformBuffer({{ShaderDataType::Mat4, "PVMatrix"}}));
        m_ShaderUniform->setData(&m_Camera.m_ProjectionViewMatrix, 1);

        m_SolidShader->setUniformBlock(m_ShaderUniform, "Projection", 0);
        m_EllipseShader->setUniformBlock(m_ShaderUniform, "Projection", 0);
        m_TextureShader->setUniformBlock(m_ShaderUniform, "Projection", 0);
        m_TextBitmapShader->setUniformBlock(m_ShaderUniform, "Projection", 0);
        m_TextSDFShader->setUniformBlock(m_ShaderUniform, "Projection", 0);
    }

    Renderer::~Renderer() {
        delete m_AtlasMono;
        delete m_AtlasRGB;
        delete m_AtlasRGBA;
    }

    void Renderer::_createLineBuffer() {
        GP_CORE_TRACE("Renderer::_createLineBuffer()");

        auto VAO = Ref<VertexArray>(new VertexArray());
        auto VBO = Ref<VertexBuffer>(new VertexBuffer());

        VBO->setLayout({{ShaderDataType::Float2, "position"},
                        {ShaderDataType::Float4, "color"}});
        VAO->setVertexBuffer(VBO);

        m_LineBatch = {VAO, 2, 2};
    }

    void Renderer::_createTriangleBuffer() {
        GP_CORE_TRACE("Renderer::_createTriangleBuffer()");

        auto VAO = Ref<VertexArray>(new VertexArray());
        auto VBO = Ref<VertexBuffer>(new VertexBuffer());

        VBO->setLayout({{ShaderDataType::Float2, "position"},
                        {ShaderDataType::Float4, "color"}});
        VAO->setVertexBuffer(VBO);

        m_TriangleBatch = {VAO, 3, 3};
    }

    void Renderer::_createQuadBuffer() {
        GP_CORE_TRACE("Renderer::_createQuadBuffer()");

        auto VAO = Ref<VertexArray>(new VertexArray());
        auto VBO = Ref<VertexBuffer>(new VertexBuffer());

        VBO->setLayout({{ShaderDataType::Float2, "position"},
                        {ShaderDataType::Float4, "color"}});
        VAO->setVertexBuffer(VBO);

        m_QuadBatch = {VAO, 6, 4};
    }

    void Renderer::_createEllipseBuffer() {
        GP_CORE_TRACE("Renderer::_createEllipseBuffer()");

        auto VAO = Ref<VertexArray>(new VertexArray());
        auto VBO = Ref<VertexBuffer>(new VertexBuffer());

        VBO->setLayout({{ShaderDataType::Float2, "position"},
                        {ShaderDataType::Float2, "localCoord"},
                        {ShaderDataType::Float4, "color"}});
        VAO->setVertexBuffer(VBO);

        m_EllipseBatch = {VAO, 6, 4};
    }

    void Renderer::_createTexturedBuffer() {
        GP_CORE_TRACE("Renderer::_createTexturedBuffer() creating Textured buffer");

        BufferLayout layout = {{ShaderDataType::Float2, "position"},
                               {ShaderDataType::Float4, "color"},
                               {ShaderDataType::Float2, "texCoord"},
                               {ShaderDataType::Int,    "texSlot"},};

        auto VAO = Ref<VertexArray>(new VertexArray());
        auto VBO = Ref<VertexBuffer>(new VertexBuffer());
        VBO->setLayout(layout);
        VAO->setVertexBuffer(VBO);

        m_TextureBatchesMono.emplace_back(VAO, 6, 4);

        VAO = Ref<VertexArray>(new VertexArray());
        VBO = Ref<VertexBuffer>(new VertexBuffer());
        VBO->setLayout(layout);
        VAO->setVertexBuffer(VBO);

        m_TextureBatchesRGB.emplace_back(VAO, 6, 4);

        VAO = Ref<VertexArray>(new VertexArray());
        VBO = Ref<VertexBuffer>(new VertexBuffer());
        VBO->setLayout(layout);
        VAO->setVertexBuffer(VBO);

        m_TextureBatchesRGBA.emplace_back(VAO, 6, 4);
    }

    void Renderer::destroy(uint32_t ID) {
        m_ToBatch[ID]->destroyObject(ID);
    }

    uint32_t Renderer::drawLine(Line *object) {
        SolidVertex vertices[2] = {{object->m_Points[0], object->m_V1},
                                   {object->m_Points[1], object->m_V2}};

        if (m_LineBatch.contains(object->getID())) {
            m_LineBatch.updateObject(object->getID(), vertices, object->isHidden());
            return 0;
        }
        else {
            uint32_t ID = m_LineBatch.newObject(vertices, object->isHidden());
            m_ToBatch.insert({ID, &m_LineBatch});
            return ID;
        }
    }

    uint32_t Renderer::drawTriangle(Triangle *object) {
        SolidVertex vertices[3] = {{object->m_Points[0], object->m_V1},
                                   {object->m_Points[1], object->m_V2},
                                   {object->m_Points[2], object->m_V3}};

        if (m_TriangleBatch.contains(object->getID())) {
            m_TriangleBatch.updateObject(object->getID(), vertices, object->isHidden());
            return 0;
        }
        else {
            uint32_t ID = m_TriangleBatch.newObject(vertices, object->isHidden());
            m_ToBatch.insert({ID, &m_TriangleBatch});
            return ID;
        }
    }

    uint32_t Renderer::drawQuad(Quad *object) {
        SolidVertex vertices[4] = {{object->m_Points[0], object->m_V1},
                                   {object->m_Points[1], object->m_V2},
                                   {object->m_Points[2], object->m_V3},
                                   {object->m_Points[3], object->m_V4}};

        if (m_QuadBatch.contains(object->getID())) {
            m_QuadBatch.updateObject(object->getID(), vertices, object->isHidden());
            return 0;
        }
        else {
            uint32_t ID = m_QuadBatch.newObject(vertices, object->isHidden());
            m_ToBatch.insert({ID, &m_QuadBatch});
            return ID;
        }
    }

    uint32_t Renderer::drawEllipse(Ellipse *object) {
        EllipseVertex vertices[4] = {{object->m_Points[0], object->m_V1},
                                     {object->m_Points[1], object->m_V2},
                                     {object->m_Points[2], object->m_V3},
                                     {object->m_Points[3], object->m_V4}};

        if (m_EllipseBatch.contains(object->getID())) {
            m_EllipseBatch.updateObject(object->getID(), vertices, object->isHidden());
            return 0;
        }
        else {
            uint32_t ID = m_EllipseBatch.newObject(vertices, object->isHidden());
            m_ToBatch.insert({ID, &m_EllipseBatch});
            return ID;
        }
    }

    uint32_t Renderer::drawTexturedQuad(TexturedQuad *object) {
        if (m_TexturesCache.find(object->getTextureName()) == m_TexturesCache.end()) {
            GP_CORE_TRACE("gp::Renderer::drawTexturedQuad() - no cached texture '{0}'", object->getTextureName());
            _cacheTexture(object->getTextureName(), object->getBitmap());
        }

        TextureCoords texCoords = m_TexturesCache[object->getTextureName()];
        uint32_t texSlot = texCoords.page % Texture2D::getTextureSlots();
        uint32_t batchIndex = texCoords.page / Texture2D::getTextureSlots();

        object->m_T1.texSlot = texSlot;
        object->m_T2.texSlot = texSlot;
        object->m_T3.texSlot = texSlot;
        object->m_T4.texSlot = texSlot;

        object->m_T1.texCoord = {texCoords.p1.x, texCoords.p2.y};
        object->m_T2.texCoord = {texCoords.p2.x, texCoords.p2.y};
        object->m_T3.texCoord = {texCoords.p2.x, texCoords.p1.y};
        object->m_T4.texCoord = {texCoords.p1.x, texCoords.p1.y};

        TextureVertex vertices[4] = {{object->m_Points[0], object->m_V1, object->m_T1},
                                     {object->m_Points[1], object->m_V2, object->m_T2},
                                     {object->m_Points[2], object->m_V3, object->m_T3},
                                     {object->m_Points[3], object->m_V4, object->m_T4}};

        Batch<TextureVertex> *batch;
        switch (texCoords.texture->getChannels()) {
            case 1:
                batch = &m_TextureBatchesMono[batchIndex];
                break;

            case 3:
                batch = &m_TextureBatchesRGB[batchIndex];
                break;

            case 4:
                batch = &m_TextureBatchesRGBA[batchIndex];
                break;

            default:
                GP_VALUE_ERROR("Renderer::drawTexturedQuad() invalid channels ({0}) in bitmap",
                               texCoords.texture->getChannels());
                return -1;
        }

        if (batch->contains(object->getID())) {
            batch->updateObject(object->getID(), vertices, object->isHidden());
            return 0;
        }
        else {
            uint32_t ID = batch->newObject(vertices, object->isHidden());
            m_ToBatch.insert({ID, batch});
            return ID;
        }
    }

    uint32_t Renderer::drawGlyph(TexturedQuad *object) {
        //        uint32_t texIndex, texSlot;
        //        if (m_TexturesCache.find(object->getTextureName()) == m_TexturesCache.end()) {
        //            GP_CORE_TRACE("gp::Renderer::drawGlyph() - no cached texture '{0}'", object->getTextureName());
        //
        //            auto bitmap = object->getBitmap();
        //            texIndex = _cacheTexture(object->getTextureName(), bitmap);
        //            texSlot = texIndex % 16;
        //
        //            if (texSlot == 0) {
        //                _createTexturedBuffer();
        //            }
        //        }
        //        else {
        //            GP_CORE_TRACE("gp::Renderer::drawGlyph() - using cached texture '{0}'", object->getTextureName());
        //            texIndex = m_TexturesCache[object->getTextureName()].index;
        //            texSlot = texIndex % 16;
        //        }
        //
        //        GP_CORE_TRACE("gp::Renderer::drawGlyph() - texIndex={0}, texSlot={1}", texIndex, texSlot);
        //
        //        object->m_T1.texSlot = texSlot;
        //        object->m_T2.texSlot = texSlot;
        //        object->m_T3.texSlot = texSlot;
        //        object->m_T4.texSlot = texSlot;
        //
        //        uint32_t batch = texIndex / Texture2D::getTextureSlots();
        //
        //        TextureVertex vertices[4] = {{object->m_Points[0], object->m_V1, object->m_T1},
        //                                     {object->m_Points[1], object->m_V2, object->m_T2},
        //                                     {object->m_Points[2], object->m_V3, object->m_T3},
        //                                     {object->m_Points[3], object->m_V4, object->m_T4}};
        //
        //        if (m_GlyphBatches[batch].contains(object->getID())) {
        //            m_GlyphBatches[batch].updateObject(object->getID(), vertices, object->isHidden());
        //            return 0;
        //        }
        //        else {
        //            uint32_t ID = m_GlyphBatches[batch].newObject(vertices, object->isHidden());
        //            m_ToBatch.insert({ID, &m_GlyphBatches[batch]});
        //            return ID;
        //        }
    }

    void Renderer::flush() {
        GP_CORE_TRACE_ALL("gp::Renderer::flush()");

        m_ShaderUniform->setData(&m_Camera.m_ProjectionViewMatrix, 1, 0);

        if (!(m_LineBatch.empty() and m_TriangleBatch.empty() and m_QuadBatch.empty())) {
            m_SolidShader->bind();
        }

        m_LineBatch.draw();
        m_TriangleBatch.draw();
        m_QuadBatch.draw();

        if (!m_EllipseBatch.empty()) {
            m_EllipseShader->bind();
            m_EllipseBatch.draw();
        }

        m_TextureShader->bind();

        for (uint32_t i = 0; i < m_AtlasMono->getPages(); i += Texture2D::getTextureSlots()) {
            auto &batch = m_TextureBatchesMono[i / 16];
            if (!batch.empty()) {
                for (uint32_t j = i; j < min(m_AtlasMono->getPages(), i + 16); j++) {
                    m_AtlasMono->getTextureAt(j)->bind(j % 16);
                }

                batch.draw();
            }
        }

        for (uint32_t i = 0; i < m_AtlasRGB->getPages(); i += Texture2D::getTextureSlots()) {
            auto &batch = m_TextureBatchesRGB[i / 16];
            if (!batch.empty()) {
                for (uint32_t j = i; j < min(m_AtlasRGB->getPages(), i + 16); j++) {
                    m_AtlasRGB->getTextureAt(j)->bind(j % 16);
                }

                batch.draw();
            }
        }

        for (uint32_t i = 0; i < m_AtlasRGBA->getPages(); i += Texture2D::getTextureSlots()) {
            auto &batch = m_TextureBatchesRGBA[i / 16];
            if (!batch.empty()) {
                for (uint32_t j = i; j < min(m_AtlasRGBA->getPages(), i + 16); j++) {
                    m_AtlasRGBA->getTextureAt(j)->bind(j % 16);
                }

                batch.draw();
            }
        }
    }

    void Renderer::_cacheTexture(const std::string &name, const Ref<Bitmap> &bitmap) {
        GP_CORE_DEBUG("gp::Renderer::_cacheTexture('{0}')", name);

        TextureCoords texCoords;
        switch (bitmap->getChannels()) {
            case 1:  // Monochrome
                texCoords = m_AtlasMono->add(bitmap);
                m_AtlasMono->createTextureAtlas();
                break;

            case 3:  // RGB
                texCoords = m_AtlasRGB->add(bitmap);
                m_AtlasRGB->createTextureAtlas();
                break;

            case 4:  // RGBA
                texCoords = m_AtlasRGBA->add(bitmap);
                m_AtlasRGBA->createTextureAtlas();
                break;

            default:
                GP_VALUE_ERROR("Renderer::_cacheTexture() invalid channels ({0}) in bitmap", bitmap->getChannels());
        }

        m_TexturesCache.insert({name, texCoords});
        if (texCoords.page % Texture2D::getTextureSlots() == 0) {
            _createTexturedBuffer();
        }
    }
}
