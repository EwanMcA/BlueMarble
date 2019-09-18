#pragma once

#include <memory>
#include <vector>
#include "BlueMarble/entity.h"
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

    class Terrain : public Entity
    {
    public:
        Terrain()
        : oXCount(0), oYCount(0), oSpacing(0) {}

        virtual ~Terrain() = default;

        void Init(const uint32_t xCount,
                  const uint32_t yCount,
                  const float spacing = 1.0f,
                  const glm::vec3& position = glm::vec3(0.0f));
        void Load(Ref<Material> material);

        // Getters
        float GetXWidth() const { return oXCount * oSpacing; }
        float GetYWidth() const { return oYCount * oSpacing; }
        float GetSpacing() const { return oSpacing; }
        uint32_t GetXCount() const { return oXCount; }
        uint32_t GetYCount() const { return oYCount; }
        float HeightAt(const uint32_t x, const uint32_t y) const { return oHeightScale * (*oHeightMap)[x + y * oXCount]; }
        float GetHeightScale() const { return oHeightScale; }
        void NormalAt(const uint32_t x, const uint32_t y, glm::vec3& normal) const;
        const glm::vec3& GetPosition() const { return oPosition; }
        float LayerGet(const int layerIx, const int x, const int y) const { return (*oDataLayers[layerIx])[x + y * oXCount]; }

        // Modifiers
        void GenerateVertices(std::vector<float>& vertices);
        void GenerateRandomHeightMap();
        void RefreshOverlay();
        void RefreshVertices() { RefreshVertices(-1, 0, 0, oXCount, oYCount); }
        void RefreshVertices(const int layerIx, const int xMin, const int yMin, const int xMax, const int yMax);
        void LayerAdd(const int layerIx, const int x, const int y, const float amount, const int radius);
        void LayerSet(const int layerIx, const int x, const int y, const float amount);
        void LayerSet(const int layerIx, const int x, const int y, const float amount, const int radius);
        void LayerSmooth(const int layerIx, const int x, const int y, const int radius);
        void LayerClear(const int layerIx) { oDataLayers[layerIx]->clear(); oDataLayers[layerIx]->resize(oXCount * oYCount, 0.0f); }
        void ResetHeightMap() { LayerClear(0); }
        void ResetHeightMap(BMPHeightMap& heightMap);
        void SetHeightScale(const float scale) { oHeightScale = scale; }
        void SetTexCoordCallback(const std::function<std::pair<float, float>(int, int)>& callback) 
        {
            oTexCoordCallback = callback;
        }
        void SetOverlayCallback(const std::function<float(int, int)>& callback)
        {
            oOverlayCallback = callback;
        }

        void AddDataLayer(Ref<std::vector<float>> layer) { oDataLayers.push_back(layer); }
        void LoadVB();
    private:

        // Counts = number of squares (also number of vertices in the heightmap)
        unsigned int oXCount;
        unsigned int oYCount;
        float oSpacing;
        float oHeightScale = 0.25f;
        glm::vec3 oPosition;

        Ref<std::vector<float>> oHeightMap;
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

        std::function<float(int, int)> oOverlayCallback =
            [](int x, int y) -> float {
            return 0.0f;
        };

        // Layers have a data point for each vertex, and are submitted to the shader in the vertex buffer
        std::vector<Ref<std::vector<float>>> oDataLayers;

        Ref<BlueMarble::VertexArray> oVA;
    };

} // namespace BlueMarble