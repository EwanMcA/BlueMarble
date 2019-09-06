#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BlueMarble/Renderer/material.h"
#include "BlueMarble/Renderer/vertexArray.h"

namespace BlueMarble {

    // Enables the layers/systems to use a bit mask to identify relevant entities
    enum class COMPONENT_TYPE {
        TRANSFORM       = 1 << 0,
        VELOCITY        = 1 << 1,
        MESH            = 1 << 2,
        LAYER           = 1 << 3,
        VERTEX_ARRAY    = 1 << 4,
        MATERIAL        = 1 << 5
    };

    class Component
    {
    public:
        virtual ~Component() = default;
    };

    class TransformComponent : public Component
    {
    public:
        TransformComponent(const glm::mat4& transform)
            : oTransform(transform) {}

        TransformComponent(const glm::vec3& position)
            : oTransform(glm::translate(glm::mat4(1.0f), position)) {}

        glm::mat4 oTransform;
    };

    class VelocityComponent : public Component
    {
    public:
        glm::vec3 oVelocity;
    };

    class MeshComponent : public Component
    {
    public:
        std::vector<float[6]> oMesh;
    };

    // Data for each vertex, apart from positions / normals.
    class LayerComponent : public Component
    {
    public:

    };

    class VertexArrayComponent : public Component
    {
    public:
        VertexArrayComponent(Ref<BlueMarble::VertexArray> va)
            : oVA(va) {}

        Ref<BlueMarble::VertexArray> oVA;
    };

    class MaterialComponent : public Component
    {
    public:
        MaterialComponent(Ref<BlueMarble::Material> material)
            : oMaterial(material) {}

        Ref<Material> oMaterial;
    };
}
