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
                  const glm::vec3& position = glm::vec3(0.0f));
        void Load();
        void Draw(const std::vector<Ref<BlueMarble::Texture2D>>& textures, glm::vec4 textureCutoffs);

        // Getters
        float GetXWidth() const { return oXCount * oSpacing; }
        float GetYWidth() const { return oYCount * oSpacing; }
        float GetSpacing() const { return oSpacing; }
        uint32_t GetXCount() const { return oXCount; }
        uint32_t GetYCount() const { return oYCount; }
        const glm::vec3& getPosition() const { return oPosition; }
        float HeightAt(const uint32_t x, const uint32_t y) const { return oHeightScale * oHeightMap[x + y * oXCount]; }
        void NormalAt(const uint32_t x, const uint32_t y, glm::vec3& normal) const;

        // Modifiers
        void GenerateVertices(std::vector<float>& vertices);
        void ResetHeightMap() { oHeightMap.clear(); oHeightMap.resize(oXCount * oYCount, 0.0f); }
        void ResetHeightMap(BMPHeightMap& heightMap);
        void GenerateRandomHeightMap();
        void RefreshVertices() { RefreshVertices(0, 0, oXCount, oYCount); }
        void RefreshVertices(int xMin, int yMin, int xMax, int yMax);
        void AddHeight(const int x, const int y, const float amount, const int radius);
        void SetHeightScale(const float scale) { oHeightScale = scale; }
        void SetTexCoordCallback(const std::function<std::pair<float, float>(int, int)>& callback) 
        {
            oTexCoordCallback = callback;
        }
        void SetVertexStatsCallback(const std::function<std::tuple<float, float>(int, int)>& callback)
        {
            oVertexStatsCallback = callback;
        }

        void SetShader(Ref<BlueMarble::Shader>& shader) { oShader = shader; }
        void LoadVB();
    private:

        // Counts = number of squares (also number of vertices in the heightmap)
        unsigned int oXCount;
        unsigned int oYCount;
        float oSpacing;
        glm::vec3 oPosition;
        float oHeightScale = 0.25f;

        std::vector<float> oHeightMap;
        std::vector<float> oVertices;

        // TODO: Generalise this callback system so that everything beyond positions/normals comes from the 
        //       user, and not with the stock terrain object.
        //       Thus only the game code / shader would know about the tex coords and vertex stats.
        //       - The difficulty lies in modifying the BufferLayout.

        // (x, y) -> (2d texture coords)  by default, function returns unit square coords.
        std::function<std::pair<float, float>(int, int)> oTexCoordCallback =
            [](int x, int y) -> std::pair<float, float> {
            float xTex = (x % 2 == 0) ? 0.0f : 1.0f;
            float yTex = (y % 2 == 0) ? 0.0f : 1.0f;
            return { xTex, yTex };
        };

        // (x, y) -> (stat1, stat2)
        std::function<std::tuple<float, float>(int, int)> oVertexStatsCallback;

        Ref<BlueMarble::VertexArray> oVA;
        // TODO: Should we actually be storing the shader here? If so, multiple allowed?
        Ref<BlueMarble::Shader> oShader;

    };

} // namespace BlueMarble