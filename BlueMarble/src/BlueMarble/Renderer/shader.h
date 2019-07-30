#pragma once

#include <string>
#include <glm/glm.hpp>

namespace BlueMarble {

    class Shader
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

        Shader(const Shader::ShaderSource& src);
        Shader(const std::string& filename)
            : Shader(Shader::ParseShader(filename)) {}
        ~Shader();

        void Bind() const;
        void Unbind() const;

        // Shader needs to be bound before calling this
        void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    private:
        static ShaderSource Shader::ParseShader(const std::string& filepath);

    private:
        uint32_t oRendererID;
    };

} // namespace BlueMarble
