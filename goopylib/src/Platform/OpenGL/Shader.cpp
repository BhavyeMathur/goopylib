#include "gp.h"
#include "src/goopylib/Shader/Shader.h"
#include "src/goopylib/Core/Buffer.h"
#include <glm/gtc/type_ptr.hpp>

#if GP_USING_OPENGL

#include <OpenGL/gl3.h>

#endif

namespace gp {
    Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) {
        int32_t success;
        char infoLog[512];

        GP_CORE_DEBUG("Compiling Vertex Shader");

        auto vertexShaderString = readFile(vertexShaderPath);
        const char *vertexShaderSource = vertexShaderString.c_str();

        GP_CORE_TRACE("\n{0}", vertexShaderSource);

        uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            GP_CORE_ERROR("SHADER: Vertex Shader Compilation Failed {0}", infoLog);

            glDeleteShader(vertexShader);
        }

        GP_CORE_DEBUG("Compiling Fragment Shader");

        auto fragmentShaderString = readFile(fragmentShaderPath);
        const char *fragmentShaderSource = fragmentShaderString.c_str();

        GP_CORE_TRACE("\n{0}", fragmentShaderSource);

        uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            GP_CORE_ERROR("SHADER: Fragment Shader Compilation Failed {0}", infoLog);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        m_RendererID = glCreateProgram();

        GP_CORE_DEBUG("Linking Shader {0}", m_RendererID);

        glAttachShader(m_RendererID, vertexShader);
        glAttachShader(m_RendererID, fragmentShader);
        glLinkProgram(m_RendererID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            glGetProgramInfoLog(m_RendererID, 512, nullptr, infoLog);
            GP_CORE_ERROR("SHADER: Shader Linking Failed {0}\n", infoLog);
        }
    }

    Shader::~Shader() {
        GP_CORE_DEBUG("Deallocating Shader {0}", m_RendererID);
        glDeleteProgram(m_RendererID);
    }

    void Shader::bind() const {
        GP_CORE_TRACE_ALL("Binding Shader {0}", m_RendererID);
        glUseProgram(m_RendererID);
    }

    void Shader::unbind() {
        GP_CORE_WARN("Unbinding Shaders");
        glUseProgram(0);
    }

    void Shader::setUniformBlock(const Ref<UniformBuffer>& uniform, const char *name, uint32_t binding) const {
        uniform->setBinding(binding);

        uint32_t index = glGetUniformBlockIndex(m_RendererID, name);
        glUniformBlockBinding(m_RendererID, index, binding);
    }

    void Shader::_setUniform(int32_t location, float value) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}", m_RendererID, location, value);
        glUniform1f(location, value);
    }

    void Shader::_setUniform(int32_t location, float v1, float v2) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}, {3}", m_RendererID, location, v1, v2);
        glUniform2f(location, v1, v2);
    }

    void Shader::_setUniform(int32_t location, float v1, float v2, float v3) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}, {3}, {4}", m_RendererID, location, v1, v2, v3);
        glUniform3f(location, v1, v2, v3);
    }

    void Shader::_setUniform(int32_t location, float v1, float v2, float v3, float v4) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}, {3}, {4}, {5}", m_RendererID, location, v1, v2, v3,
                      v4);
        glUniform4f(location, v1, v2, v3, v4);
    }

    void Shader::_setUniform(int32_t location, double value) const {
        _setUniform(location, (float) value);
    }

    void Shader::_setUniform(int32_t location, double v1, double v2) const {
        _setUniform(location, (float) v1, (float) v2);
    }

    void Shader::_setUniform(int32_t location, double v1, double v2, double v3) const {
        _setUniform(location, (float) v1, (float) v2, (float) v3);
    }

    void Shader::_setUniform(int32_t location, double v1, double v2, double v3, double v4) const {
        _setUniform(location, (float) v1, (float) v2, (float) v3, (float) v4);
    }

    void Shader::_setUniform(int32_t location, int32_t value) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}", m_RendererID, location, value);
        glUniform1i(location, value);
    }

    void Shader::_setUniform(int32_t location, int32_t v1, int32_t v2) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}, {3}", m_RendererID, location, v1, v2);
        glUniform2i(location, v1, v2);
    }

    void Shader::_setUniform(int32_t location, int32_t v1, int32_t v2, int32_t v3) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}, {3}, {4}", m_RendererID, location, v1, v2, v3);
        glUniform3i(location, v1, v2, v3);
    }

    void Shader::_setUniform(int32_t location, int32_t v1, int32_t v2, int32_t v3, int32_t v4) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}, {3}, {4}, {5}", m_RendererID, location, v1, v2, v3,
                      v4);
        glUniform4i(location, v1, v2, v3, v4);
    }

    void Shader::_setUniform(int32_t location, uint32_t value) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}", m_RendererID, location, value);
        glUniform1ui(location, value);
    }

    void Shader::_setUniform(int32_t location, uint32_t v1, uint32_t v2) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}, {3}", m_RendererID, location, v1, v2);
        glUniform2ui(location, v1, v2);
    }

    void Shader::_setUniform(int32_t location, uint32_t v1, uint32_t v2, uint32_t v3) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}, {3}, {4}", m_RendererID, location, v1, v2, v3);
        glUniform3ui(location, v1, v2, v3);
    }

    void Shader::_setUniform(int32_t location, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}, {3}, {4}, {5}", m_RendererID, location, v1, v2, v3,
                      v4);
        glUniform4ui(location, v1, v2, v3, v4);
    }

    void Shader::_setUniform(int32_t location, const glm::mat2 &value, bool transpose) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}", m_RendererID, location, glm::to_string(value));
        glUniformMatrix2fv(location, 1, transpose, glm::value_ptr(value));
    }

    void Shader::_setUniform(int32_t location, const glm::mat3 &value, bool transpose) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}", m_RendererID, location, glm::to_string(value));
        glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(value));
    }

    void Shader::_setUniform(int32_t location, const glm::mat4 &value, bool transpose) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}", m_RendererID, location, glm::to_string(value));
        glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
    }

    void Shader::_setUniform(int32_t location, const glm::mat2x3 &value, bool transpose) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}", m_RendererID, location, glm::to_string(value));
        glUniformMatrix2x3fv(location, 1, transpose, glm::value_ptr(value));
    }

    void Shader::_setUniform(int32_t location, const glm::mat3x2 &value, bool transpose) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}", m_RendererID, location, glm::to_string(value));
        glUniformMatrix3x2fv(location, 1, transpose, glm::value_ptr(value));
    }

    void Shader::_setUniform(int32_t location, const glm::mat2x4 &value, bool transpose) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}", m_RendererID, location, glm::to_string(value));
        glUniformMatrix2x4fv(location, 1, transpose, glm::value_ptr(value));
    }

    void Shader::_setUniform(int32_t location, const glm::mat4x2 &value, bool transpose) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}", m_RendererID, location, glm::to_string(value));
        glUniformMatrix4x2fv(location, 1, transpose, glm::value_ptr(value));
    }

    void Shader::_setUniform(int32_t location, const glm::mat3x4 &value, bool transpose) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}", m_RendererID, location, glm::to_string(value));
        glUniformMatrix3x4fv(location, 1, transpose, glm::value_ptr(value));
    }

    void Shader::_setUniform(int32_t location, const glm::mat4x3 &value, bool transpose) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1} to {2}", m_RendererID, location, glm::to_string(value));
        glUniformMatrix4x3fv(location, 1, transpose, glm::value_ptr(value));
    }

    void Shader::_setUniform(int32_t location, int32_t count, float *value) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1}", m_RendererID, location);
        glUniform1fv(location, count, value);
    }

    void Shader::_setUniform(int32_t location, int32_t count, double *value) const {
        _setUniform(location, count, (float *) value);
    }

    void Shader::_setUniform(int32_t location, int32_t count, int32_t *value) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1}", m_RendererID, location);
        glUniform1iv(location, count, value);
    }

    void Shader::_setUniform(int32_t location, int32_t count, uint32_t *value) const {
        GP_CORE_TRACE("Shader {0} setting uniform at {1}", m_RendererID, location);
        glUniform1uiv(location, count, value);
    }

    int32_t Shader::_getUniform(const char *name) const {
        return glGetUniformLocation(m_RendererID, name);
    }
}
