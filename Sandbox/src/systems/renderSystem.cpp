#include "renderSystem.h"

using BlueMarble::MaterialComponent;
using BlueMarble::VertexArrayComponent;
using BlueMarble::TransformComponent;

void RenderSystem::OnUpdate(BlueMarble::TimeStep ts, std::vector<Ref<Entity>>& entities)
{
    BlueMarble::RenderCommand::SetClearColor({ 0.4f, 0.6f, 1.0f, 1 });
    BlueMarble::RenderCommand::Clear();

    BlueMarble::Renderer::BeginScene(*oCamera);

    for (BlueMarble::Ref<BlueMarble::Entity> entity : entities)
    {
        if (entity->HasComponent<MaterialComponent>() &&
            entity->HasComponent<VertexArrayComponent>() &&
            entity->HasComponent<TransformComponent>())
        {
            auto transformComp = entity->GetComponent<TransformComponent>();
            if (transformComp->oTrackMouse) 
            {
                glm::vec3& worldCoords = oCamera->GetWorldCoords(BlueMarble::Input::GetNormMouseX(),
                                                                 BlueMarble::Input::GetNormMouseY(),
                                                                 BlueMarble::Input::GetNormMouseZ());

                BlueMarble::Renderer::Submit(entity->GetComponent<MaterialComponent>()->oMaterial, 
                                             entity->GetComponent<VertexArrayComponent>()->oVA, 
                                             glm::translate(transformComp->oTransform,
                                                 { worldCoords.x, worldCoords.y, worldCoords.z + 0.05f }));
            }
            else
            {
                BlueMarble::Renderer::Submit(entity->GetComponent<MaterialComponent>()->oMaterial,
                                             entity->GetComponent<VertexArrayComponent>()->oVA,
                                             transformComp->oTransform);
            }
        }
    }

    BlueMarble::Renderer::EndScene();
}
