#pragma once

#include "Component.h"
#include "Components.h"
#include  <string>


/**
 * \brief Represents a defensive item in the game (i.e. a shield, an armor, etc.).
 * Each defensive item has a name, a description, and defense points. Defense points are removed before life points.
 */
struct DefensiveItem : public ecs::Component<DefensiveItem, ComponentType::Defensive>
{
    DefensiveItem(std::string _name, std::string _description, int _defensePoints)
    : name(std::move(_name))
    , description(std::move(_description))
    , defensePoints(_defensePoints)

    {

    }

    std::string const name;
    std::string const description;

    int defensePoints;
};

