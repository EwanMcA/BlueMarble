#include "territorySystem.h"
#include "components/components.h"

#include <queue>
#include <set>
#include <vector>

#define SECONDS_PER_TILE 0.25

void TerritorySystem::OnUpdate(BlueMarble::TimeStep ts, std::vector<Ref<Entity>>& entities)
{
    // TODO: Better way of accessing terrain
    auto terrain = std::dynamic_pointer_cast<BlueMarble::Terrain>(entities[0]);

    // How this works:
    // Array is treated as a graph, for the purpose of a uniform cost search outward from the city (every x seconds).
    // If runs into opponent territory, checks war status - terrain combat ensues (or just stop, if in peacetime)
    // If runs into empty territory within the given pop radius, converts it and ends.

    for (BlueMarble::Ref<BlueMarble::Entity> entity : entities)
    {
        if (entity->HasComponent<PopulationComponent>() &&
            entity->GetComponent<PopulationComponent>()->oDirty)
        {
            auto posComp = entity->GetComponent<MapPositionComponent>();
            if (!entity->HasComponent<PlacingComponent>())
            {
                if (entity->HasComponent<TerritoryComponent>())
                {
                    BlueMarble::TimeStep& timer = entity->GetComponent<TerritoryComponent>()->oTimer += ts;
                    if (timer.GetSeconds() > SECONDS_PER_TILE)
                    {
                        //terrain->LayerSet(3, posComp->oX, posComp->oY, 1.0f, GetRadius(entity));
                        Grow(terrain, posComp->oX, posComp->oY, GetRadius(entity));
                        timer = BlueMarble::TimeStep(0);
                    }
                }
            }
        }
    }
}

int TerritorySystem::GetRadius(Ref<Entity> entity) const
{
    return entity->GetComponent<PopulationComponent>()->oPopulation / 10;
}

void TerritorySystem::Grow(Ref<BlueMarble::Terrain> terrain, int x, int y, int r)
{
    auto comp = [&terrain, x, y](std::pair<int, int> a, std::pair<int, int> b) -> bool
    {
        float ar = abs(a.first - x) * abs(a.first - x) + abs(a.second - y) * abs(a.second - y);
        float br = abs(b.first - x) * abs(b.first - x) + abs(b.second - y) * abs(b.second - y);
        return ar > br;
    };
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(comp)> q(comp);
    
    std::unordered_set<int> visited;
    q.push({x, y});

    while (!q.empty())
    {

        auto [nx, ny] = q.top();
        q.pop();

        if (visited.find(nx + ny * terrain->GetXCount()) != visited.end())
            continue;
        else
            visited.insert(nx + ny * terrain->GetXCount());
        
        // TODO: Get rid of magic layer index 3, define enum somewhere, or specific territory get method
        // TODO: Handle different players' territory.
        if (terrain->LayerGet(3, nx, ny) == 1.0f)
        {
            for (int i = -1; i < 2; i += 2)
            {
                int nextX = nx + i;
                if (nextX >= 0 && nextX < terrain->GetXCount() && 
                    abs(nextX - x) * abs(nextX - x) + abs(ny - y) * abs(ny - y) <= (r * r))
                {
                    q.push({ nextX, ny });
                }
            }
            for (int i = -1; i < 2; i += 2)
            {
                int nextY = ny + i;
                if (nextY >= 0 && nextY < terrain->GetYCount() &&
                    (abs(nx - x) * abs(nx - x) + abs(nextY - y) * abs(nextY - y) <= r))
                {
                    q.push({ nx, nextY });
                }
            }
        }
        else if (terrain->LayerGet(3, nx, ny) == 0.0f)
        {
            // TODO: Handle different players' territory.
            terrain->LayerSet(3, nx, ny, 1.0f);
            break;
        }
    }
}

