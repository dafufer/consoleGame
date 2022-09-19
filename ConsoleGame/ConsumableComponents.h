#pragma once

#include "Component.h"
#include "ComponentTypes.h"


struct Consumable
{
    explicit Consumable(Info _info)
        : info(std::move(_info))
    {

    }

    Info info;
};

struct ConsumableSet final : ecs::Component<ConsumableSet, ComponentType::ConsumableSet>
{
    explicit ConsumableSet(std::unordered_map<ComponentType, Consumable> _consumables)
        : consumables(std::move(_consumables))
    {

    }

    std::unordered_map<ComponentType, Consumable> consumables;
};

/**
 * \brief Represents a health potion item in the game.
 * Potions can only be used once.
 */
struct HealthPotionItem : public Consumable, ecs::Component<HealthPotionItem, ComponentType::HealthPotion>
{
    HealthPotionItem(int _lifePoints)
    : Consumable(Info("Health Potion", ""))
    , lifePoints(_lifePoints)

    {

    }

    int lifePoints;
};


/**
 * \brief Represents a strength potion item in the game. Strength potions have a damage factor that multiplies to the current weapon damage.
 * Potions can only be used once.
 */
struct StrengthPotionItem : public Consumable, ecs::Component<StrengthPotionItem, ComponentType::StrengthPotion>
{
    StrengthPotionItem(int _damageFactor)
    : Consumable(Info("Strength Potion", ""))
    , damageFactor(_damageFactor)
    {

    }

    int damageFactor;
};
