#pragma once

#include "System.h"
#include "SystemDefs.h"
#include <random>
#include "SkillComponents.h"
#include "EntityManager.h"
#include "ComponentTypes.h"
#include "TargetComponent.h"

#include <iostream>


class SkillSystem final : public ecs::System<ComponentCount, SystemCount>
{
public:
    explicit SkillSystem(ecs::EntityManager<ComponentCount, SystemCount>& entityManager);
    

    void update() const;

private:
    void useStun(ecs::Entity const entity) const;
    void useCharge(ecs::Entity const entity) const;
    void useDodge(ecs::Entity const entity) const;
    void useDoubleAttack(ecs::Entity const entity) const;

    template <typename Status, typename... Args>
    void useSkill(ecs::Entity const entity, ComponentType const type,
        std::string const & successMessage, std::string const & failureMessage, Args&& ... args) const;
private:
    ecs::EntityManager<ComponentCount, SystemCount>& mEntityManager;
    std::random_device r;
    mutable std::default_random_engine randomEngine;
    std::uniform_int_distribution<int> uniform_dist;
};

// TODO: Restrict template to types ?
template <typename Status, typename... Args>
void SkillSystem::useSkill(ecs::Entity const entity, ComponentType const type, std::string const& successMessage,
    std::string const& failureMessage, Args&&... args) const
{
    auto& skill = mEntityManager.getComponent<SkillSet>(entity).skills.at(type);

    // Charge cannot be used
    if (skill.remainingTime > 0)
    {
        --skill.remainingTime;
        return;
    }

    skill.remainingTime = skill.rechargeTime;

    if (auto const probability = uniform_dist(randomEngine); probability > skill.successChance)
    {
        // did not succeed
        std::cout << successMessage << std::endl;
        return;
    }

    // Success!!!
    const auto& target = mEntityManager.getComponent<Target>(entity);
    mEntityManager.addComponent<Status>(target.entity, std::forward<Args>(args)...);
    std::cout << failureMessage << std::endl;
}
