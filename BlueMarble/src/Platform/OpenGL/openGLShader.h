#pragma once

#include "BlueMarble/Renderer/shader.h"
#include "glm/glm.hpp"

// TODO: Remove
typedef unsigned int GLenum;

namespace BlueMarble {

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader();

        void CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc);

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
        uint32_t GetUniformLocation(const std::string& name) const;

    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    private:
        uint32_t oRendererID;
        mutable std::unordered_map<std::string, uint32_t> oUniformLocationCache;
    };

} // namespace BlueMarble
