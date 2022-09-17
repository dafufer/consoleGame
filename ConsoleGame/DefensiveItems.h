#pragma once

#include "Component.h"
#include "Components.h"
#include "InfoComponent.h"

/**
 * \brief Represents a defensive item in the game (i.e. a shield, an armor, etc.).
 * Each defensive item has a name, a description, and defense points. Defense points are removed before life points.
 */
struct DefensiveItem : public ecs::Component<DefensiveItem, ComponentType::Defense>
{
    DefensiveItem(Info _info, int _defensePoints)
    : info(std::move(_info)),
    defensePoints(_defensePoints)

    {

    }

    Info info;
    int defensePoints;
};

