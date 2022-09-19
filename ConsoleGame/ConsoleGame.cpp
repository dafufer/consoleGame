// ConsoleGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "SystemDefs.h"
#include "EntityManager.h"
#include "LifeComponent.h"
#include "WeaponComponent.h"
#include "DefensiveItems.h"
#include "SkillComponents.h"
#include "ConsumableComponents.h"
#include "FightSystem.h"
#include "TargetComponent.h"
#include "SkillSystem.h"
#include "InitiativeComponent.h"
#include "StatusComponent.h"

#include <Windows.h>

#include "ConsumableSystem.h"

HANDLE h;
CONSOLE_SCREEN_BUFFER_INFO bufferInfo;

ecs::Entity createKnight(ecs::EntityManager<ComponentCount, SystemCount> & manager)
{
    const auto entity = manager.createEntity();
    manager.addComponent<Info>(entity, "Knight", "");
    manager.addComponent<Life>(entity, 20);

    return entity;
}


ecs::Entity createOrc(ecs::EntityManager<ComponentCount, SystemCount> & manager)
{
    const auto entity = manager.createEntity();
    manager.addComponent<Info>(entity, "Orc", "");
    manager.addComponent<Life>(entity, 60);

    return entity;
}

// Helper function used to print combatants info
void printInfo(ecs::EntityManager<ComponentCount, SystemCount> const & manager,
    const ecs::Entity knightEntity, const ecs::Entity orcEntity)
{
    // TODO: Print equipment
    std::cout << "Knight -> Life: " << manager.getComponent<Life>(knightEntity).lifePoints << " Defense:"
        << (manager.hasComponent<DefensiveItem>(knightEntity) ? manager.getComponent<DefensiveItem>(knightEntity).defensePoints : 0);
    for (auto const &[type, skill] : manager.getComponent<SkillSet>(knightEntity).skills)
    {
        std::cout << " " << skill.info.name << ": " << skill.remainingTime;
    }
    std::cout << std::endl;

    std::cout << "Orc -> Life: " << manager.getComponent<Life>(orcEntity).lifePoints;
    for (auto const &[type, skill] : manager.getComponent<SkillSet>(orcEntity).skills)
    {
        std::cout << " " << skill.info.name << ": " << skill.remainingTime;
    }
    std::cout << std::endl;
}


void equipKnight(ecs::EntityManager<ComponentCount, SystemCount> & manager, const ecs::Entity knightEntity)
{
    int initiative = 0;
    int skillTurns = 0;
    int successChance = 0;

    // Weapon
    std::cout << "Let's start equipping the Knight." << std::endl;
    std::cout << "Choose which weapon should the Knight use:" << std::endl;
    std::cout << "\t1. Sword: 5 damage" << std::endl;
    std::cout << "\t2. Great Sword: 12 damage [Cannot wear shields, -2 initiative]." << std::endl;
    std::cout << "\t3. Short Sword: 3 damage [+2 initiative, -1 skill turn, +10 success chance]" << std::endl;
    char key = std::cin.get();
    while (key != '1' && key != '2' && key != '3')
    {
        std::cout << "Please choose 1, 2 or 3." << std::endl;
        key = std::cin.get();
    }

    switch (key)
    {
    default:
    case '1':
        manager.addComponent<WeaponItem>(knightEntity, Info("Sword", ""), 5, false, false);
        break;
    case '2':
        manager.addComponent<WeaponItem>(knightEntity, Info("Great Sword", ""), 10, false, false);
        initiative -= 2;
        break;
    case '3':
        initiative += 2;
        skillTurns -= 1;
        successChance += 10;
        manager.addComponent<WeaponItem>(knightEntity, Info(" Short Sword", ""), 3, false, false);
        break;
    }

    char const attackChoice = key;
    char defenseChoice = '0';
    if (attackChoice != '2')
    {
        // Shield. Only available if Great Sword was not chosen
        if (attackChoice == '3')
        {
            // Small shield for bastard swords
            std::cout << "You are equipped with a small shield." << std::endl;
            manager.addComponent<DefensiveItem>(knightEntity, Info{ "Small shield", "" }, 25);
        }
        else
        {
            std::cout << "Choose which shield should the Knight use:" << std::endl;
            std::cout << "\t1. Small shield: 25 defense [+2 initiative]" << std::endl;
            std::cout << "\t2. Medium shield: 50 defense." << std::endl;
            std::cout << "\t3. Big shield: 75 defense [-5 initiative, +2 turns for skill, -10 success chance]." << std::endl;
            std::cout << "\t4. Tower shield: 100 defense [-5 initiative, +2 turns for skill, cannot charge]." << std::endl;

            key = std::cin.get();
            while (key != '1' && key != '2' && key != '3' && key != '4')
            {
                std::cout << "Please choose 1, 2, 3, or 4." << std::endl;
                key = std::cin.get();
            }

            defenseChoice = key;
            switch (key)
            {
            default:
            case '1':
                initiative += 2;
                manager.addComponent<DefensiveItem>(knightEntity, Info{ "Small shield", "" }, 25);
                break;
            case '2':
                manager.addComponent<DefensiveItem>(knightEntity, Info{ "Medium shield", "" }, 50);
                break;
            case '3':
                initiative -= 5;
                skillTurns = 2;
                successChance -= 10;
                manager.addComponent<DefensiveItem>(knightEntity, Info{ "Big shield", "" }, 75);
                break;
            case '4':
                initiative -= 5;
                skillTurns = 2;
                manager.addComponent<DefensiveItem>(knightEntity, Info{ "Tower shield", "" }, 100);
                break;
            }
        }
    }

    // Skill
    if (defenseChoice != 4)
    {
        std::cout << "Choose the Knight skill:" << std::endl;
        std::cout << "\t1. Charge: Attack does double damage" << std::endl;
        std::cout << "\t2. Dodge: Avoid an attack." << std::endl;
        key = std::cin.get();
        while (key != '1' && key != '2')
        {
            std::cout << "Please choose 1, or 2" << std::endl;
            key = std::cin.get();
        }

        switch (key)
        {
        default:
        case '1':
            manager.addComponent<SkillSet>(knightEntity, SkillSet(
                {
                    {ComponentType::ChargeSkill, Skill(
                        Info("Charge", ""), 60 + successChance, 3 + skillTurns) }
                }));
            break;

        case '2':
            manager.addComponent<SkillSet>(knightEntity, SkillSet(
                {
                    {ComponentType::DodgeSkill, Skill(
                        Info("Dodge", ""), 40 + successChance, 3 + skillTurns) }
                }));
            break;
        }
    }

    // Choose potion
    std::cout << "Choose the Knight potion:" << std::endl;
    std::cout << "\t1. Health Potion: Recovers 10 life points" << std::endl;
    std::cout << "\t2. Strength Potion: x2 damage of your attack." << std::endl;
    key = std::cin.get();
    while (key != '1' && key != '2')
    {
        std::cout << "Please choose 1, or 2" << std::endl;
        key = std::cin.get();
    }

    switch (key)
    {
    default:
    case '1':
        manager.addComponent<ConsumableSet>(knightEntity, ConsumableSet(
            { {ComponentType::HealthPotion, HealthPotionItem(10) } }));
        break;

    case '2':
        manager.addComponent<ConsumableSet>(knightEntity, ConsumableSet(
            { {ComponentType::StrengthPotion, StrengthPotionItem(2) } }));
        break;
    }

    // Initiative
    manager.addComponent<Initiative>(knightEntity, 10 + initiative);
}

void equipOrc(ecs::EntityManager<ComponentCount, SystemCount> & manager, const ecs::Entity orcEntity)
{
    int initiative = 0;
    int skillTurns = 0;
    int successChance = 0;

    // Weapon
    std::cout << "Now we will be equipping the Orc:" << std::endl;
    std::cout << "Choose which weapon should the Orc use:" << std::endl;
    std::cout << "\t1. Axe: 8 damage" << std::endl;
    std::cout << "\t2. Great Axe: 16 damage [+2 turns to your skill, -15 success chance, -5 initiative]." << std::endl;
    std::cout << "\t3. Battle Axe: 11 damage [-5 success chance, -2 initiative]" << std::endl;
    char key = std::cin.get();
    while (key != '1' && key != '2' && key != '3')
    {
        std::cout << "Please choose 1, 2 or 3." << std::endl;
        key = std::cin.get();
    }

    switch (key)
    {
    default:
    case '1':
        manager.addComponent<WeaponItem>(orcEntity, Info("Axe", ""), 8, false, false);
        break;
    case '2':
        skillTurns += 2;
        successChance -= 15;
        initiative -= 5;
        manager.addComponent<WeaponItem>(orcEntity, Info("Great Axe", ""), 16, false, false);
        break;
    case '3':
        successChance -= 5;
        initiative -= 2;
        manager.addComponent<WeaponItem>(orcEntity, Info("Battle Axe", ""), 11, false, false);
        break;
    }

    // Skill
    std::cout << "Choose the Orc skill:" << std::endl;
    std::cout << "\t1. Stun: The enemy loses its turn" << std::endl;
    std::cout << "\t2. Double attack: Attacks twice in a turn." << std::endl;
    key = std::cin.get();
    while (key != '1' && key != '2')
    {
        std::cout << "Please choose 1, or 2" << std::endl;
        key = std::cin.get();
    }

    switch (key)
    {
    default:
    case '1':
        manager.addComponent<SkillSet>(orcEntity, SkillSet(
            {
                {ComponentType::StunSkill, Skill(
                    Info("Stun", ""), 20 + successChance, 5 + skillTurns) }
            }));
        break;

    case '2':
        manager.addComponent<SkillSet>(orcEntity, SkillSet(
            {
                {ComponentType::DoubleAttackSkill, Skill(
                    Info("Double Attack", ""), 40 + successChance, 4 + skillTurns) }
            }));
        break;
    }

    // Choose potion
    std::cout << "Choose the Orc potion:" << std::endl;
    std::cout << "\t1. Health Potion: Recovers 10 life points" << std::endl;
    std::cout << "\t2. Strength Potion: x2 damage of your attack." << std::endl;
    key = std::cin.get();
    while (key != '1' && key != '2')
    {
        std::cout << "Please choose 1, or 2" << std::endl;
        key = std::cin.get();
    }

    switch (key)
    {
    default:
    case '1':
        manager.addComponent<ConsumableSet>(orcEntity, ConsumableSet(
            { {ComponentType::HealthPotion, HealthPotionItem(10) } }));
        break;

    case '2':
        manager.addComponent<ConsumableSet>(orcEntity, ConsumableSet(
            { {ComponentType::StrengthPotion, StrengthPotionItem(2) } }));
        break;
    }

    // Initiative
    manager.addComponent<Initiative>(orcEntity, 8 + initiative);
}

void equipmentPhase(ecs::EntityManager<ComponentCount, SystemCount> & manager,
    const ecs::Entity knightEntity, const ecs::Entity orcEntity)
{
    equipKnight(manager, knightEntity);
    equipOrc(manager, orcEntity);
}

void combatPhase(ecs::EntityManager<ComponentCount, SystemCount> const & manager,
    const ecs::Entity knightEntity, const ecs::Entity orcEntity,
    const FightSystem& fightSystem, const SkillSystem & skillSystem, const ConsumableSystem & consumableSystem)
{
    // Combat phase
    while (manager.getComponent<Life>(knightEntity).lifePoints > 0 and
        manager.getComponent<Life>(orcEntity).lifePoints > 0)
    {
        // reset the cursor position to where it was each time
        SetConsoleCursorPosition(h, bufferInfo.dwCursorPosition);
        system("cls");

        // Potions
        consumableSystem.update();

        // Skills
        skillSystem.update();

        // Remove life points
        fightSystem.update();

        printInfo(manager, knightEntity, orcEntity);

        if (manager.getComponent<Life>(knightEntity).lifePoints > 0 and
            manager.getComponent<Life>(orcEntity).lifePoints > 0)
        {
            std::cout << "Press enter for the next turn..." << std::endl;
            std::cin.ignore();
        }
        else if (manager.getComponent<Life>(knightEntity).lifePoints > 0)
        {
            std::cout << "Knight won!" << std::endl;
        }
        else if (manager.getComponent<Life>(orcEntity).lifePoints > 0)
        {
            std::cout << "Orc won!" << std::endl;
        }
        else
        {
            std::cout << "It's a draw!" << std::endl;
        }
    }
}

int main()
{
    // This will let us overwrite console text
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(h, &bufferInfo);

    // Register components
    constexpr auto nbEntities = static_cast<std::size_t>(10000);
    auto manager = ecs::EntityManager<ComponentCount, SystemCount>();
    manager.registerComponent<Info>();
    manager.registerComponent<Life>();
    manager.registerComponent<WeaponItem>();
    manager.registerComponent<DefensiveItem>();
    manager.registerComponent<SkillSet>();
    manager.registerComponent<ConsumableSet>();
    manager.registerComponent<Target>();
    manager.registerComponent<Stunned>();
    manager.registerComponent<Charged>();
    manager.registerComponent<Healed>();
    manager.registerComponent<Dodged>();
    manager.registerComponent<DoubleAttacked>();
    manager.registerComponent<Strong>();


    // Initialize systems
    const auto consumableSystem = manager.createSystem<ConsumableSystem>(manager);
    const auto skillSystem = manager.createSystem<SkillSystem>(manager);
    const auto fightSystem = manager.createSystem<FightSystem>(manager);

    // Create entities
    manager.reserve(nbEntities);

    const auto knightEntity = createKnight(manager);
    const auto orcEntity = createOrc(manager);
    manager.addComponent<Target>(knightEntity, orcEntity);
    manager.addComponent<Target>(orcEntity, knightEntity);

    // Game Starts
    do
    {
        std::cout << "Knight vs Orc Battle" << std::endl;
        std::cout << "Welcome traveler. This is a simple battle game between a Knight and an Orc." << std::endl;
        std::cout << "First you will be able to equip the contenders, then the combat will start." << std::endl;
        std::cout << "During combat, you will see the contender's life points, defense points (if any), " <<
            "and they're skill with the remaining turns before being able to execute it." << std::endl;
        std::cout << "Press enter to start equipping the Knight and the Orc..." << std::endl;
        std::cin.ignore();

        equipmentPhase(manager, knightEntity, orcEntity);

        SetConsoleCursorPosition(h, bufferInfo.dwCursorPosition);
        static_cast<void>(system("cls"));
        printInfo(manager, knightEntity, orcEntity);
        std::cout << "Press enter to start the battle..." << std::endl;
        std::cin.ignore();

        combatPhase(manager, knightEntity, orcEntity, *fightSystem, *skillSystem, *consumableSystem);

        std::cout << "\n\nPress R to restart the game or any other key to quit." << std::endl;
    } while (std::cin.get() == 'r');

    return 0;
}