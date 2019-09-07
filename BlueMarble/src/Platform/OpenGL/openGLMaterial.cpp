#include "bmpch.h"
#include "openGLMaterial.h"

namespace BlueMarble {

    void OpenGLMaterial::UploadUniforms()
    {
        oShader->Bind();

        for (auto kv : oBools)
            oShader->UploadUniformBool(kv.first, kv.second);

        for (auto kv : oInts)
            oShader->UploadUniformInt(kv.first, kv.second);

        for (auto kv : oFloats)
            oShader->UploadUniformFloat(kv.first, kv.second);
        
        for (auto kv : oFloat2s)
            oShader->UploadUniformFloat2(kv.first, kv.second);
        
        for (auto kv : oFloat3s)
            oShader->UploadUniformFloat3(kv.first, kv.second);
        
        for (auto kv : oFloat4s)
            oShader->UploadUniformFloat4(kv.first, kv.second);
        
        for (auto kv : oMat3s)
            oShader->UploadUniformMat3(kv.first, kv.second);

        for (auto kv : oMat4s)
            oShader->UploadUniformMat4(kv.first, kv.second);
    }

    void OpenGLMaterial::UploadTextures()
    {
        oShader->Bind();
        for (int i = 0; i < oTextures.size(); ++i) {
            oTextures[i]->Bind(i);
            std::string name{ "uTexture" };
            name += '0' + i;
            oShader->UploadUniformInt(name, i);
        }
    }
}