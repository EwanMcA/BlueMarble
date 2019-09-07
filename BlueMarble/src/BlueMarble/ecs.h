#pragma once

#include "Core/timeStep.h"
#include "entity.h"

#include <vector>

namespace BlueMarble {

    class System
    {
    public:
        System() = default;
        virtual ~System() = default;

        virtual void OnUpdate(TimeStep ts, std::vector<Ref<Entity>>& entities) = 0;
    };

    class EntityComponentSystem
    {
    public:
        EntityComponentSystem() {}

        void AddSystem(System* system);
        void RemoveSystem(System* system);

        void Update(TimeStep ts, std::vector<Ref<Entity>>& entities);
    private:
        std::vector<System*> oSystems;
    };

}