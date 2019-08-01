#include "bmpch.h"

#include "terrain.h"
#include "BlueMarble/Renderer/renderer.h"

namespace BlueMarble {
    
    void Terrain::GenerateRandomHeightMap()
    {
        if (oXCount == oYCount)
        {
            // TODO: Diamond Square Algorithm

            for (int i = 0; i < oHeightMap.size(); ++i) 
            {
                int x = i % oXCount;
                int dx = abs(x - (int)oXCount/2);
                int y = i / oXCount;
                int dy = abs(y - (int)oYCount/2);
                oHeightMap[i] = -1 * (1.0f/200.0f) * (dx*dx) - 1 * (1.0f / 200.0f) * (dy*dy) + 0.3f;
                oHeightMap[i] = std::max(0.0f, oHeightMap[i]);
            }
        }
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
                vertices.insert(vertices.end(), { x * oSpacing, y * oSpacing, oHeightMap[(x + y * oXCount)], 
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

        oTexture.reset(BlueMarble::Texture::Create("grass.png"));
    }

    void Terrain::Draw()
    {
        BlueMarble::Renderer::Submit(oShader, oVA, oTexture, glm::mat4(1.0f));
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