#pragma once

#include <BlueMarble.h>

class MapPositionComponent : public BlueMarble::Component 
{
public:
    MapPositionComponent(int x, int y) 
        : oX(x), oY(y) {}

    int oX, oY;
};

class PopulationComponent : public BlueMarble::Component
{
public:
    PopulationComponent(uint32_t population = 0)
        : oPopulation(population) {}

    uint32_t oPopulation;
    bool oDirty;
};

// Size of a rectangular or circular structure on map
class SizeComponent : public BlueMarble::Component
{
public:
    SizeComponent(uint32_t x, uint32_t y, uint32_t r = 0)
        : oX(x), oY(y), oR(r) {}

    // Number of vertices in width (or radius)
    // R = 0 -> rectangular object
    // R > 0 -> circular object
    uint32_t oX, oY, oR;

};

// Defines player ownership of many entities (not an actual player entity)
class PlayerComponent : public BlueMarble::Component
{
public:
    PlayerComponent(uint32_t playerID)
        : oPlayerID(playerID) {}

    uint32_t oPlayerID;
};

class PlayerInputComponent : public BlueMarble::Component
{
public:
    PlayerInputComponent() = default;

    bool leftClick = false;
    bool rightClick = false;
    glm::vec3 mousePosition{ 0.0f, 0.0f, 0.0f };
};

class SelectedComponent : public BlueMarble::Component
{
public:
    SelectedComponent(bool selected = false) : oSelected(selected) {}
    bool oSelected = false;
};

class PlacingComponent : public BlueMarble::Component
{
public:
    PlacingComponent(bool placing = false) : oPlacing(placing) {}
    bool oPlacing = false;
};

class TerritoryComponent : public BlueMarble::Component
{
public:
    TerritoryComponent() = default;
    BlueMarble::TimeStep oTimer;
};