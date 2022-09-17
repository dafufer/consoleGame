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
    WeaponItem(int _damage, bool _throwable, bool _usesAmmunition)
    : damage(_damage)
    , throwable(_throwable)
    , usesAmmunition(_usesAmmunition)
    {

    }

    int damage;
    bool throwable;
    bool usesAmmunition;
};

/**
 * \brief Represents ammunition for a weapon that needs it. Ammunition has a name, a description, and damage points, and a quantity.
 */
struct Ammunition : public ecs::Component<WeaponItem, ComponentType::Ammunition>
{
    Ammunition(int _damage, int _quantity)
    : damage(_damage)
    , quantity(_quantity)
    {

    }

    int damage;
    int quantity;
};

