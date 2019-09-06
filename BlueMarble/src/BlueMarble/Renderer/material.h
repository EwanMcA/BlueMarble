#pragma once

#include "shader.h"
#include "texture.h"

#include <string>
#include "glm/glm.hpp"

namespace BlueMarble {

    class Material 
    {
    public:
        virtual ~Material() = default;
        static Material* Create(Ref<Shader> shader);

        virtual void UploadUniforms() = 0;
        virtual void UploadTextures() = 0;

        virtual void SetInt(const std::string& name, const int value) = 0;
        virtual void SetFloat(const std::string& name, const float value) = 0;
        virtual void SetFloat2(const std::string& name, const glm::vec2& values) = 0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& values) = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& values) = 0;
        virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;

        virtual void AddTexture2D(Ref<Texture2D> texture) = 0;
    };

}
