#pragma once

#include <map>

#include "component.h"

namespace BlueMarble {

    class Entity
    {
    public:
        Entity(long id = 0) : oID(id) {}
        virtual ~Entity() = default;

        long GetID() { return oID; }

        template <typename T>
        void SetComponent(Ref<Component> component) { oComponents[GetTypeID<T>()] = component; }
        template <typename T>
        void RemoveComponent() { oComponents.erase(GetTypeID<T>()); }

        template<typename T>
        Ref<T> GetComponent() 
        { 
            return std::static_pointer_cast<T>(oComponents[GetTypeID<T>()]);
        }

        template<typename T>
        bool HasComponent() { return oComponents.find(GetTypeID<T>()) != oComponents.end(); }

        std::map<uint32_t, Ref<Component>>::iterator begin() { return oComponents.begin(); }
        std::map<uint32_t, Ref<Component>>::iterator end() { return oComponents.end(); }
        std::map<uint32_t, Ref<Component>>::const_iterator begin() const { return oComponents.begin(); }
        std::map<uint32_t, Ref<Component>>::const_iterator end() const { return oComponents.end(); }
    private:
        std::map<uint32_t, Ref<Component>> oComponents;

        long oID{ 0 };
    };

}
