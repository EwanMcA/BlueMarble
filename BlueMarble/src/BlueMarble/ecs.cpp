#include "bmpch.h"
#include "ecs.h"

namespace BlueMarble {
    

    void EntityComponentSystem::Update(TimeStep ts)
    {
        for (System* system : oSystems)
        {
            system->OnUpdate(ts, oEntities);
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

    void EntityComponentSystem::RemoveEntity(Ref<Entity> entity)
    {
        auto it = std::find(oEntities.begin(), oEntities.end(), entity);
        if (it != oEntities.end())
            oEntities.erase(it);
    }

}
