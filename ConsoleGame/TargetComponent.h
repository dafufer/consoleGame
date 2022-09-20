#pragma once

#include "ComponentTypes.h"
#include "Component.h"
#include "Entity.h"

/**
 * \brief Represents the entity's target.
 */
struct Target final : public ecs::Component<Target, ComponentType::Target>
{
    explicit Target(ecs::Entity const _entity)
    : entity(_entity)

    {

    }

    ecs::Entity entity;

};