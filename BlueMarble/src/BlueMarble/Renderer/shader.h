#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

// TODO: Abstract out the OpenGL calls to OpenGLShader class.
//       Retain this class as RendererAPI agnostic shader api.

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
        void UploadUniformInt(const std::string& name, const int value);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    private:
        static ShaderSource Shader::ParseShader(const std::string& filepath);

        uint32_t GetUniformLocation(const std::string& name) const;

    private:
        uint32_t oRendererID;
        mutable std::unordered_map<std::string, uint32_t> oUniformLocationCache;
    };

} // namespace BlueMarble
