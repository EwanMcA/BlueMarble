#pragma once

#include <string>
#include <glm/glm.hpp>

namespace BlueMarble {

    class Shader
    {
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        // Shader needs to be bound before calling this
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
    private:
        uint32_t oRendererID;
    };

} // namespace BlueMarble
