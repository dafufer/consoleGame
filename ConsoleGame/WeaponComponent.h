#pragma once

#include "Component.h"
#include "Components.h"
#include  <string>


/**
 * \brief Represents a weapon in the game. Each weapon has a name, a description, and damage points
 * Throwable weapons are lost after one use
 * Ammunition weapons needs an ammunition component in order to be able to be used
 */
struct WeaponItem : public ecs::Component<WeaponItem, ComponentType::Weapon>
{
    WeaponItem(std::string _name, std::string _description, int _damage, bool _throwable, bool _usesAmmunition)
    : name(std::move(_name))
    , description(std::move(_description))
    , damage(_damage)
    , throwable(_throwable)
    , usesAmmunition(_usesAmmunition)
    {

    }

    std::string const name;
    std::string const description;

    int const damage;
    bool const throwable;
    bool const usesAmmunition;
};

/**
 * \brief Represents ammunition for a weapon that needs it. Ammunition has a name, a description, and damage points, and a quantity.
 */
struct Ammunition : public ecs::Component<WeaponItem, ComponentType::Ammunition>
{
    Ammunition(std::string _name, std::string _description, int _damage, int _quantity)
    : name(std::move(_name))
    , description(std::move(_description))
    , damage(_damage)
    , quantity(_quantity)
    {

    }

    std::string const name;
    std::string const description;

    int const damage;
    int quantity;
};

