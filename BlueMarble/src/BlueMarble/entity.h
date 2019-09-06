#pragma once

#include <map>

#include "component.h"

namespace BlueMarble {

    class Entity
    {
    public:
        Entity() = default;
        virtual ~Entity() = default;

        long GetID() { return oID; }

        template <COMPONENT_TYPE T>
        void SetComponent(Ref<Component> component) { oComponents[T] = component; oID = (long) T | oID; }
        template <COMPONENT_TYPE T>
        void RemoveComponent(Ref<Component> component) { oComponents.erase(T); oID = (~(long)T) & oID; }

        // TODO: A bit cumbersome (and unsafe) to have to specify the type and the enum. Find a cleaner method.
        template<typename T>
        Ref<T> GetComponent(COMPONENT_TYPE componentType) 
        { 
            return std::static_pointer_cast<T>(oComponents[componentType]);
        }

        std::map<COMPONENT_TYPE, Ref<Component>>::iterator begin() { return oComponents.begin(); }
        std::map<COMPONENT_TYPE, Ref<Component>>::iterator end() { return oComponents.end(); }
        std::map<COMPONENT_TYPE, Ref<Component>>::const_iterator begin() const { return oComponents.begin(); }
        std::map<COMPONENT_TYPE, Ref<Component>>::const_iterator end() const { return oComponents.end(); }
    private:
        std::map<COMPONENT_TYPE, Ref<Component>> oComponents;

        // ID made up of bit flags which identify the components the entity currently contains
        long oID{ 0 };
    };

}
