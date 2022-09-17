#pragma once

#include "System.h"
#include "SystemDefs.h"
#include "WeaponComponent.h"
#include "EntityManager.h"
#include "TargetComponent.h"
#include "Status.h"
#include "DefensiveItems.h"
#include "LifeComponent.h"

class FightSystem : public ecs::System<ComponentCount, SystemCount>
{
public:
    FightSystem(ecs::EntityManager<ComponentCount, SystemCount>& entityManager) : mEntityManager(entityManager)
    {
        setRequirements<WeaponItem, Life, Target>();
    }

    void update() const
    {
        for (const auto& entity : getManagedEntities())
        {
            // Entity is stunned, can't do anything
            if (mEntityManager.hasComponent<Stunned>(entity))
            {
                mEntityManager.removeComponent<Stunned>(entity);
                continue;
            }

            const auto target = mEntityManager.getComponent<Target>(entity);
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
                    weapon.damage =  std::abs(defense.defensePoints);
                    mEntityManager.removeComponent<DefensiveItem>(target.entity);
                }
            }

            // Remove life points
            auto & life = mEntityManager.getComponent<Life>(target.entity);
            life.lifePoints -= weapon.damage;
        }
    }

private:
    ecs::EntityManager<ComponentCount, SystemCount>& mEntityManager;
};
