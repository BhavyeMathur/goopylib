#pragma once

#include "src/Platform/Independent/BaseShader.h"


namespace gp {
    class Shader final : public BaseShader {

        friend class Renderer;

    public:
        Shader(const char *vertexShaderPath, const char *fragmentShaderPath);

        ~Shader() override;

        static Ref<Shader> create(const char *vertexShaderPath, const char *fragmentShaderPath) {
            return Ref<Shader>(new Shader(vertexShaderPath, fragmentShaderPath));
        }

    private:
        void bind() const override;

        void unbind() const override;

    private:
        uint32_t m_RendererID = 0;

        void _setUniform(int32_t location, float value) const override;

        void _setUniform(int32_t location, float v1, float v2) const override;

        void _setUniform(int32_t location, float v1, float v2, float v3) const override;

        void _setUniform(int32_t location, float v1, float v2, float v3, float v4) const override;

        void _setUniform(int32_t location, double value) const override;

        void _setUniform(int32_t location, double v1, double v2) const override;

        void _setUniform(int32_t location, double v1, double v2, double v3) const override;

        void _setUniform(int32_t location, double v1, double v2, double v3, double v4) const override;

        void _setUniform(int32_t location, int32_t value) const override;

        void _setUniform(int32_t location, int32_t v1, int32_t v2) const override;

        void _setUniform(int32_t location, int32_t v1, int32_t v2, int32_t v3) const override;

        void _setUniform(int32_t location, int32_t v1, int32_t v2, int32_t v3, int32_t v4) const override;

        void _setUniform(int32_t location, uint32_t value) const override;

        void _setUniform(int32_t location, uint32_t v1, uint32_t v2) const override;

        void _setUniform(int32_t location, uint32_t v1, uint32_t v2, uint32_t v3) const override;

        void _setUniform(int32_t location, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4) const override;

        void _setUniform(int32_t location, const glm::mat2 &value, bool transpose) const override;

        void _setUniform(int32_t location, const glm::mat3 &value, bool transpose) const override;

        void _setUniform(int32_t location, const glm::mat4 &value, bool transpose) const override;

        void _setUniform(int32_t location, const glm::mat2x3 &value, bool transpose) const override;

        void _setUniform(int32_t location, const glm::mat3x2 &value, bool transpose) const override;

        void _setUniform(int32_t location, const glm::mat2x4 &value, bool transpose) const override;

        void _setUniform(int32_t location, const glm::mat4x2 &value, bool transpose) const override;

        void _setUniform(int32_t location, const glm::mat3x4 &value, bool transpose) const override;

        void _setUniform(int32_t location, const glm::mat4x3 &value, bool transpose) const override;

        int32_t _getUniform(const char *name) const override;
    };
}
