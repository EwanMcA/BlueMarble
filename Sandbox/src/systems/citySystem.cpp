#include "citySystem.h"
#include "components/components.h"

using BlueMarble::TransformComponent;
using BlueMarble::MaterialComponent;

void CitySystem::OnUpdate(BlueMarble::TimeStep ts, std::vector<Ref<Entity>>& entities)
{
    for (BlueMarble::Ref<BlueMarble::Entity> entity : entities)
    {
        if (entity->HasComponent<PlacingComponent>() &&
            entity->HasComponent<PlayerInputComponent>() &&
            entity->HasComponent<MaterialComponent>() &&
            entity->HasComponent<TransformComponent>())
        {
            bool& placing = entity->GetComponent<PlacingComponent>()->oPlacing;
            auto pInputComp = entity->GetComponent<PlayerInputComponent>();
            auto materialComp = entity->GetComponent<MaterialComponent>();

            if (placing && CanPlace(pInputComp->mousePosition))
            {
                materialComp->oMaterial->SetBool("uCanPlace", true);
                
                if (pInputComp->leftClick)
                {
                    placing = false;
                    entity->GetComponent<TransformComponent>()->oTrackMouse = false;
                    entity->GetComponent<TransformComponent>()->oTransform = 
                        glm::translate(glm::mat4(1.0), pInputComp->mousePosition);
                }
            }
            else if (placing)
            {
                materialComp->oMaterial->SetBool("uCanPlace", false);
            }
        }
    }
}

bool CitySystem::CanPlace(glm::vec3& position)
{
    // TODO: Flesh out placement conditions
    return position.z > 0.1;
}