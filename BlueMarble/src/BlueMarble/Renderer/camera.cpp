#include "bmpch.h"
#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace BlueMarble {

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
        // This is rotation-order dependent, should replace with quarternions in future
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), oPosition) *
            glm::rotate(glm::mat4(1.0f), glm::radians(oRotation.x), glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(oRotation.y), glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(oRotation.z), glm::vec3(0, 0, 1));

        oViewMatrix = glm::inverse(transform);
        oPVMatrix = oProjectionMatrix * oViewMatrix;
    }
} // namespace BlueMarble