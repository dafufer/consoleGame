#include "FightSystem.h"
#include "WeaponComponent.h"
#include "EntityManager.h"
#include "TargetComponent.h"
#include "StatusComponent.h"
#include "DefensiveItems.h"
#include "LifeComponent.h"
#include "InitiativeComponent.h"

FightSystem::FightSystem(ecs::EntityManager<ComponentCount, SystemCount>& entityManager) : mEntityManager(entityManager)
{
    setRequirements<WeaponItem, Life, Target, Initiative>();
}

void FightSystem::onManagedEntityAdded(ecs::Entity entity)
{
    // We can pre-order the entities here since their initiative won't be modified after
    auto & entities = getManagedEntities();
    std::sort(entities.begin(), entities.end(), [this](ecs::Entity const & entity1, ecs::Entity const & entity2) {
        auto const & initiative1 = mEntityManager.getComponent<Initiative>(entity1);
        auto const & initiative2 = mEntityManager.getComponent<Initiative>(entity2);

        return initiative1.initiative > initiative2.initiative;
    });
}

void FightSystem::update() const
{
    for (const auto& entity : getManagedEntities())
    {
        if (const auto & life = mEntityManager.getComponent<Life>(entity); life.lifePoints < 0)
        {
            // Entity is dead
            continue;
        }

        // Deal with consumable items. They have priority over skills
        int damageFactor = 1;
        if (not consumableItems(entity, damageFactor))
        {
            continue;
        }

        // Deal with skills
        if (not skills(entity, damageFactor))
        {
            continue;
        }

        // Remove defense points first
        auto const & weapon = mEntityManager.getComponent<WeaponItem>(entity);
        const auto & target = mEntityManager.getComponent<Target>(entity);
        int damage = weapon.damage * damageFactor;
        if (mEntityManager.hasComponent<DefensiveItem>(target.entity))
        {
            auto& defense = mEntityManager.getComponent<DefensiveItem>(target.entity);
            defense.defensePoints -= damage;
            damage = 0;
            if (defense.defensePoints < 0)
            {
                damage = std::abs(defense.defensePoints);
                mEntityManager.removeComponent<DefensiveItem>(target.entity);
            }
        }

        // Remove life points
        auto & targetLife = mEntityManager.getComponent<Life>(target.entity);
        targetLife.lifePoints -= damage;
    }
}

bool FightSystem::consumableItems(const ecs::Entity entity, int & damageFactor) const
{
    // Entity is healed
    if (mEntityManager.hasComponent<Healed>(entity))
    {
        auto & life = mEntityManager.getComponent<Life>(entity);
        life.lifePoints += mEntityManager.getComponent<Healed>(entity).lifePoints;
        mEntityManager.removeComponent<Healed>(entity);
    }

    if (mEntityManager.hasComponent<Strong>(entity))
    {
        damageFactor *= mEntityManager.getComponent<Strong>(entity).damageFactor;
        mEntityManager.removeComponent<Strong>(entity);
    }

    return true;
}

bool FightSystem::skills(const ecs::Entity entity, int & damageFactor) const
{
    // Entity is stunned, can't do anything
    if (mEntityManager.hasComponent<Stunned>(entity))
    {
        mEntityManager.removeComponent<Stunned>(entity);
        return false;
    }

    // Entity has been dodged, attack does nothing
    if (mEntityManager.hasComponent<Dodged>(entity))
    {
        mEntityManager.removeComponent<Dodged>(entity);
        return false;
    }

    const auto targetCharged = mEntityManager.getComponent<Target>(entity);
    // Target is being charged
    if (mEntityManager.hasComponent<Charged>(targetCharged.entity))
    {
        damageFactor *= mEntityManager.getComponent<Charged>(targetCharged.entity).damageFactor;
        mEntityManager.removeComponent<Charged>(targetCharged.entity);
    }

    const auto targetDoubleAttack = mEntityManager.getComponent<Target>(entity);
    // Target is being charged
    if (mEntityManager.hasComponent<DoubleAttacked>(targetDoubleAttack.entity))
    {
        damageFactor *= 2;
        mEntityManager.removeComponent<DoubleAttacked>(targetDoubleAttack.entity);
    }

    return true;
}
