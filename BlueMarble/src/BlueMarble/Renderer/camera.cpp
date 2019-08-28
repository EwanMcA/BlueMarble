#include "bmpch.h"
#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace BlueMarble {

    float quickLerp(const float f1, const float f2, const float ratio)
    {
        return f1 * (1.0f - ratio) + f2 * ratio;
    }

    /**********************************************************
        Orthographic Camera
    ***********************************************************/

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : Camera(glm::ortho(left, right, bottom, top, -1.0f, 1.0f), glm::mat4(1.0f))
    {

    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), oPosition) * 
            glm::rotate(glm::mat4(1.0f), glm::radians(oRotation), glm::vec3(0, 0, 1));

        oViewMatrix = glm::inverse(transform);
        oPVMatrix = oProjectionMatrix * oViewMatrix;
    }


    /**********************************************************
        Perspective Camera
    ***********************************************************/

    PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float zNear, float zFar)
        : Camera(glm::perspective(fovy, aspect, zNear, zFar), glm::mat4(1.0f))
    {

    }

    void PerspectiveCamera::RecalculateViewMatrix()
    {
        // TODO: Fix this hacky way to do the rotation
        glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(oRotation.x), glm::vec3(1, 0, 0)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(oRotation.y), glm::vec3(0, 1, 0)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(oRotation.z), glm::vec3(0, 0, 1));
        glm::vec3 up = rotationMat * glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f};
        glm::vec3 dir = rotationMat * glm::vec4{ 0.0f, 0.0f, -1.0f, 1.0f };

        oViewMatrix = glm::lookAt(oPosition, oPosition + dir, up);
        oPVMatrix = oProjectionMatrix * oViewMatrix;
    }

    glm::vec3 PerspectiveCamera::GetWorldCoords(float mouseX, float mouseY, float mouseZ) const
    {
        glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, mouseZ, 1.0f);
        glm::vec4 worldPos = (glm::inverse(oPVMatrix) * screenPos);
        worldPos /= worldPos.w;

        return worldPos;
    }

    /**********************************************************
    Game Camera
    ***********************************************************/

    GameCamera::GameCamera(float fovy, float aspect, float zNear, float zFar)
    : PerspectiveCamera(fovy, aspect, zNear, zFar)
    {
        RecalculateViewMatrix();
    }

    void GameCamera::RecalculateViewMatrix()
    {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), 
                                         glm::radians(quickLerp(0.0f, 70.0f, (oZoom * oZoom) / 10000.0f)), 
                                         glm::vec3(1, 0, 0));

        oPosition.z = quickLerp(10.0f, 0.5f, oZoom / 100.0f);
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), oPosition);

        oViewMatrix = glm::inverse(rotation) * glm::inverse(translation);
        oPVMatrix = oProjectionMatrix * oViewMatrix;
    }

    void GameCamera::Zoom(int change)
    {
        oZoom += change;
        oZoom = std::clamp(oZoom, 0, 100);
        RecalculateViewMatrix();
    }

    void GameCamera::Translate(const float x, const float y)
    {
        oPosition.x += x;
        oPosition.y += y;
        RecalculateViewMatrix();
    }

} // namespace BlueMarble