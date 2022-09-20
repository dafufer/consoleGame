#include "ConsumableSystem.h"

#include "EntityManager.h"
#include "StatusComponent.h"
#include "InfoComponent.h"
#include "ConsumableComponents.h"

#include <iostream>

ConsumableSystem::ConsumableSystem(ecs::EntityManager<ComponentCount, SystemCount>& entityManager)
    : mEntityManager(entityManager)
{
    setRequirements<ConsumableSet, Info>();
}

void ConsumableSystem::update() const
{
    for (const auto& entity : getManagedEntities())
    {
        if ((mEntityManager.getComponent<ConsumableSet>(entity).consumables.empty()))
        {
            // No potions available
            continue;
        }

        std::cout << mEntityManager.getComponent<Info>(entity).name << ": Would you like to use the potion ? [y/n]" << std::endl;
        char key;
        std::cin >> key;
        while(key != 'y' && key != 'n')
        {
            std::cout << "Please press 'y' or 'n'." << std::endl;
            std::cin >> key;
        }

        if (key == 'n')
        {
            // Do not use the potion equipped
            continue;
        }

        if (mEntityManager.getComponent<ConsumableSet>(entity).consumables.count(ComponentType::HealthPotion))
        {
            useHealthPotion(entity);
        }

         if (mEntityManager.getComponent<ConsumableSet>(entity).consumables.count(ComponentType::StrengthPotion))
        {
            useStrengthPotion(entity);
        }
    }
}

void ConsumableSystem::useHealthPotion(ecs::Entity entity) const
{
    auto const & consumable = mEntityManager.getComponent<ConsumableSet>(entity).consumables.at(ComponentType::HealthPotion);

    if (auto const healthPotion = std::dynamic_pointer_cast<HealthPotionItem>(consumable))
    {
        mEntityManager.addComponent<Healed>(entity, healthPotion->lifePoints);
    }

    mEntityManager.getComponent<ConsumableSet>(entity).consumables.erase(ComponentType::HealthPotion);
    std::cout << mEntityManager.getComponent<Info>(entity).name << " was healed!" << std::endl;
}


void ConsumableSystem::useStrengthPotion(ecs::Entity entity) const
{
    auto const & consumable = mEntityManager.getComponent<ConsumableSet>(entity).consumables.at(ComponentType::StrengthPotion);

    if (auto const strengthPotion = std::dynamic_pointer_cast<StrengthPotionItem>(consumable))
    {
        mEntityManager.addComponent<Strong>(entity, strengthPotion->damageFactor);
    }
    
    mEntityManager.getComponent<ConsumableSet>(entity).consumables.erase(ComponentType::StrengthPotion);
    std::cout << mEntityManager.getComponent<Info>(entity).name << " was strengthened!" << std::endl;
}