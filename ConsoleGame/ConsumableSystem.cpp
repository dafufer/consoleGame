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
        // TODO: Ask what to use ?
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
    auto& healthPotion = static_cast<HealthPotionItem&>(
        mEntityManager.getComponent<ConsumableSet>(entity).consumables.at(ComponentType::HealthPotion));

    // Success!!!
    mEntityManager.addComponent<Healed>(entity, healthPotion.lifePoints);

    mEntityManager.removeComponent<HealthPotionItem>(entity);
    std::cout << mEntityManager.getComponent<Info>(entity).name << " was healed!" << std::endl;
}


void ConsumableSystem::useStrengthPotion(ecs::Entity entity) const
{
    auto& strengthPotion = static_cast<StrengthPotionItem&>(
        mEntityManager.getComponent<ConsumableSet>(entity).consumables.at(ComponentType::HealthPotion));

    // Success!!!
    mEntityManager.addComponent<Strong>(entity, strengthPotion.damageFactor);
    mEntityManager.removeComponent<StrengthPotionItem>(entity);

    std::cout << mEntityManager.getComponent<Info>(entity).name << " was strengthened!" << std::endl;
}