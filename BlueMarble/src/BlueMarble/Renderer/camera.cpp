#include "bmpch.h"
#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace BlueMarble {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : oProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), oViewMatrix(1.0f)
    {
        oPVMatrix = oProjectionMatrix * oViewMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), oPosition) * 
            glm::rotate(glm::mat4(1.0f), glm::radians(oRotation), glm::vec3(0, 0, 1));

        oViewMatrix = glm::inverse(transform);
        oPVMatrix = oProjectionMatrix * oViewMatrix;
    }

} // namespace BlueMarble