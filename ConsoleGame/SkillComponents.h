#pragma once

#include "ComponentTypes.h"
#include "Component.h"
#include "InfoComponent.h"

/**
 * \brief Represents a skill of an entity. Skills has a success chance, and a recharge time.
 * The remaining time indicates how many turns remain for the skill to be used again.
 */
struct Skill
{
    Skill(Info _info, int _successChance, int _rechargeTime)
        : info(std::move(_info))
        , successChance(_successChance)
        , rechargeTime(_rechargeTime)
        , remainingTime(_rechargeTime)
    {

    }

    Info info;
    int successChance;
    int rechargeTime;
    int remainingTime;
};

struct SkillSet final : ecs::Component<SkillSet, ComponentType::SkillSet>
{
    explicit SkillSet(std::unordered_map<ComponentType, Skill> _skills)
        : skills(std::move(_skills))
    {

    }

    std::unordered_map<ComponentType, Skill> skills;
};