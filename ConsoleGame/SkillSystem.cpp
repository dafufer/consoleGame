#include "SkillSystem.h"
#include "EntityManager.h"
#include "SkillComponents.h"
#include "TargetComponent.h"
#include "StatusComponent.h"
#include "WeaponComponent.h"
#include "InfoComponent.h"

#include <iostream>

SkillSystem::SkillSystem(ecs::EntityManager<ComponentCount, SystemCount>& entityManager)
    : mEntityManager(entityManager)
    , r(std::random_device())
    , randomEngine(r())
    , uniform_dist(1, 100)
{
    setRequirements<SkillSet, Target, Info>();
}

void SkillSystem::update() const
{
    for (const auto& entity : getManagedEntities())
    {
        if (mEntityManager.getComponent<SkillSet>(entity).skills.count(ComponentType::StunSkill))
        {
            useStun(entity);
        }

         if (mEntityManager.getComponent<SkillSet>(entity).skills.count(ComponentType::ChargeSkill))
        {
            useCharge(entity);
        }
    }
}

void SkillSystem::useStun(ecs::Entity entity) const
{
    auto& stun = mEntityManager.getComponent<SkillSet>(entity).skills.at(ComponentType::StunSkill);

    // Stun cannot be used
    if (stun.remainingTime > 0)
    {
        --stun.remainingTime;
        return;
    }

    stun.remainingTime = stun.rechargeTime;

    if (auto const probability = uniform_dist(randomEngine); probability > stun.successChance)
    {
        // did not succeed
        std::cout << mEntityManager.getComponent<Info>(entity).name << " failed to stun his target!" << std::endl;
        return;
    }

    // Success!!!
    const auto& target = mEntityManager.getComponent<Target>(entity);
    mEntityManager.addComponent<Stunned>(target.entity, 1);

    std::cout << mEntityManager.getComponent<Info>(entity).name << " was stunned!" << std::endl;
}


void SkillSystem::useCharge(ecs::Entity entity) const
{
    auto& charge = mEntityManager.getComponent<SkillSet>(entity).skills.at(ComponentType::ChargeSkill);

    // Charge cannot be used
    if (charge.remainingTime > 0)
    {
        --charge.remainingTime;
        return;
    }

    charge.remainingTime = charge.rechargeTime;

    if (auto const probability = uniform_dist(randomEngine); probability > charge.successChance)
    {
        // did not succeed
        std::cout << mEntityManager.getComponent<Info>(entity).name << " failed to charge his target!" << std::endl;
        return;
    }

    // Success!!!
    const auto& target = mEntityManager.getComponent<Target>(entity);
    mEntityManager.addComponent<Charged>(target.entity, 2);
    std::cout << mEntityManager.getComponent<Info>(entity).name << " has charged!" << std::endl;
}