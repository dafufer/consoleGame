#pragma once

#include "System.h"
#include "SystemDefs.h"
#include "Entity.h"

class FightSystem : public ecs::System<ComponentCount, SystemCount>
{
public:
    explicit FightSystem(ecs::EntityManager<ComponentCount, SystemCount>& entityManager);

    void onManagedEntityAdded(ecs::Entity entity) override;

    void update() const;

private:
    bool consumableItems(ecs::Entity const entity, int & damageFactor) const;
    bool skills(ecs::Entity const entity, int & damageFactor) const;

private:
    ecs::EntityManager<ComponentCount, SystemCount>& mEntityManager;
};
