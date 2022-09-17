// ConsoleGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// TODO: Add initiative + Potions + Equipment

#include <iostream>
#include <ctime>

#include "SystemDefs.h"
#include "EntityManager.h"
#include "LifeComponent.h"
#include "WeaponComponent.h"
#include "DefensiveItems.h"
#include "Skills.h"
#include "PotionItem.h"
#include "FightSystem.h"
#include "TargetComponent.h"
#include "SkillSystems.h"

ecs::Entity createKnight(ecs::EntityManager<ComponentCount, SystemCount> & manager)
{
    const auto entity = manager.createEntity();
    manager.addComponent<Info>(entity, "Knight", "");
    manager.addComponent<Life>(entity, 20);
    manager.addComponent<DefensiveItem>(entity, Info{ "Shield", "" }, 50);
    manager.addComponent<WeaponItem>(entity, 5, false, false);
    manager.addComponent<ChargeSkill>(entity);

    return entity;
}


ecs::Entity createOrc(ecs::EntityManager<ComponentCount, SystemCount> & manager)
{
    const auto entity = manager.createEntity();
    manager.addComponent<Info>(entity, "Orc", "");
    manager.addComponent<Life>(entity, 60);
    manager.addComponent<WeaponItem>(entity, 8, false, false);
    manager.addComponent<StunSkill>(entity);

    return entity;
}

void printInfo(ecs::EntityManager<ComponentCount, SystemCount> const & manager,
               const ecs::Entity knightEntity, const ecs::Entity orcEntity)
{
    std::cout << "Knight:" << manager.getComponent<Life>(knightEntity).lifePoints << " Defense:"
        << (manager.hasComponent<DefensiveItem>(knightEntity) ? manager.getComponent<DefensiveItem>(knightEntity).defensePoints : 0) << std::endl;
        std::cout << "Orc:" << manager.getComponent<Life>(orcEntity).lifePoints << std::endl;
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // TODO: Better random number generator

    constexpr auto nbEntities = static_cast<std::size_t>(10000);
    constexpr auto nbUpdates = static_cast<std::size_t>(100);
    auto manager = ecs::EntityManager<ComponentCount, SystemCount>();
    manager.registerComponent<Info>();
    manager.registerComponent<Life>();
    manager.registerComponent<WeaponItem>();
    manager.registerComponent<DefensiveItem>();
    manager.registerComponent<StunSkill>();
    manager.registerComponent<ChargeSkill>();
    manager.registerComponent<HealthPotionItem>();
    manager.registerComponent<StrengthPotionItem>();
    manager.registerComponent<Target>();
    manager.registerComponent<Stunned>();
    manager.registerComponent<Charged>();

    const auto chargeSystem = manager.createSystem<ChargeSystem>(manager);
    const auto stunSystem = manager.createSystem<StunSystem>(manager);

    const auto fightSystem = manager.createSystem<FightSystem>(manager);

    manager.reserve(nbEntities);

    const auto knightEntity = createKnight(manager);
    const auto orcEntity = createOrc(manager);
    manager.addComponent<Target>(knightEntity, orcEntity);
    manager.addComponent<Target>(orcEntity, knightEntity);

    printInfo(manager, knightEntity, orcEntity);

    while(manager.getComponent<Life>(knightEntity).lifePoints > 0 and
        manager.getComponent<Life>(orcEntity).lifePoints > 0)
    {
        // TODO: Gameplay and print TUI

        // Skills
        chargeSystem->update();
        stunSystem->update();

        // Remove life points
        fightSystem->update();

        printInfo(manager, knightEntity, orcEntity); 
    }


    return 0;
}