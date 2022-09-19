#pragma once

#include "Component.h"
#include "ComponentTypes.h"
#include  "InfoComponent.h"


/**
 * \brief Represents a weapon in the game. Each weapon has a name, a description, and damage points
 * Throwable weapons are lost after one use
 * Ammunition weapons needs an ammunition component in order to be able to be used
 */
struct WeaponItem : public ecs::Component<WeaponItem, ComponentType::Weapon>
{
    WeaponItem(Info _info, int _damage, bool _throwable, bool _usesAmmunition)
    : info(_info)
    , damage(_damage)
    , throwable(_throwable)
    , usesAmmunition(_usesAmmunition)
    {

    }

    Info info;
    int damage;
    bool throwable;
    bool usesAmmunition;
};

/**
 * \brief Represents ammunition for a weapon that needs it. Ammunition has a name, a description, and damage points, and a quantity.
 */
struct Ammunition : public ecs::Component<WeaponItem, ComponentType::Ammunition>
{
    Ammunition(Info _info, int _damage, int _quantity)
    : info(_info)
    , damage(_damage)
    , quantity(_quantity)
    {

    }

    Info info;
    int damage;
    int quantity;
};

