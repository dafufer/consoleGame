#pragma once

#include "Components.h"
#include "Component.h"

/**
 * \brief Represents a skill of an entity. Skills has a success chance, and a recharge time.
 * The remaining time indicates how many turns remain for the skill to be used again.
 */
struct Skill
{
    Skill(int _successChance, int _rechargeTime)
    : successChance(_successChance)
    , rechargeTime(_rechargeTime)
    , remainingTime(0)

    {

    }

    int successChance;
    int rechargeTime;
    int remainingTime;
};


struct StunSkill final : public Skill, ecs::Component<StunSkill, ComponentType::StunSkill>
{
    StunSkill()
    : Skill(20, 5)
    {

    }
};


struct ChargeSkill final : public Skill, ecs::Component<StunSkill, ComponentType::ChargeSkill>
{
    ChargeSkill()
    :Skill(60, 3)
    {

    }
};