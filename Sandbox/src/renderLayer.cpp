#include "renderLayer.h"

using BlueMarble::COMPONENT_TYPE;

void RenderLayer::OnUpdate(BlueMarble::TimeStep ts)
{
    BlueMarble::RenderCommand::SetClearColor({ 0.4f, 0.6f, 1.0f, 1 });
    BlueMarble::RenderCommand::Clear();

    BlueMarble::Renderer::BeginScene(*oCamera);

    for (BlueMarble::Ref<BlueMarble::Entity> entity : *oEntities)
    {
        long id{ entity->GetID() };
        if (id & (long)COMPONENT_TYPE::TRANSFORM &&
            id & (long)COMPONENT_TYPE::VERTEX_ARRAY &&
            id & (long)COMPONENT_TYPE::MATERIAL)
        {
            const BlueMarble::Ref<BlueMarble::Material> material =  
                    (entity->GetComponent<BlueMarble::MaterialComponent>(COMPONENT_TYPE::MATERIAL))->oMaterial;
            const BlueMarble::Ref<BlueMarble::VertexArray> va = 
                    (entity->GetComponent<BlueMarble::VertexArrayComponent>(COMPONENT_TYPE::VERTEX_ARRAY))->oVA;
            const glm::mat4 transform = 
                    (entity->GetComponent<BlueMarble::TransformComponent>(COMPONENT_TYPE::TRANSFORM))->oTransform;

            BlueMarble::Renderer::Submit(material, va, transform);
        }
    }

    BlueMarble::Renderer::EndScene();
}
