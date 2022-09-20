#include "SkillSystem.h"
#include "StatusComponent.h"
#include "InfoComponent.h"

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
            useSkill<Stunned>(entity, ComponentType::StunSkill,
                mEntityManager.getComponent<Info>(entity).name + " failed to stun his target!",
                mEntityManager.getComponent<Info>(entity).name + " was stunned!",
                1);
        }

        if (mEntityManager.getComponent<SkillSet>(entity).skills.count(ComponentType::ChargeSkill))
        {
            useSkill<Charged>(entity, ComponentType::ChargeSkill,
                mEntityManager.getComponent<Info>(entity).name + " failed to charge his target!",
                mEntityManager.getComponent<Info>(entity).name + " has charged!",
                2);
        }

        if (mEntityManager.getComponent<SkillSet>(entity).skills.count(ComponentType::DodgeSkill))
        {
            useSkill<Dodged>(entity, ComponentType::DodgeSkill,
                mEntityManager.getComponent<Info>(entity).name + " failed to dodge the enemy's attack!",
                mEntityManager.getComponent<Info>(entity).name + " has dodged the attack!");
        }

        if (mEntityManager.getComponent<SkillSet>(entity).skills.count(ComponentType::DoubleAttackSkill))
        {
            useSkill<DoubleAttacked>(entity, ComponentType::DoubleAttackSkill,
                mEntityManager.getComponent<Info>(entity).name + " failed to attack twice!",
                mEntityManager.getComponent<Info>(entity).name + " attacked twice!");
        }
    }
}