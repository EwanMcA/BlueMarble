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

        void ResetHeightMap() { oHeightMap.resize(oXCount * oYCount, 0.0f); }
        void ResetHeightMap(BMPHeightMap& heightMap);
        void GenerateRandomHeightMap();
        void RefreshVertices();
        void AddHeight(const uint32_t x, const uint32_t y, const float amount, const uint32_t radius);

        float HeightAt(const uint32_t x, const uint32_t y) const { return oHeightMap[x + y * oXCount]; }
        void NormalAt(const uint32_t x, const uint32_t y, glm::vec3& normal) const;

        void SetShader(std::shared_ptr<BlueMarble::Shader>& shader) { oShader = shader; }
        void AddTexture(std::shared_ptr<BlueMarble::Texture>& texture) { oTextures.push_back(texture); }
        void SetVA(std::shared_ptr<BlueMarble::VertexArray>& va) { oVA = va; }
    private:

        // Counts = number of squares (also number of vertices in the heightmap)
        unsigned int oXCount;
        unsigned int oYCount;
        float oSpacing;
        glm::vec3 oPosition;
        float oHeightScale = 0.25f;

        std::vector<float> oHeightMap;

        std::shared_ptr<BlueMarble::VertexArray> oVA;
        // TODO: Should we actually be storing the shader here? If so, multiple allowed?
        std::shared_ptr<BlueMarble::Shader> oShader;
        // TODO: multiple textures
        std::vector<std::shared_ptr<BlueMarble::Texture>> oTextures;

    };

} // namespace BlueMarble