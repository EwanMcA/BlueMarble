#include "renderLayer.h"

using BlueMarble::MaterialComponent;
using BlueMarble::VertexArrayComponent;
using BlueMarble::TransformComponent;

void RenderLayer::OnUpdate(BlueMarble::TimeStep ts)
{
    BlueMarble::RenderCommand::SetClearColor({ 0.4f, 0.6f, 1.0f, 1 });
    BlueMarble::RenderCommand::Clear();

    BlueMarble::Renderer::BeginScene(*oCamera);

    for (BlueMarble::Ref<BlueMarble::Entity> entity : *oEntities)
    {
        if (entity->HasComponent<MaterialComponent>() &&
            entity->HasComponent<VertexArrayComponent>() &&
            entity->HasComponent<TransformComponent>())
        {
            BlueMarble::Renderer::Submit(entity->GetComponent<MaterialComponent>()->oMaterial, 
                                         entity->GetComponent<VertexArrayComponent>()->oVA, 
                                         entity->GetComponent<TransformComponent>()->oTransform);
        }
    }

    BlueMarble::Renderer::EndScene();
}
