#include "Renderer.h"

#include "src/goopylib/objects/Line.h"
#include "src/goopylib/objects/Triangle.h"
#include "src/goopylib/objects/Quad.h"
#include "src/goopylib/objects/Ellipse.h"
#include "src/goopylib/objects/Image.h"
#include "src/goopylib/shader/Shader.h"


#if !GP_LOG_RENDERER
#undef GP_LOGGING
#endif

#include "src/goopylib/debug/LogMacros.h"

#if !GP_VALUE_CHECK_RENDERER
#undef GP_VALUE_CHECKING
#endif

namespace gp {
    Renderer::Renderer(float width, float height)
            : m_Camera(-width / 2, width / 2, -height / 2, height / 2) {

    }

    Renderer::~Renderer() = default;

    void Renderer::init() {
        GP_CORE_INFO("Initializing Renderer");

        #if GP_USING_OPENGL
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        #endif

        GP_CORE_DEBUG("Initializing Line");
        Line::init();

        GP_CORE_TRACE("Initializing Line Shader");
        m_LineShader = CreateRef<Shader>(GP_DIRECTORY "goopylib/shader/line.vert",
                                         GP_DIRECTORY "goopylib/shader/line.frag");

        GP_CORE_TRACE("Initializing Polygon Shader");
        m_PolygonShader = CreateRef<Shader>(GP_DIRECTORY "goopylib/shader/vec2.vert",
                                            GP_DIRECTORY "goopylib/shader/solid.frag");

        GP_CORE_TRACE("Initializing Ellipse Shader");
        m_EllipseShader = CreateRef<Shader>(GP_DIRECTORY "goopylib/shader/ellipse.vert",
                                            GP_DIRECTORY "goopylib/shader/ellipse.frag");

        _createLineBuffer();
        _createTriangleBuffer();
        _createQuadBuffer();
        _createEllipseBuffer();

        GP_CORE_TRACE("Initializing Image Shader");
        m_ImageShader = CreateRef<Shader>(GP_DIRECTORY "goopylib/shader/image.vert",
                                          GP_DIRECTORY "goopylib/shader/image.frag");

        int32_t samplers[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8,
                                9, 10, 11, 12, 13, 14, 15};
        m_ImageShader->set("Texture", s_TextureSlots, samplers);


        m_ShaderUniform = Ref<UniformBuffer>(new UniformBuffer({{ShaderDataType::Mat4, "PVMatrix"}}));
        m_ShaderUniform->setData(&m_Camera.m_ProjectionViewMatrix, 1);

        m_LineShader->setUniformBlock(m_ShaderUniform, "Projection", 0);
        m_PolygonShader->setUniformBlock(m_ShaderUniform, "Projection", 0);
        m_EllipseShader->setUniformBlock(m_ShaderUniform, "Projection", 0);
        m_ImageShader->setUniformBlock(m_ShaderUniform, "Projection", 0);
    }

    void Renderer::_createLineBuffer() {
        GP_CORE_TRACE("Creating Line Buffers");

        auto lineVAO = Ref<VertexArray>(new VertexArray());
        auto lineVBO = Ref<VertexBuffer>(new VertexBuffer());

        lineVBO->setLayout({{ShaderDataType::Float2, "vertices"},
                            {ShaderDataType::Float4, "color"}});
        lineVAO->setVertexBuffer(lineVBO);

        m_RenderingObjects.emplace_back(lineVAO, nullptr, m_LineShader, GP_DRAW_MODE_LINES);
    }

    void Renderer::_createTriangleBuffer() {
        GP_CORE_TRACE("Creating Triangle Buffers");

        auto triangleVAO = Ref<VertexArray>(new VertexArray());
        auto triangleVBO = Ref<VertexBuffer>(new VertexBuffer());

        triangleVBO->setLayout({{ShaderDataType::Float2, "vertices"},
                                {ShaderDataType::Float4, "color"}});
        triangleVAO->setVertexBuffer(triangleVBO);

        m_RenderingObjects.emplace_back(triangleVAO, nullptr, m_PolygonShader);
    }

    void Renderer::_createQuadBuffer() {
        GP_CORE_TRACE("Creating Quad Buffers");

        auto quadVAO = Ref<VertexArray>(new VertexArray());
        auto quadVBO = Ref<VertexBuffer>(new VertexBuffer());

        quadVBO->setLayout({{ShaderDataType::Float2, "vertices"},
                            {ShaderDataType::Float4, "color"}});
        quadVAO->setVertexBuffer(quadVBO);

        m_RenderingObjects.emplace_back(quadVAO, nullptr, m_PolygonShader);
    }

    void Renderer::_createEllipseBuffer() {
        GP_CORE_TRACE("Creating Ellipse Buffers");

        auto ellipseVAO = Ref<VertexArray>(new VertexArray());
        auto ellipseVBO = Ref<VertexBuffer>(new VertexBuffer());

        ellipseVBO->setLayout({{ShaderDataType::Float2, "vertices"},
                               {ShaderDataType::Float2, "localCoord"},
                               {ShaderDataType::Float4, "color"}});
        ellipseVAO->setVertexBuffer(ellipseVBO);

        m_RenderingObjects.emplace_back(ellipseVAO, nullptr, m_EllipseShader);
    }

    void Renderer::_newImageBuffer() {
        GP_CORE_TRACE("Creating New Image Buffer");

        auto imageVAO = Ref<VertexArray>(new VertexArray());
        auto imageVBO = Ref<VertexBuffer>(new VertexBuffer());

        imageVBO->setLayout({{ShaderDataType::Float2, "vertices"},
                             {ShaderDataType::Float2, "texCoord"},
                             {ShaderDataType::Int, "texSlot"},
                             {ShaderDataType::Float, "transparency"}});
        imageVAO->setVertexBuffer(imageVBO);

        m_RenderingObjects.emplace_back(imageVAO, nullptr, m_ImageShader);
        m_ImageVertices.emplace_back();
        m_ImageIDs.emplace_back();
    }

    uint32_t Renderer::drawLine(Line *object) {
        uint32_t ID = m_NextLineID;
        m_NextLineID++;
        GP_CORE_DEBUG("Drawing Line {0}", ID);

        uint32_t index = m_LineVertices.size();
        m_LineIDs.insert({ID, index});

        m_LineVertices.push_back({object->m_Points[0], object->m_V1});
        m_LineVertices.push_back({object->m_Points[1], object->m_V2});

        if (object->isHidden()) {
            m_LineVertices[index + 0].attrib.color.alpha = 0;
            m_LineVertices[index + 1].attrib.color.alpha = 0;
        }

        m_RenderingObjects[LINES].indices += 2;
        m_RenderingObjects[LINES].vertices += 2;
        m_RenderingObjects[LINES].bufferData = &m_LineVertices[0];
        m_RenderingObjects[LINES].reallocateBufferData = true;

        return ID;
    }

    void Renderer::destroyLine(uint32_t ID) {
        uint32_t index = m_LineIDs.at(ID);

        m_LineVertices.erase(std::next(m_LineVertices.begin(), index),
                             std::next(m_LineVertices.begin(), index + 2));

        m_LineIDs.erase(ID);
        for (auto &i: m_LineIDs) {
            if (i.second > index) {
                i.second -= 2;
            }
        }

        m_RenderingObjects[LINES].indices -= 2;
        m_RenderingObjects[LINES].vertices -= 2;
        m_RenderingObjects[LINES].bufferData = &m_LineVertices[0];
        m_RenderingObjects[LINES].reallocateBufferData = true;
    }

    void Renderer::updateLine(uint32_t ID, const Line *object) {
        uint32_t index = m_LineIDs.at(ID);

        m_LineVertices[index + 0] = {object->m_Points[0], object->m_V1};
        m_LineVertices[index + 1] = {object->m_Points[1], object->m_V2};

        if (object->isHidden()) {
            m_LineVertices[index + 0].attrib.color.alpha = 0;
            m_LineVertices[index + 1].attrib.color.alpha = 0;
        }

        m_RenderingObjects[LINES].updateBufferData = true;
    }

    uint32_t Renderer::drawTriangle(Triangle *object) {
        uint32_t ID = m_NextTriangleID;
        m_NextTriangleID++;
        GP_CORE_DEBUG("Drawing Triangle {0}", ID);

        uint32_t index = m_TriangleVertices.size();
        m_TriangleIDs.insert({ID, index});

        m_TriangleVertices.push_back({object->m_Points[0], object->m_V1});
        m_TriangleVertices.push_back({object->m_Points[1], object->m_V2});
        m_TriangleVertices.push_back({object->m_Points[2], object->m_V3});

        if (object->isHidden()) {
            m_TriangleVertices[index + 0].attrib.color.alpha = 0;
            m_TriangleVertices[index + 1].attrib.color.alpha = 0;
            m_TriangleVertices[index + 2].attrib.color.alpha = 0;
        }

        m_RenderingObjects[TRIANGLES].indices += 3;
        m_RenderingObjects[TRIANGLES].vertices += 3;
        m_RenderingObjects[TRIANGLES].bufferData = &m_TriangleVertices[0];
        m_RenderingObjects[TRIANGLES].reallocateBufferData = true;

        return ID;
    }

    void Renderer::destroyTriangle(uint32_t ID) {
        uint32_t index = m_TriangleIDs.at(ID);

        m_TriangleVertices.erase(std::next(m_TriangleVertices.begin(), index),
                                 std::next(m_TriangleVertices.begin(), index + 3));

        m_TriangleIDs.erase(ID);
        for (auto &i: m_TriangleIDs) {
            if (i.second > index) {
                i.second -= 4;
            }
        }

        m_RenderingObjects[TRIANGLES].indices -= 3;
        m_RenderingObjects[TRIANGLES].vertices -= 3;
        m_RenderingObjects[TRIANGLES].bufferData = &m_TriangleVertices[0];
        m_RenderingObjects[TRIANGLES].reallocateBufferData = true;
    }

    void Renderer::updateTriangle(uint32_t ID, const Triangle *object) {
        uint32_t index = m_TriangleIDs.at(ID);

        m_TriangleVertices[index + 0] = {object->m_Points[0], object->m_V1};
        m_TriangleVertices[index + 1] = {object->m_Points[1], object->m_V2};
        m_TriangleVertices[index + 2] = {object->m_Points[2], object->m_V3};

        if (object->isHidden()) {
            m_TriangleVertices[index + 0].attrib.color.alpha = 0;
            m_TriangleVertices[index + 1].attrib.color.alpha = 0;
            m_TriangleVertices[index + 2].attrib.color.alpha = 0;
        }

        m_RenderingObjects[TRIANGLES].updateBufferData = true;
    }

    uint32_t Renderer::drawQuad(Quad *object) {
        uint32_t ID = m_NextQuadID;
        m_NextQuadID++;
        GP_CORE_DEBUG("Drawing Quad {0}", ID);

        uint32_t index = m_QuadVertices.size();
        m_QuadIDs.insert({ID, index});

        m_QuadVertices.push_back({object->m_Points[0], object->m_V1});
        m_QuadVertices.push_back({object->m_Points[1], object->m_V2});
        m_QuadVertices.push_back({object->m_Points[2], object->m_V3});
        m_QuadVertices.push_back({object->m_Points[3], object->m_V4});

        if (object->isHidden()) {
            m_QuadVertices[index + 0].attrib.color.alpha = 0;
            m_QuadVertices[index + 1].attrib.color.alpha = 0;
            m_QuadVertices[index + 2].attrib.color.alpha = 0;
            m_QuadVertices[index + 3].attrib.color.alpha = 0;
        }

        m_RenderingObjects[QUADS].indices += 6;
        m_RenderingObjects[QUADS].vertices += 4;
        m_RenderingObjects[QUADS].bufferData = &m_QuadVertices[0];
        m_RenderingObjects[QUADS].reallocateBufferData = true;

        return ID;
    }

    void Renderer::destroyQuad(uint32_t ID) {
        uint32_t index = m_QuadIDs.at(ID);

        m_QuadVertices.erase(std::next(m_QuadVertices.begin(), index),
                             std::next(m_QuadVertices.begin(), index + 4));

        m_QuadIDs.erase(ID);
        for (auto &i: m_QuadIDs) {
            if (i.second > index) {
                i.second -= 4;
            }
        }

        m_RenderingObjects[QUADS].indices -= 6;
        m_RenderingObjects[QUADS].vertices -= 4;
        m_RenderingObjects[QUADS].bufferData = &m_QuadVertices[0];
        m_RenderingObjects[QUADS].reallocateBufferData = true;
    }

    void Renderer::updateQuad(uint32_t ID, const Quad *object) {
        uint32_t index = m_QuadIDs.at(ID);

        m_QuadVertices[index + 0] = {object->m_Points[0], object->m_V1};
        m_QuadVertices[index + 1] = {object->m_Points[1], object->m_V2};
        m_QuadVertices[index + 2] = {object->m_Points[2], object->m_V3};
        m_QuadVertices[index + 3] = {object->m_Points[3], object->m_V4};

        if (object->isHidden()) {
            m_QuadVertices[index + 0].attrib.color.alpha = 0;
            m_QuadVertices[index + 1].attrib.color.alpha = 0;
            m_QuadVertices[index + 2].attrib.color.alpha = 0;
            m_QuadVertices[index + 3].attrib.color.alpha = 0;
        }


        m_RenderingObjects[QUADS].updateBufferData = true;
    }

    uint32_t Renderer::drawEllipse(Ellipse *object) {
        uint32_t ID = m_NextEllipseID;
        m_NextEllipseID++;
        GP_CORE_DEBUG("Drawing Ellipse {0}", ID);

        uint32_t index = m_EllipseVertices.size();
        m_EllipseIDs.insert({ID, index});

        m_EllipseVertices.push_back({object->m_Points[0], object->m_V1});
        m_EllipseVertices.push_back({object->m_Points[1], object->m_V2});
        m_EllipseVertices.push_back({object->m_Points[2], object->m_V3});
        m_EllipseVertices.push_back({object->m_Points[3], object->m_V4});

        if (object->isHidden()) {
            m_EllipseVertices[index + 0].attrib.color.alpha = 0;
            m_EllipseVertices[index + 1].attrib.color.alpha = 0;
            m_EllipseVertices[index + 2].attrib.color.alpha = 0;
            m_EllipseVertices[index + 3].attrib.color.alpha = 0;
        }

        m_RenderingObjects[ELLIPSES].indices += 6;
        m_RenderingObjects[ELLIPSES].vertices += 4;
        m_RenderingObjects[ELLIPSES].bufferData = &m_EllipseVertices[0];
        m_RenderingObjects[ELLIPSES].reallocateBufferData = true;

        return ID;
    }

    void Renderer::destroyEllipse(uint32_t ID) {
        uint32_t index = m_EllipseIDs.at(ID);

        m_EllipseVertices.erase(std::next(m_EllipseVertices.begin(), index),
                                std::next(m_EllipseVertices.begin(), index + 4));

        m_EllipseIDs.erase(ID);
        for (auto &i: m_EllipseIDs) {
            if (i.second > index) {
                i.second -= 4;
            }
        }

        m_RenderingObjects[ELLIPSES].indices -= 6;
        m_RenderingObjects[ELLIPSES].vertices -= 4;
        m_RenderingObjects[ELLIPSES].bufferData = &m_EllipseVertices[0];
        m_RenderingObjects[ELLIPSES].reallocateBufferData = true;
    }

    void Renderer::updateEllipse(uint32_t ID, const Ellipse *object) {
        uint32_t index = m_EllipseIDs.at(ID);

        m_EllipseVertices[index + 0] = {object->m_Points[0], object->m_V1};
        m_EllipseVertices[index + 1] = {object->m_Points[1], object->m_V2};
        m_EllipseVertices[index + 2] = {object->m_Points[2], object->m_V3};
        m_EllipseVertices[index + 3] = {object->m_Points[3], object->m_V4};

        if (object->isHidden()) {
            m_EllipseVertices[index + 0].attrib.color.alpha = 0;
            m_EllipseVertices[index + 1].attrib.color.alpha = 0;
            m_EllipseVertices[index + 2].attrib.color.alpha = 0;
            m_EllipseVertices[index + 3].attrib.color.alpha = 0;
        }

        m_RenderingObjects[ELLIPSES].updateBufferData = true;
    }

    uint32_t Renderer::drawImage(Image *object) {
        uint32_t ID = m_NextImageID;
        m_NextImageID++;
        GP_CORE_DEBUG("Drawing Image {0}", ID);

        uint32_t texIndex, texSlot;
        if (m_TexturesCache.find(object->m_Path) == m_TexturesCache.end()) {
            texIndex = _cacheTexture(object->m_Path);
            texSlot = texIndex % 16;

            if (texSlot == 0) {
                _newImageBuffer();
            }
        }
        else {
            texIndex = m_TexturesCache.at(object->m_Path).index;
            texSlot = texIndex % 16;
        }

        object->m_V1.texSlot = texSlot;
        object->m_V2.texSlot = texSlot;
        object->m_V3.texSlot = texSlot;
        object->m_V4.texSlot = texSlot;

        unsigned int batch = texIndex / s_TextureSlots;
        unsigned int i = IMAGES + batch;

        uint32_t index = m_ImageVertices[batch].size();
        m_ImageBatches.insert({ID, batch});
        m_ImageIDs[batch].insert({ID, index});

        m_ImageVertices[batch].push_back({object->m_Points[0], object->m_V1});
        m_ImageVertices[batch].push_back({object->m_Points[1], object->m_V2});
        m_ImageVertices[batch].push_back({object->m_Points[2], object->m_V3});
        m_ImageVertices[batch].push_back({object->m_Points[3], object->m_V4});

        if (object->isHidden()) {
            m_ImageVertices[batch][index + 0].attrib.transparency = 0;
            m_ImageVertices[batch][index + 1].attrib.transparency = 0;
            m_ImageVertices[batch][index + 2].attrib.transparency = 0;
            m_ImageVertices[batch][index + 3].attrib.transparency = 0;
        }

        m_RenderingObjects[i].indices += 6;
        m_RenderingObjects[i].vertices += 4;
        m_RenderingObjects[i].bufferData = &m_ImageVertices[batch][0];
        m_RenderingObjects[i].reallocateBufferData = true;

        return ID;
    }

    void Renderer::destroyImage(uint32_t ID) {
        uint32_t batch = m_ImageBatches.at(ID);
        auto &imageIDs = m_ImageIDs[batch];
        uint32_t index = imageIDs.at(ID);

        m_ImageVertices[batch].erase(std::next(m_ImageVertices[batch].begin(), index),
                                     std::next(m_ImageVertices[batch].begin(), index + 4));

        imageIDs.erase(ID);
        for (auto &i: imageIDs) {
            if (i.second > index) {
                i.second -= 4;
            }
        }

        m_RenderingObjects[IMAGES + batch].indices -= 6;
        m_RenderingObjects[IMAGES + batch].vertices -= 4;
        m_RenderingObjects[IMAGES + batch].bufferData = &m_ImageVertices[batch][0];
        m_RenderingObjects[IMAGES + batch].reallocateBufferData = true;
    }

    void Renderer::updateImage(uint32_t ID, const Image *object) {
        uint32_t batch = m_ImageBatches.at(ID);
        uint32_t index = m_ImageIDs[batch].at(ID);

        m_ImageVertices[batch][index + 0] = {object->m_Points[0], object->m_V1};
        m_ImageVertices[batch][index + 1] = {object->m_Points[1], object->m_V2};
        m_ImageVertices[batch][index + 2] = {object->m_Points[2], object->m_V3};
        m_ImageVertices[batch][index + 3] = {object->m_Points[3], object->m_V4};

        if (object->isHidden()) {
            m_ImageVertices[batch][index + 0].attrib.transparency = 0;
            m_ImageVertices[batch][index + 1].attrib.transparency = 0;
            m_ImageVertices[batch][index + 2].attrib.transparency = 0;
            m_ImageVertices[batch][index + 3].attrib.transparency = 0;
        }

        m_RenderingObjects[IMAGES + batch].updateBufferData = true;
    }

    void Renderer::flush() {
        GP_CORE_TRACE_ALL("gp::Renderer::flush()");

        m_ShaderUniform->setData(&m_Camera.m_ProjectionViewMatrix, 1, 0);

        _updateRenderingObjectVBO(m_RenderingObjects[LINES]);
        _updateRenderingObjectVBO(m_RenderingObjects[TRIANGLES]);

        for (int i = QUADS; i < m_RenderingObjects.size(); i++) {
            auto &object = m_RenderingObjects[i];

            _updateRenderingObjectEBO(object);
            _updateRenderingObjectVBO(object);
        }

        for (int i = 0; i < IMAGES; i++) {
            auto &object = m_RenderingObjects[i];

            if (object.indices) {
                object.shader->bind();
                object.VAO->draw(object.indices, object.mode);
            }
        }

        m_ImageShader->bind();

        for (int i = IMAGES; i < m_RenderingObjects.size(); i++) {
            auto &object = m_RenderingObjects[i];

            if (object.indices) {
                _bindTextureBatch(i - IMAGES);
                object.VAO->draw(object.indices, object.mode);
            }
        }
    }

    uint32_t Renderer::_cacheTexture(const char *path) {
        GP_CORE_DEBUG("Adding '{}' to Textures Cache", path);

        auto texture = Ref<Texture2D>(new Texture2D(path));
        uint32_t texIndex = m_Textures.size();

        m_TexturesCache.insert({path, {texture, texIndex}});
        m_Textures.push_back(texture);

        return texIndex;
    }

    void Renderer::_bindTextureBatch(uint32_t batch) {
        uint32_t textures = min((batch + 1) * s_TextureSlots, (uint32_t) m_Textures.size());

        for (uint32_t i = batch * s_TextureSlots; i < textures; i++) {
            m_Textures[i]->bind(i % s_TextureSlots);
        }
    }

    void Renderer::_updateRenderingObjectVBO(RenderingData &object) {
        if (object.reallocateBufferData) {
            object.VAO->m_VertexBuffer->setData(object.bufferData, object.vertices);
            object.reallocateBufferData = false;
            object.updateBufferData = false;
        }
        else if (object.updateBufferData) {
            object.VAO->m_VertexBuffer->setData(object.bufferData, object.vertices, 0);
            object.updateBufferData = false;
        }
    }

    void Renderer::_updateRenderingObjectEBO(RenderingData &object) {
        if (object.reallocateBufferData) {

            uint32_t *indices = new uint32_t[object.indices];

            for (uint32_t i = 0; i < object.vertices / 4; i++) {
                uint32_t indicesIndex = i * 6;
                uint32_t vertexIndex = i * 4;

                indices[indicesIndex + 0] = vertexIndex + 0;
                indices[indicesIndex + 1] = vertexIndex + 1;
                indices[indicesIndex + 2] = vertexIndex + 2;

                indices[indicesIndex + 3] = vertexIndex + 0;
                indices[indicesIndex + 4] = vertexIndex + 2;
                indices[indicesIndex + 5] = vertexIndex + 3;
            }
            object.VAO->setIndexBuffer(object.indices, indices);

            delete[] indices;
        }
    }
}
