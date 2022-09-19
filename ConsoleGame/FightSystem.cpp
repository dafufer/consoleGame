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
        const auto target = mEntityManager.getComponent<Target>(entity);
        auto & life = mEntityManager.getComponent<Life>(target.entity);

        if (life.lifePoints < 0)
        {
            // Entity is dead
            continue;
        }

        // Entity is stunned, can't do anything
        if (mEntityManager.hasComponent<Stunned>(entity))
        {
            mEntityManager.removeComponent<Stunned>(entity);
            continue;
        }

        auto weapon = mEntityManager.getComponent<WeaponItem>(entity);

        // Target is being charged
        if (mEntityManager.hasComponent<Charged>(target.entity))
        {
            weapon.damage *= 2;
        }

        if (mEntityManager.hasComponent<Charged>(entity))
        {
            mEntityManager.removeComponent<Charged>(entity);
        }

        // Remove defense points first
        if (mEntityManager.hasComponent<DefensiveItem>(target.entity))
        {
            auto& defense = mEntityManager.getComponent<DefensiveItem>(target.entity);
            defense.defensePoints -= weapon.damage;
            weapon.damage = 0;
            if (defense.defensePoints < 0)
            {
                weapon.damage = std::abs(defense.defensePoints);
                mEntityManager.removeComponent<DefensiveItem>(target.entity);
            }
        }

        // Remove life points
        life.lifePoints -= weapon.damage;
    }
}
