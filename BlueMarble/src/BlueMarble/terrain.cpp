#include "bmpch.h"

#include "terrain.h"
#include "BlueMarble/Renderer/renderer.h"

#include "stb_image.h"
#include "glm/gtc/matrix_transform.hpp"

#define STRIDE 12

namespace BlueMarble {
    
    bool WithinRadius(const int x, const int y, const int i, const int j, const int radius)
    {
        float dx = abs(j - (int)x);
        float dy = abs(i - (int)y);
        return sqrt(dx * dx + dy * dy) < radius;
    }
    
    BMPHeightMap::BMPHeightMap(const std::string& path)
        : oFilePath(path), oLocalBuffer(nullptr), oWidth(0), oHeight(0), oBPP(0)
    {
        stbi_set_flip_vertically_on_load(1);
        oLocalBuffer = stbi_load(path.c_str(), &oWidth, &oHeight, &oBPP, 4);
    }

    void Terrain::Init(const uint32_t xCount,
                       const uint32_t yCount,
                       const float spacing,
                       const glm::vec3& position)
    {
        oXCount = xCount;
        oYCount = yCount;
        oSpacing = spacing;
        oPosition = position;

        SetComponent<BlueMarble::TransformComponent>(std::make_shared<TransformComponent>(position));

        oHeightMap = std::make_shared<std::vector<float>>(oXCount * oYCount, 0.0f);
        oDataLayers.push_back(oHeightMap);
    }

    void Terrain::ResetHeightMap(BMPHeightMap& heightMap)
    {
        for (int i = 0; i < oYCount && i < heightMap.oHeight; ++i)
        {
            for (int j = 0; j < oXCount && j < heightMap.oWidth; ++j)
            {
                int pixel = i * heightMap.oWidth + j;
                int vertex = i * oXCount + j;
                int height = heightMap.oLocalBuffer[pixel * 4];
                
                (*oHeightMap)[vertex] = (height / 255.0f);
            }
        }
    }

    void Terrain::GenerateRandomHeightMap()
    {
        if (oXCount == oYCount)
        {
            // TODO: Diamond Square Algorithm
            //       or noise functions?
        }
    }

    void Terrain::LoadVB()
    {
        // TODO: Expose layout, to keep terrain agnostic to the amount of layer data

        Ref<BlueMarble::VertexBuffer> squareVB;
        squareVB.reset(BlueMarble::VertexBuffer::Create(oVertices.data(), oVertices.size() * sizeof(float)));
        squareVB->SetLayout({ { BlueMarble::ShaderDataType::Float3, "aPosition" },
                              { BlueMarble::ShaderDataType::Float3, "aNormal"   },
                              { BlueMarble::ShaderDataType::Float2, "aTexCoord" },
                              { BlueMarble::ShaderDataType::Float3, "aStats" },
                              { BlueMarble::ShaderDataType::Float, "aOverlay" } });
        oVA->SetVertexBuffer(squareVB);
    }

    void Terrain::GenerateVertices(std::vector<float>& vertices)
    {
        vertices.clear();
        vertices.reserve(oXCount * oYCount * 5);
        for (int y = 0; y < oYCount; ++y)
        {
            for (int x = 0; x < oXCount; ++x)
            {
                glm::vec3 normal;
                NormalAt(x, y, normal);
                float xpos = x * oSpacing;
                float ypos = y * oSpacing;
                vertices.insert(vertices.end(), { xpos, ypos, HeightAt(x, y),
                                                  normal.x, normal.y, normal.z });

                float xTex{ 0.0f }, yTex{ 0.0f };
                if (oTexCoordCallback) {
                    std::tie(xTex, yTex) = oTexCoordCallback(x, y);
                }
                vertices.insert(vertices.end(), { xTex, yTex });

                for (int i = 1; i < oDataLayers.size(); ++i)
                    vertices.push_back((*oDataLayers[i])[x + y * oXCount]);

                // Overlay
                vertices.push_back(0.0f);
            }
        }
    }

    void Terrain::RefreshOverlay()
    {
        for (int y = 0; y < oYCount; ++y)
        {
            for (int x = 0; x < oXCount; ++x)
            {
                if (oOverlayCallback)
                {
                    int vbIndex = y * (oXCount * STRIDE) + (x * STRIDE);
                    oVertices[vbIndex + STRIDE - 1] = oOverlayCallback(x, y);
                }
            }
        }
     
        LoadVB();
    }

    void Terrain::RefreshVertices(int xMin, int yMin, int xMax, int yMax)
    {
        // Update heights and normals
        for (int y = yMin; y < yMax; ++y)
        {
            for (int x = xMin; x < xMax; ++x)
            {
                int vbIndex = y * (oXCount * STRIDE) + (x * STRIDE);
                glm::vec3 normal;
                NormalAt(x, y, normal);
                oVertices[vbIndex + 2] = HeightAt(x, y);
                oVertices[vbIndex + 3] = normal.x;
                oVertices[vbIndex + 4] = normal.y;
                oVertices[vbIndex + 5] = normal.z;

                if (oTexCoordCallback) 
                {
                    float xTex{ 0.0f }, yTex{ 0.0f };
                    std::tie(xTex, yTex) = oTexCoordCallback(x, y);
                    oVertices[vbIndex + 6] = xTex;
                    oVertices[vbIndex + 7] = yTex;
                }

                // oDataLayers[0] is the heightmap
                for (int i = 1; i < oDataLayers.size(); ++i)
                    oVertices[vbIndex + 7 + i] = (*oDataLayers[i])[x + y * oXCount];

                // Overlay
                if (oOverlayCallback)
                {
                    oVertices[vbIndex + 7 + oDataLayers.size()] = oOverlayCallback(x, y);
                }
            }
        }

        LoadVB();
    }

    void Terrain::LayerAdd(const int layerIx, const int x, const int y, const float amount, const int radius)
    {
        // loops form a square, so there is an additional check for circular radius
        for (int i = std::max(0, y - radius); i < y + radius && i < (int) oYCount; ++i)
        {
            for (int j = std::max(0, x - radius); j < x + radius && j < (int) oXCount; ++j)
            {
                if (WithinRadius(x, y, i, j, radius))
                {
                    (*oDataLayers[layerIx])[j + i * oXCount] += amount;
                    (*oDataLayers[layerIx])[j + i * oXCount] = std::clamp((*oDataLayers[layerIx])[j + i * oXCount], 0.0f, 1.0f);
                }
            }
        }

        RefreshVertices(std::max(0, x - radius), 
                        std::max(0, y - radius), 
                        std::min(x + radius, (int)oXCount), 
                        std::min(y + radius, (int)oYCount));
    }

    void Terrain::LayerSet(const int layerIx, const int x, const int y, const float amount, const int radius)
    {
        float setAmount = std::clamp(amount, 0.0f, 1.0f);

        // loops form a square, so there is an additional check for circular radius
        for (int i = std::max(0, y - radius); i < y + radius && i < (int)oYCount; ++i)
        {
            for (int j = std::max(0, x - radius); j < x + radius && j < (int)oXCount; ++j)
            {
                if (WithinRadius(x, y, i, j, radius))
                {
                    (*oDataLayers[layerIx])[j + i * oXCount] = setAmount;
                }
            }
        }

        RefreshVertices(std::max(0, x - radius),
            std::max(0, y - radius),
            std::min(x + radius, (int)oXCount),
            std::min(y + radius, (int)oYCount));
    }

    void Terrain::LayerSmooth(const int layerIx, const int x, const int y, const int radius)
    {
        for (int i = std::max(0, y - radius); i < y + radius && i < (int)oYCount; ++i)
        {
            for (int j = std::max(0, x - radius); j < x + radius && j < (int)oXCount; ++j)
            {
                if (WithinRadius(x, y, i, j, radius)) {
                    float sum{ 0 };
                    sum += (j > 0) ? (*oDataLayers[layerIx])[j - 1 + i * oXCount] : 0;
                    sum += (i > 0) ? (*oDataLayers[layerIx])[j + (i - 1) * oXCount] : 0;
                    sum += (j < oXCount - 1) ? (*oDataLayers[layerIx])[j + 1 + i * oXCount] : 0;
                    sum += (i < oYCount - 1) ? (*oDataLayers[layerIx])[j + (i + 1) * oXCount] : 0;
                    (*oDataLayers[layerIx])[j + i * oXCount] += 
                        ((sum / 4) - (*oDataLayers[layerIx])[j + i * oXCount]) * 0.1f;
                }
            }
        }

        RefreshVertices(std::max(0, x - radius),
            std::max(0, y - radius),
            std::min(x + radius, (int)oXCount),
            std::min(y + radius, (int)oYCount));
    }

    void Terrain::Load(Ref<Material> material)
    {
        oVA.reset(BlueMarble::VertexArray::Create());

        GenerateVertices(oVertices);
        LoadVB();

        std::vector<uint32_t> squareIndices;
        for (unsigned int y = 0; y < oYCount - 1; ++y)
        {
            for (unsigned int x = 0; x < (oXCount - 1); ++x)
            {
                unsigned int topLeft{ x + y * oXCount };
                unsigned int topRight{ topLeft + 1 };
                unsigned int botLeft{ topLeft + oXCount };
                unsigned int botRight{ botLeft + 1 };
                squareIndices.insert(squareIndices.end(), { topLeft, topRight, botRight, botRight, botLeft, topLeft });
            }
        }
        Ref<BlueMarble::IndexBuffer> squareIB;
        squareIB.reset(BlueMarble::IndexBuffer::Create(squareIndices.data(), squareIndices.size()));
        oVA->SetIndexBuffer(squareIB);

        SetComponent<BlueMarble::VertexArrayComponent>(std::make_shared<VertexArrayComponent>(oVA));
        SetComponent<BlueMarble::MaterialComponent>(std::make_shared<MaterialComponent>(material));
    }

    void Terrain::NormalAt(const unsigned int x, const unsigned int y, glm::vec3& normal) const
    {
        // Quick hack for surface normals

        if (x > 0 && x < oXCount - 1)
            normal.x = HeightAt(x - 1, y) - HeightAt(x + 1, y);
        else if (x > 0)
            normal.x = 2.0f * (HeightAt(x - 1, y) - HeightAt(x, y));
        else
            normal.x = 2.0f * (HeightAt(x, y) - HeightAt(x + 1, y));

        if (y > 0 && y < oYCount - 1)
            normal.y = HeightAt(x, y - 1) - HeightAt(x, y + 1);
        else if (y > 0)
            normal.y = 2 * (HeightAt(x, y - 1) - HeightAt(x, y));
        else
            normal.y = 2 * (HeightAt(x, y) - HeightAt(x, y + 1));

        normal.z = 2 * oSpacing;
        glm::normalize(normal);
    }

} // namespace BlueMarble