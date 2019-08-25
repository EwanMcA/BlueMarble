#include "bmpch.h"

#include "terrain.h"
#include "BlueMarble/Renderer/renderer.h"
// TODO: Remove this, to make terrain API agnostic
#include "Platform/OpenGL/openGLShader.h"

#include "stb_image.h"
#include "glm/gtc/matrix_transform.hpp"

namespace BlueMarble {
    
    
    BMPHeightMap::BMPHeightMap(const std::string& path)
        : oFilePath(path), oLocalBuffer(nullptr), oWidth(0), oHeight(0), oBPP(0)
    {
        stbi_set_flip_vertically_on_load(1);
        oLocalBuffer = stbi_load(path.c_str(), &oWidth, &oHeight, &oBPP, 4);
    }

    void Terrain::ResetHeightMap(BMPHeightMap& heightMap)
    {
        int xPPV = heightMap.oWidth / oXCount;
        int yPPV = heightMap.oHeight / oYCount;

        for (int i = 0; i < oXCount * oYCount; ++i)
        {
            int height = heightMap.oLocalBuffer[i * 4 * xPPV];
                
            oHeightMap[i] = (height / 255.0f);
        }
    }

    void Terrain::GenerateRandomHeightMap()
    {
        if (oXCount == oYCount)
        {
            // TODO: Diamond Square Algorithm
        }
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
    }

    void Terrain::RefreshVertices()
    {
        // TODO: Check for performance increase by modifying buffer, rather than recreating.

        std::vector<float> vertices;
        GenerateVertices(vertices);

        Ref<BlueMarble::VertexBuffer> squareVB;
        squareVB.reset(BlueMarble::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float)));
        squareVB->SetLayout({ { BlueMarble::ShaderDataType::Float3, "aPosition" },
                              { BlueMarble::ShaderDataType::Float3, "aNormal"   },
                              { BlueMarble::ShaderDataType::Float2, "aTexCoord" } });
        oVA->SetVertexBuffer(squareVB);
    }

    void Terrain::AddHeight(const int x, const int y, const float amount, const int radius)
    {
        for (int i = y - radius; i < y + radius && i < (int) oYCount; ++i)
        {
            for (int j = x - radius; j < x + radius && j < (int) oXCount; ++j)
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
        GenerateVertices(vertices);

        Ref<BlueMarble::VertexBuffer> squareVB;
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
        Ref<BlueMarble::IndexBuffer> squareIB;
        squareIB.reset(BlueMarble::IndexBuffer::Create(squareIndices.data(), squareIndices.size()));
        oVA->SetIndexBuffer(squareIB);

        oShader.reset(BlueMarble::Shader::Create("shaders/terrain.glsl"));

        Ref<BlueMarble::Texture2D> texture;
        texture = BlueMarble::Texture2D::Create("assets/textures/water.png");
        oTextures.push_back(texture);
        texture = BlueMarble::Texture2D::Create("assets/textures/sand.png");
        oTextures.push_back(texture);
        texture = BlueMarble::Texture2D::Create("assets/textures/grass.png");
        oTextures.push_back(texture);
        texture = BlueMarble::Texture2D::Create("assets/textures/snow.png");
        oTextures.push_back(texture);
    }

    void Terrain::Draw()
    {
        BlueMarble::Renderer::Submit(oShader, oVA, oTextures, glm::mat4(1.0f));
    }

    void Terrain::Draw(glm::vec4 textureCutoffs)
    {
        oShader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(oShader)->UploadUniformFloat4("uTextureCutoffs", textureCutoffs);
        BlueMarble::Renderer::Submit(oShader, oVA, oTextures, glm::translate(glm::mat4(1.0f), oPosition));
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