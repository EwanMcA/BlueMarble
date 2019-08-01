#pragma once

#include <memory>
#include <vector>
#include "BlueMarble/Renderer/shader.h"
#include "BlueMarble/Renderer/vertexArray.h"
#include "BlueMarble/Renderer/texture.h"

namespace BlueMarble {

    class Terrain
    {
    public:
        Terrain()
        : oXCount(0), oYCount(0), oSpacing(0), oPosition(glm::vec3(0.0f)) {}

        void Init(const unsigned int xCount,
                  const unsigned int yCount,
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
        void GenerateRandomHeightMap();

        float HeightAt(const unsigned int x, const unsigned int y) const { return oHeightMap[x + y * oXCount]; }
        void NormalAt(const unsigned int x, const unsigned int y, glm::vec3& normal) const;

        void SetShader(std::shared_ptr<BlueMarble::Shader>& shader) { oShader = shader; }
        void SetTexture(std::shared_ptr<BlueMarble::Texture>& texture) { oTexture = texture; }
        void SetVA(std::shared_ptr<BlueMarble::VertexArray>& va) { oVA = va; }
    private:

        // Counts = number of squares (also number of vertices in the heightmap)
        unsigned int oXCount;
        unsigned int oYCount;
        float oSpacing;
        glm::vec3 oPosition;

        std::vector<float> oHeightMap;

        std::shared_ptr<BlueMarble::VertexArray> oVA;
        // TODO: Should we actually be storing the shader here? If so, multiple allowed?
        std::shared_ptr<BlueMarble::Shader> oShader;
        // TODO: multiple textures
        std::shared_ptr<BlueMarble::Texture> oTexture;

    };

} // namespace BlueMarble