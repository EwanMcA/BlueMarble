#pragma once

#include <BlueMarble.h>
#include "components/components.h"

using BlueMarble::Ref;

class City : public BlueMarble::Entity 
{
public:
    City(uint32_t playerID, uint32_t initialPop, glm::vec3& position = glm::vec3(0))
    {
        auto va = BlueMarble::Ref<BlueMarble::VertexArray>(BlueMarble::VertexArray::Create());

        // TODO: Scale this the proper way.
        // TODO: Get a 3D mesh / texture for cities.
        float scale = 0.25f;
        std::vector<float> vertices =
        { scale * -0.5f, scale *  0.5f, 0.0f, 0.0f, 1.0f,
          scale * -0.5f, scale * -0.5f, 0.0f, 0.0f, 0.0f,
          scale *  0.5f, scale * -0.5f, 0.0f, 1.0f, 0.0f,
          scale *  0.5f, scale *  0.5f, 0.0f, 1.0f, 1.0f };

        Ref<BlueMarble::VertexBuffer> squareVB;
        squareVB.reset(BlueMarble::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float)));
        squareVB->SetLayout({ { BlueMarble::ShaderDataType::Float3, "aPosition" },
                              { BlueMarble::ShaderDataType::Float2, "aTexCoord" } });
        va->SetVertexBuffer(squareVB);
        std::vector<uint32_t> squareIndices = { 0, 1, 3, 3, 1, 2 };
        Ref<BlueMarble::IndexBuffer> squareIB;
        squareIB.reset(BlueMarble::IndexBuffer::Create(squareIndices.data(), squareIndices.size()));
        va->SetIndexBuffer(squareIB);
        SetComponent<BlueMarble::VertexArrayComponent>(std::make_shared<BlueMarble::VertexArrayComponent>(va));

        BlueMarble::Ref<BlueMarble::Shader> shader =
            BlueMarble::Ref<BlueMarble::Shader>(BlueMarble::Shader::Create("assets/shaders/basic.glsl"));
        BlueMarble::Ref<BlueMarble::Material> material =
            BlueMarble::Ref<BlueMarble::Material>(BlueMarble::Material::Create(shader));
        material->AddTexture2D(BlueMarble::Texture2D::Create("assets/textures/village.png"));

        SetComponent<BlueMarble::MaterialComponent>(std::make_shared<BlueMarble::MaterialComponent>(material));
        SetComponent<BlueMarble::TransformComponent>(std::make_shared<BlueMarble::TransformComponent>(position));
        SetComponent<PlayerComponent>(std::make_shared<PlayerComponent>(playerID));
        SetComponent<SizeComponent>(std::make_shared<SizeComponent>(5, 5)); // starting width & length
        SetComponent<MapPositionComponent>(std::make_shared<MapPositionComponent>(0, 0));
        SetComponent<PopulationComponent>(std::make_shared<PopulationComponent>(initialPop));
    }
    virtual ~City() = default;
};
