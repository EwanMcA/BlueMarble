#include "bmpch.h"
#include "ecs.h"

namespace BlueMarble {
    

    void EntityComponentSystem::Update(TimeStep ts, std::vector<Ref<Entity>>& entities)
    {
        for (System* system : oSystems)
        {
            system->OnUpdate(ts, entities);
        }
    }

    void EntityComponentSystem::AddSystem(System* system)
    {
        oSystems.push_back(system);
    }

    void EntityComponentSystem::RemoveSystem(System* system)
    {
        auto it = std::find(oSystems.begin(), oSystems.end(), system);
        if (it != oSystems.end())
            oSystems.erase(it);
    }
}