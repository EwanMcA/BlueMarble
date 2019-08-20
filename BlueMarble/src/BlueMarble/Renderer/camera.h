#pragma once

#include <glm/glm.hpp>

namespace BlueMarble {

    class Camera 
    {
    public:
        Camera(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
            : oProjectionMatrix(projectionMatrix), oViewMatrix(viewMatrix) 
        {
            oPVMatrix = oProjectionMatrix * oViewMatrix;
        }

        const glm::vec3& GetPosition() const { return oPosition; }
        void SetPosition(const glm::vec3& position) { oPosition = position; RecalculateViewMatrix(); }

        const glm::mat4 GetProjectionMatrix() const { return oProjectionMatrix; }
        const glm::mat4 GetViewMatrix() const { return oViewMatrix; }
        const glm::mat4 GetViewProjectionMatrix() const { return oPVMatrix; }

    protected:
        virtual void RecalculateViewMatrix() = 0;

    protected:
        glm::mat4 oProjectionMatrix;
        glm::mat4 oViewMatrix;
        glm::mat4 oPVMatrix;

        glm::vec3 oPosition = { 0.0f, 0.0f, 0.0f };
    };

    class OrthographicCamera : public Camera
    {
    public:
        // clipping planes -1 near, 1 far
        OrthographicCamera(float left, float right, float bottom, float top);

        const float GetRotation() const { return oRotation; }
        void SetRotation(float rotation) { oRotation = rotation; RecalculateViewMatrix(); }

    protected:
        virtual void RecalculateViewMatrix() override;

    private:
        float oRotation = 0.0f;

    };

    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(float fovy, float aspect, float near, float far);

        const glm::vec3 GetRotation() const { return oRotation; }
        
        // mouseX & mouseY in normalized [-1, 1] coords
        glm::vec3 CreateRay(float mouseX, float mouseY, float mouseZ) const;

        void SetRotation(glm::vec3 rotation) { oRotation = rotation; RecalculateViewMatrix(); }

    protected:
        virtual void RecalculateViewMatrix() override;

    private:
        //TODO Need to do this with quarternions in future
        glm::vec3 oRotation = { 0.0f, 0.0f, 0.0f };
    };

} // namespace BlueMarble