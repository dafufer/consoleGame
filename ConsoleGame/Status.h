#pragma once

#include "Component.h"
#include "Components.h"


/**
 * \brief Represents a stunned status for an entity. While stunned, the entity can't do anything.
 */
struct Stunned final : public ecs::Component<Stunned, ComponentType::StunnedStatus>
{
    explicit Stunned(int _remainingTurns)
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
    explicit Charged(int _damageFactor)
        : damageFactor(_damageFactor)
    {

    }

    int damageFactor;
};