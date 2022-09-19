#pragma once

#include "System.h"
#include "SystemDefs.h"
#include <random>


class SkillSystem final : public ecs::System<ComponentCount, SystemCount>
{
public:
    explicit SkillSystem(ecs::EntityManager<ComponentCount, SystemCount>& entityManager);
    

    void update() const;

private:
    void printInfo([[maybe_unused]] ecs::Entity entity, bool success) const;

    void useStun(ecs::Entity entity) const;
    void useCharge(ecs::Entity entity) const;

private:
    ecs::EntityManager<ComponentCount, SystemCount>& mEntityManager;
    std::random_device r;
    mutable std::default_random_engine randomEngine;
    std::uniform_int_distribution<int> uniform_dist;
};
