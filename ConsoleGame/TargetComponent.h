#pragma once

#include "Components.h"
#include "Component.h"
#include "Entity.h"

/**
 * \brief Represents the entity the target is attacking.
 */
struct Target final : public ecs::Component<Target, ComponentType::Target>
{
    explicit Target(ecs::Entity _entity)
    : entity(_entity)

    {

    }

    ecs::Entity entity;

};