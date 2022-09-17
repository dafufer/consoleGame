#pragma once

#include "EntityManager.h"
#include "System.h"
#include "SystemDefs.h"
#include "Skills.h"
#include "TargetComponent.h"
#include "Status.h"
#include "WeaponComponent.h"

#include <cstdlib>

class StunSystem final : public ecs::System<ComponentCount, SystemCount>
{
public:
    explicit StunSystem(ecs::EntityManager<ComponentCount, SystemCount>& entityManager) : mEntityManager(entityManager)
    {
        setRequirements<StunSkill, Target, Info>();
    }

    void update() const
    {
        for (const auto& entity : getManagedEntities())
        {
            auto& stun = mEntityManager.getComponent<StunSkill>(entity);

            // Stun cannot be used
            if (stun.remainingTime > 0)
            {
                --stun.remainingTime;
                continue;
            }

            stun.remainingTime = stun.rechargeTime;

            if (auto const probability = std::rand() % 100; probability > stun.successChance)
            {
                // did not succeed
                printInfo(entity, false);
                continue;
            }

            // Success!!!
            const auto& target = mEntityManager.getComponent<Target>(entity);
            mEntityManager.addComponent<Stunned>(target.entity, 1);

            printInfo(target.entity, true);
        }
    }

private:
    void printInfo([[maybe_unused]] ecs::Entity entity, bool success) const
    {
        if (success)
        {
            std::cout << mEntityManager.getComponent<Info>(entity).name << " was stunned!" << std::endl;
        }
        else
        {
            std::cout << mEntityManager.getComponent<Info>(entity).name << " failed to stun his target!" << std::endl;
            
        }
    }

private:
    ecs::EntityManager<ComponentCount, SystemCount>& mEntityManager;
};




class ChargeSystem final : public ecs::System<ComponentCount, SystemCount>
{
public:
    explicit ChargeSystem(ecs::EntityManager<ComponentCount, SystemCount>& entityManager) : mEntityManager(entityManager)
    {
        setRequirements<WeaponItem, ChargeSkill, Target>();
    }

    void update() const
    {
        for (const auto& entity : getManagedEntities())
        {
            auto& charge = mEntityManager.getComponent<ChargeSkill>(entity);

            // Charge cannot be used
            if (charge.remainingTime > 0)
            {
                --charge.remainingTime;
                continue;
            }

            charge.remainingTime = charge.rechargeTime;

            if (auto const probability = std::rand() % 100; probability > charge.successChance)
            {
                // did not succeed
                printInfo(entity, false); 
                continue;
            }

            // Success!!!
            const auto& target = mEntityManager.getComponent<Target>(entity);
            mEntityManager.addComponent<Charged>(target.entity, 2);
            printInfo(target.entity, true);
        }
    }

private:
    void printInfo([[maybe_unused]] ecs::Entity entity, bool success) const
    {
        if (success)
        {
            std::cout << mEntityManager.getComponent<Info>(entity).name << " is being charged!" << std::endl;
        }
        else
        {
            std::cout << mEntityManager.getComponent<Info>(entity).name << " failed to charge his target!" << std::endl;
            
        }
    }

private:
    ecs::EntityManager<ComponentCount, SystemCount>& mEntityManager;
};
