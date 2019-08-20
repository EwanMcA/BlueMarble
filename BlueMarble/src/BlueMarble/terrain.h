#pragma once

#include <memory>
#include <vector>
#include "BlueMarble/Renderer/shader.h"
#include "BlueMarble/Renderer/vertexArray.h"
#include "BlueMarble/Renderer/texture.h"

namespace BlueMarble {

    class BMPHeightMap
    {
    public:
        BMPHeightMap(const std::string& path);

        std::string oFilePath;
        unsigned char* oLocalBuffer;
        int oWidth, oHeight, oBPP;
    };

    class Terrain
    {
    public:
        Terrain()
        : oXCount(0), oYCount(0), oSpacing(0), oPosition(glm::vec3(0.0f)) {}

        void Init(const uint32_t xCount,
                  const uint32_t yCount,
                  const float spacing = 1.0f,
                  const glm::vec3& position = glm::vec3(0.0f))
        {
            oXCount = xCount;
            oYCount = yCount;
            oPosition = position;
            oSpacing = spacing;

            oHeightMap.resize(oXCount * oYCount, 0.0f);
        }

        void Load();
        void Draw();
        void Draw(glm::vec4 textureCutoffs);

        // Getters
        float GetXWidth() const { return oXCount * oSpacing; }
        float GetYWidth() const { return oYCount * oSpacing; }
        float GetSpacing() const { return oSpacing; }
        uint32_t GetXCount() const { return oXCount; }
        uint32_t GetYCount() const { return oYCount; }
        const glm::vec3& getPosition() const { return oPosition; }

        float HeightAt(const uint32_t x, const uint32_t y) const { return oHeightScale * oHeightMap[x + y * oXCount]; }
        void NormalAt(const uint32_t x, const uint32_t y, glm::vec3& normal) const;

        void GenerateVertices(std::vector<float>& vertices);

        // Modifiers
        void ResetHeightMap() { oHeightMap.resize(oXCount * oYCount, 0.0f); }
        void ResetHeightMap(BMPHeightMap& heightMap);
        void GenerateRandomHeightMap();
        void RefreshVertices();
        void AddHeight(const int x, const int y, const float amount, const int radius);

        void SetHeightScale(const float scale) { oHeightScale = scale; }
        void SetShader(Ref<BlueMarble::Shader>& shader) { oShader = shader; }
        void AddTexture(Ref<BlueMarble::Texture2D>& texture) { oTextures.push_back(texture); }
        void SetVA(Ref<BlueMarble::VertexArray>& va) { oVA = va; }
    private:

        // Counts = number of squares (also number of vertices in the heightmap)
        unsigned int oXCount;
        unsigned int oYCount;
        float oSpacing;
        glm::vec3 oPosition;
        float oHeightScale = 0.25f;

        std::vector<float> oHeightMap;

        Ref<BlueMarble::VertexArray> oVA;
        // TODO: Should we actually be storing the shader here? If so, multiple allowed?
        Ref<BlueMarble::Shader> oShader;
        // TODO: multiple textures
        std::vector<Ref<BlueMarble::Texture2D>> oTextures;

    };

} // namespace BlueMarble