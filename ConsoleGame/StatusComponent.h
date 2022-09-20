#pragma once

#include "Component.h"
#include "ComponentTypes.h"


/**
 * \brief Represents a stunned status for an entity. While stunned, the entity can't do anything.
 */
struct Stunned final : public ecs::Component<Stunned, ComponentType::StunnedStatus>
{
    explicit Stunned(int const _remainingTurns)
    : remainingTurns(_remainingTurns)
    {

    }

    int remainingTurns;
};


/**
 * \brief Represents a charged status for an entity
 */
struct Charged final : public ecs::Component<Stunned, ComponentType::ChargedStatus>
{
    explicit Charged(int const _damageFactor)
        : damageFactor(_damageFactor)
    {

    }

    int damageFactor;
};

/**
 * \brief Entity has been healed
 */
struct Healed final : public ecs::Component<Stunned, ComponentType::HealedStatus>
{
    explicit Healed(int const _lifePoints)
        : lifePoints(_lifePoints)
    {

    }

    int lifePoints;
};

/**
 * \brief Represents a strengthened entity
 */
struct Strong final : public ecs::Component<Stunned, ComponentType::StrongStatus>
{
    explicit Strong(int const _damageFactor)
        : damageFactor(_damageFactor)
    {

    }

    int damageFactor;
};

/**
 * \brief Represents that an attack have been dodged
 */
struct Dodged : public ecs::Component<Stunned, ComponentType::DodgeStatus>
{
    explicit Dodged() = default;
};

/**
 * \brief An entity receives 2 attacks
 */
struct DoubleAttacked : public ecs::Component<Stunned, ComponentType::DoubleAttackStatus>
{
    explicit DoubleAttacked() = default;
};