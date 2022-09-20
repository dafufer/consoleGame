#pragma once

#include "System.h"
#include "SystemDefs.h"

/**
 * \brief This system deals with consumable items. When a consumable item is used, it disappears.
 *  A status if assigned to the corresponding entity.
 */
class ConsumableSystem final : public ecs::System<ComponentCount, SystemCount>
{
public:
    explicit ConsumableSystem(ecs::EntityManager<ComponentCount, SystemCount>& entityManager);
    

    void update() const;

private:
    void useHealthPotion(ecs::Entity entity) const;
    void useStrengthPotion(ecs::Entity entity) const;

private:
    ecs::EntityManager<ComponentCount, SystemCount>& mEntityManager;
};
