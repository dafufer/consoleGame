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
            useStun(entity);
        }

        if (mEntityManager.getComponent<SkillSet>(entity).skills.count(ComponentType::ChargeSkill))
        {
            useCharge(entity);
        }
    }
}

// TODO: Skills could be refactored
void SkillSystem::useStun(ecs::Entity const entity) const
{
    useSkill<Stunned>(entity, ComponentType::StunSkill,
        mEntityManager.getComponent<Info>(entity).name + " failed to stun his target!",
        mEntityManager.getComponent<Info>(entity).name + " was stunned!",
        1);
}


void SkillSystem::useCharge(ecs::Entity const entity) const
{
    useSkill<Charged>(entity, ComponentType::ChargeSkill,
        mEntityManager.getComponent<Info>(entity).name + " failed to charge his target!",
        mEntityManager.getComponent<Info>(entity).name + " has charged!",
        2);
}

void SkillSystem::useDodge(ecs::Entity const entity) const
{
    useSkill<Dodged>(entity, ComponentType::DodgeSkill,
        mEntityManager.getComponent<Info>(entity).name + " failed to dodge the enemy's attack!",
        mEntityManager.getComponent<Info>(entity).name + " has dodged the attack!");
}

void SkillSystem::useDoubleAttack(ecs::Entity const entity) const
{
    useSkill<DoubleAttacked>(entity, ComponentType::DoubleAttackSkill,
        mEntityManager.getComponent<Info>(entity).name + " failed to attack twice!",
        mEntityManager.getComponent<Info>(entity).name + " attacked twice!");
}
