#include "citySystem.h"
#include "components/components.h"

using BlueMarble::TransformComponent;
using BlueMarble::MaterialComponent;

void CitySystem::OnUpdate(BlueMarble::TimeStep ts, std::vector<Ref<Entity>>& entities)
{
    for (BlueMarble::Ref<BlueMarble::Entity> entity : entities)
    {
        if (entity->HasComponent<PlayerInputComponent>() &&
            entity->HasComponent<MaterialComponent>() &&
            entity->HasComponent<PopulationComponent>() &&
            entity->HasComponent<TransformComponent>() &&
            entity->HasComponent<MapPositionComponent>())
        {
            auto pInputComp = entity->GetComponent<PlayerInputComponent>();
            auto materialComp = entity->GetComponent<MaterialComponent>();
            
            // TODO: Set up a better way of accessing terrain
            Ref<BlueMarble::Terrain> terrain = std::dynamic_pointer_cast<BlueMarble::Terrain>(entities[0]);

            if (entity->HasComponent<PlacingComponent>())
            {
                auto posComp = entity->GetComponent<MapPositionComponent>();
                float xRatio = (pInputComp->mousePosition.x - terrain->GetPosition().x) / terrain->GetXWidth();
                float yRatio = (pInputComp->mousePosition.y - terrain->GetPosition().y) / terrain->GetYWidth();

                if (0.0f <= xRatio && xRatio <= 1.0f && 0.0f <= yRatio && yRatio <= 1.0f)
                {
                    posComp->oX = (int)(xRatio * terrain->GetXCount());
                    posComp->oY = (int)(yRatio * terrain->GetYCount());
                }

                if (CanPlace(terrain, pInputComp->mousePosition))
                {
                    materialComp->oMaterial->SetBool("uCanPlace", true);

                    if (pInputComp->leftClick)
                    {
                        PlaceCity(entity, pInputComp->mousePosition);
                        // TODO: Derive starting radius properly (in sync with territory system)
                        terrain->LayerSet(3, posComp->oX, posComp->oY, 1.0f, 10);
                    }
                }
                else
                {
                    materialComp->oMaterial->SetBool("uCanPlace", false);
                }
            }
            else // not placing
            {
                auto popComp = entity->GetComponent<PopulationComponent>();
                popComp->oPopulation += 1;
                if (popComp->oPopulation % 10 == 0)
                    popComp->oDirty = true;
            }
        }
    }
}

bool CitySystem::CanPlace(BlueMarble::Ref<Entity> terrain, glm::vec3& position)
{
    // TODO: Flesh out placement conditions
    return position.z > terrain->GetComponent<MaterialComponent>()->oMaterial->GetFloat4("uTextureCutoffs").y + 0.05f;
}

void CitySystem::PlaceCity(BlueMarble::Ref<Entity> city, glm::vec3& position)
{
    city->RemoveComponent<PlacingComponent>();
    city->GetComponent<TransformComponent>()->oTrackMouse = false;
    city->SetComponent<TerritoryComponent>(std::make_shared<TerritoryComponent>());

    // TODO: This position isn't locked to map vertices (so may appear slightly out of place)
    city->GetComponent<TransformComponent>()->oTransform = glm::translate(glm::mat4(1.0), position);
}
