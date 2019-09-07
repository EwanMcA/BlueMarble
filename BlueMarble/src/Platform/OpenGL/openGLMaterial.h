#pragma once

#include <map>

#include "BlueMarble/Renderer/material.h"
#include "Platform/OpenGL/openGLShader.h"
#include "Platform/OpenGL/openGLTexture.h"

namespace BlueMarble {

    class OpenGLMaterial : public Material
    {
    public:
        OpenGLMaterial(Ref<OpenGLShader> shader) 
            : oShader(shader) {}

        virtual ~OpenGLMaterial() = default;

        virtual void UploadUniforms() override;
        virtual void UploadTextures() override;

        virtual void SetInt(const std::string& name, const int value) override
            { oInts[name] = value; }
        virtual void SetFloat(const std::string& name, const float value) override
            { oFloats[name] = value; }
        virtual void SetFloat2(const std::string& name, const glm::vec2& values) override
            { oFloat2s[name] = values; }
        virtual void SetFloat3(const std::string& name, const glm::vec3& values) override
            { oFloat3s[name] = values; }
        virtual void SetFloat4(const std::string& name, const glm::vec4& values) override
            { oFloat4s[name] = values; }
        virtual void SetMat3(const std::string& name, const glm::mat3& matrix) override
            { oMat3s[name] = matrix; }
        virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override
            { oMat4s[name] = matrix; }

        virtual void AddTexture2D(Ref<Texture2D> texture) override
            { oTextures.push_back(std::dynamic_pointer_cast<OpenGLTexture2D>(texture)); }

        virtual const int GetInt(const std::string& name) const override { return oInts.find(name)->second; }
        virtual const float GetFloat(const std::string& name) const override { return oFloats.find(name)->second; }
        virtual const glm::vec2& GetFloat2(const std::string& name) const override { return oFloat2s.find(name)->second; }
        virtual const glm::vec3& GetFloat3(const std::string& name) const override { return oFloat3s.find(name)->second; }
        virtual const glm::vec4& GetFloat4(const std::string& name) const override { return oFloat4s.find(name)->second; }
        virtual const glm::mat3& GetMat3(const std::string& name) const override { return oMat3s.find(name)->second; }
        virtual const glm::mat4& GetMat4(const std::string& name) const override { return oMat4s.find(name)->second; }

    private:
        Ref<OpenGLShader> oShader;
        std::vector<Ref<OpenGLTexture2D>> oTextures;

        std::map<std::string, int> oInts;
        std::map<std::string, float> oFloats;
        std::map<std::string, glm::vec2> oFloat2s;
        std::map<std::string, glm::vec3> oFloat3s;
        std::map<std::string, glm::vec4> oFloat4s;
        std::map<std::string, glm::mat3> oMat3s;
        std::map<std::string, glm::mat4> oMat4s;
    };

}
