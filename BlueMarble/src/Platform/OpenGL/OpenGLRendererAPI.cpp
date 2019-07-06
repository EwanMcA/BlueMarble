#include "bmpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace BlueMarble {

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        // glm gives us these aliases
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
    {
        // TODO: A few of these params are just for proof of concept purposes
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

} // namespace BlueMarble