#pragma once

#include <atomic>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BlueMarble/Renderer/material.h"
#include "BlueMarble/Renderer/vertexArray.h"

namespace BlueMarble {
    
    extern std::atomic<uint32_t> TypeIdCounter;

    template <typename T>
    int GetTypeID() {
        static int uint32_t = ++TypeIdCounter;
        return uint32_t;
    }

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

        virtual ~TransformComponent() = default;

        glm::mat4 oTransform;
    };

    class VelocityComponent : public Component
    {
    public:
        virtual ~VelocityComponent() = default;

        glm::vec3 oVelocity;
    };

    class MeshComponent : public Component
    {
    public:
        virtual ~MeshComponent() = default;

        std::vector<float[6]> oMesh;
    };

    class VertexArrayComponent : public Component
    {
    public:
        VertexArrayComponent(Ref<BlueMarble::VertexArray> va)
            : oVA(va) {}

        virtual ~VertexArrayComponent() = default;

        Ref<BlueMarble::VertexArray> oVA;
    };

    class MaterialComponent : public Component
    {
    public:
        MaterialComponent(Ref<BlueMarble::Material> material)
            : oMaterial(material) {}

        virtual ~MaterialComponent() = default;

        Ref<Material> oMaterial;
    };
}
