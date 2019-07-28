#pragma once

#include <glm/glm.hpp>

namespace BlueMarble {

    class OrthographicCamera 
    {
    public:
        // clipping planes -1 near, 1 far
        OrthographicCamera(float left, float right, float bottom, float top);

        const glm::vec3& GetPosition() const { return oPosition; }
        void SetPosition(const glm::vec3& position) { oPosition = position; RecalculateViewMatrix(); }

        const float GetRotation() const { return oRotation; }
        void SetRotation(float rotation) { oRotation = rotation; RecalculateViewMatrix(); }

        const glm::mat4 GetProjectionMatrix() const { return oProjectionMatrix; }
        const glm::mat4 GetViewMatrix() const { return oViewMatrix; }
        const glm::mat4 GetViewProjectionMatrix() const { return oPVMatrix; }
    private:
        void RecalculateViewMatrix();
    private:
        glm::mat4 oProjectionMatrix;
        glm::mat4 oViewMatrix;
        glm::mat4 oPVMatrix;

        glm::vec3 oPosition = { 0.0f, 0.0f, 0.0f };
        float oRotation = 0.0f;
    };

} // namespace BlueMarble