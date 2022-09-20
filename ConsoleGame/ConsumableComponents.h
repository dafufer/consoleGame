#pragma once

#include "Component.h"
#include "ComponentTypes.h"


/**
 * \brief A consumable item is an item that can only be used once. An example of a consumable item would be a potion.
 */
struct Consumable
{
    explicit Consumable(Info _info)
        : info(std::move(_info))
    {

    }

    virtual ~Consumable() = default;

    Info info;
};

/**
 * \brief An entity can wear a list of consumables.
 */
struct ConsumableSet final : ecs::Component<ConsumableSet, ComponentType::ConsumableSet>
{
    explicit ConsumableSet(std::unordered_map<ComponentType, std::shared_ptr<Consumable>> _consumables)
        : consumables(std::move(_consumables))
    {

    }

    std::unordered_map<ComponentType, std::shared_ptr<Consumable>> consumables;
};

/**
 * \brief Represents a health potion item in the game.
 * Potions can only be used once.
 */
struct HealthPotionItem final : public Consumable, ecs::Component<HealthPotionItem, ComponentType::HealthPotion>
{
    explicit HealthPotionItem(int const _lifePoints)
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
struct StrengthPotionItem final : public Consumable, ecs::Component<StrengthPotionItem, ComponentType::StrengthPotion>
{
    explicit StrengthPotionItem(int const _damageFactor)
    : Consumable(Info("Strength Potion", ""))
    , damageFactor(_damageFactor)
    {

    }

    int damageFactor;
};
