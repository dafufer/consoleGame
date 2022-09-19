#pragma once

#include "System.h"
#include "SystemDefs.h"

class FightSystem : public ecs::System<ComponentCount, SystemCount>
{
public:
    explicit FightSystem(ecs::EntityManager<ComponentCount, SystemCount>& entityManager);

    void onManagedEntityAdded(ecs::Entity entity) override;

    void update() const;

private:
    ecs::EntityManager<ComponentCount, SystemCount>& mEntityManager;
};
