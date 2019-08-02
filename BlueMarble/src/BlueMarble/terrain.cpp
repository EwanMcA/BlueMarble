#include "bmpch.h"

#include "terrain.h"
#include "BlueMarble/Renderer/renderer.h"
#include "stb_image.h"

namespace BlueMarble {
    
    
    BMPHeightMap::BMPHeightMap(const std::string& path)
        : oFilePath(path), oLocalBuffer(nullptr), oWidth(0), oHeight(0), oBPP(0)
    {
        stbi_set_flip_vertically_on_load(1);
        oLocalBuffer = stbi_load(path.c_str(), &oWidth, &oHeight, &oBPP, 4);
    }

    void Terrain::ResetHeightMap(BMPHeightMap& heightMap)
    {
        //BM_CORE_INFO("{0}, {1}, {2}", heightMap.oWidth, heightMap.oHeight, heightMap.oBPP);
        //for (int i = 0; i < heightMap.oWidth * heightMap.oHeight; i += 4)
        //{
        //    //if ((uint8_t)heightMap.oLocalBuffer[i] != 255 && (uint8_t)heightMap.oLocalBuffer[i] != 0)
        //        BM_CORE_INFO("{0} {1} {2} {3}", heightMap.oLocalBuffer[i], heightMap.oLocalBuffer[i+1], heightMap.oLocalBuffer[i+2], heightMap.oLocalBuffer[i+3]);
        //}

        int xPPV = heightMap.oWidth / oXCount;
        int yPPV = heightMap.oHeight / oYCount;

        for (int i = 0; i < oXCount * oYCount; ++i)
        {
            int height = heightMap.oLocalBuffer[i * 4 * xPPV];
                
            oHeightMap[i] = oHeightScale * (height / 255.0f);
        }
    }

    void Terrain::GenerateRandomHeightMap()
    {
        if (oXCount == oYCount)
        {
            // TODO: Diamond Square Algorithm

            for (int i = 0; i < oHeightMap.size(); ++i) 
            {
                int x = i % oXCount;
                //int dx = abs(x - (int)oXCount/2);
                //int y = i / oXCount;
                //int dy = abs(y - (int)oYCount/2);
                //oHeightMap[i] = -1 * (1.0f/200.0f) * (dx*dx) - 1 * (1.0f / 200.0f) * (dy*dy) + 0.3f;
                //oHeightMap[i] = std::max(0.0f, oHeightMap[i]);
                switch (x)
                {
                    case 7: oHeightMap[i] = 0.05f; break;
                    case 6: oHeightMap[i] = 0.1f; break;
                    case 5: oHeightMap[i] = 0.25f; break;
                    case 4: oHeightMap[i] = 0.15f; break;
                    case 3: oHeightMap[i] = 0.1f; break;
                    case 2: oHeightMap[i] = 0.05f; break;
                }
            }
        }
    }

    void Terrain::RefreshVertices()
    {
        // TODO: refresh without reload
        Load();
    }

    void Terrain::AddHeight(const uint32_t x, const uint32_t y, const float amount, const uint32_t radius)
    {
        for (int i = y - radius; i < y + radius; ++i)
        {
            for (int j = x - radius; j < x + radius; ++j)
            {
                if (i < 0 || j < 0 || i > oYCount || j > oXCount)
                    continue;

                float dx = abs(j - (int)x);
                float dy = abs(i - (int)y);
                float dist = sqrt(dx*dx + dy*dy);
                if (dist < radius)
                    oHeightMap[j + i * oXCount] += amount;
            }
        }

        RefreshVertices();
    }

    void Terrain::Load()
    {
        oVA.reset(BlueMarble::VertexArray::Create());

        std::vector<float> vertices;
        vertices.reserve(oXCount * oYCount * 5);
        for (int y = 0; y < oYCount; ++y)
        {
            for (int x = 0; x < oXCount; ++x)
            {
                glm::vec3 normal;
                NormalAt(x, y, normal);
                vertices.insert(vertices.end(), { x * oSpacing, y * oSpacing, HeightAt(x, y), 
                                                  normal.x,     normal.y,     normal.z });
                if ((x % 2 == 0) && (y % 2 == 0))
                    vertices.insert(vertices.end(), { 0.5f, 0.5f });
                else if (y % 2 == 0)
                    vertices.insert(vertices.end(), { 1.0f, 0.5f });
                else if (x % 2 == 0)
                    vertices.insert(vertices.end(), { 0.5f, 1.0f });
                else
                    vertices.insert(vertices.end(), { 1.0f, 1.0f });
            }
        }

        std::shared_ptr<BlueMarble::VertexBuffer> squareVB;
        squareVB.reset(BlueMarble::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float)));
        squareVB->SetLayout({ { BlueMarble::ShaderDataType::Float3, "aPosition" },
                              { BlueMarble::ShaderDataType::Float3, "aNormal"   },
                              { BlueMarble::ShaderDataType::Float2, "aTexCoord" } });
        oVA->AddVertexBuffer(squareVB);

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
        std::shared_ptr<BlueMarble::IndexBuffer> squareIB;
        squareIB.reset(BlueMarble::IndexBuffer::Create(squareIndices.data(), squareIndices.size()));
        oVA->SetIndexBuffer(squareIB);

        oShader.reset(new BlueMarble::Shader("shaders/shader.glsl"));

        std::shared_ptr<BlueMarble::Texture> texture;
        texture.reset(BlueMarble::Texture::Create("water.png"));
        oTextures.push_back(texture);
        texture.reset(BlueMarble::Texture::Create("grass.png"));
        oTextures.push_back(texture);
        texture.reset(BlueMarble::Texture::Create("snow.png"));
        oTextures.push_back(texture);
    }

    void Terrain::Draw()
    {
        BlueMarble::Renderer::Submit(oShader, oVA, oTextures, glm::mat4(1.0f));
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