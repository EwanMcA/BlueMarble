#pragma once

#include <BlueMarble.h>

class PopulationComponent : public BlueMarble::Component
{
public:
    PopulationComponent(uint32_t population = 0)
        : oPopulation(population) {}

    uint32_t oPopulation;
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