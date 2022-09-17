#pragma once

#include "Component.h"
#include "Components.h"
#include  <string>


/**
 * \brief Represents a health potion item in the game.
 * Potions can only be used once.
 */
struct HealthPotionItem : public ecs::Component<HealthPotionItem, ComponentType::HealthPotion>
{
    HealthPotionItem(int _lifePoints)
    : lifePoints(_lifePoints)

    {

    }

    int lifePoints;
};


/**
 * \brief Represents a strength potion item in the game. Strength potions have a damage factor that multiplies to the current weapon damage.
 * Potions can only be used once.
 */
struct StrengthPotionItem : public ecs::Component<StrengthPotionItem, ComponentType::StrengthPotion>
{
    StrengthPotionItem(float _damageFactor)
    : damageFactor(_damageFactor)

    {

    }

    float damageFactor;
};
