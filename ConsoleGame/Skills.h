#pragma once

#include "Components.h"
#include "Component.h"
#include <string>

/**
 * \brief Represents a skill of an entity. Skills has a success chance, and a recharge time.
 * The remaining time indicates how many turns remain for the skill to be used again.
 */
struct Skill : public ecs::Component<Skill, ComponentType::Skill>
{
    Skill(std::string _name, std::string _description, float _successChance, int _rechargeTime)
    : name(std::move(_name))
    , description(std::move(_description))
    , successChance(_successChance)
    , rechargeTime(_rechargeTime)
    , remainingTime(0)

    {

    }

    std::string const name;
    std::string const description;

    float const successChance;
    int const rechargeTime;
    int const remainingTime;

};