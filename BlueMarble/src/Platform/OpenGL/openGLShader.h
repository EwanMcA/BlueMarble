#pragma once

#include "BlueMarble/Renderer/shader.h"
#include "glm/glm.hpp"

namespace BlueMarble {

    class OpenGLShader : public Shader
    {
    public:
        // The Shader object will have both of these (they are just for parsing)
        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        struct ShaderSource
        {
            std::string vertexSrc;
            std::string fragmentSrc;
        };

        OpenGLShader(const OpenGLShader::ShaderSource& src);
        OpenGLShader(const std::string& filename)
            : OpenGLShader(OpenGLShader::ParseShader(filename)) {}
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        // Shader needs to be bound before calling this
        void UploadUniformInt(const std::string& name, const int value);
        void UploadUniformFloat(const std::string& name, const float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    private:
        static ShaderSource OpenGLShader::ParseShader(const std::string& filepath);
        uint32_t GetUniformLocation(const std::string& name) const;

    private:
        uint32_t oRendererID;
        mutable std::unordered_map<std::string, uint32_t> oUniformLocationCache;
    };

} // namespace BlueMarble
