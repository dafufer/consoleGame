#pragma once

#include "Component.h"
#include "Components.h"
#include  <string>


/**
 * \brief Represents a health potion item in the game.
 * Potions can only be used once.
 */
struct HealthPotionItem : public ecs::Component<HealthPotionItem, ComponentType::Defensive>
{
    HealthPotionItem(std::string _name, std::string _description, int _lifePoints)
    : name(std::move(_name))
    , description(std::move(_description))
    , lifePoints(_lifePoints)

    {

    }

    std::string const name;
    std::string const description;

    int const lifePoints;
};


/**
 * \brief Represents a strength potion item in the game. Strength potions have a damage factor that multiplies to the current weapon damage.
 * Potions can only be used once.
 */
struct StrengthPotionItem : public ecs::Component<StrengthPotionItem, ComponentType::Defensive>
{
    StrengthPotionItem(std::string _name, std::string _description, float _damageFactor)
    : name(std::move(_name))
    , description(std::move(_description))
    , damageFactor(_damageFactor)

    {

    }

    std::string const name;
    std::string const description;

    float const damageFactor;
};
