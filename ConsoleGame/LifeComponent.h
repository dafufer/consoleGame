#pragma once

#include "ComponentTypes.h"
#include "Component.h"

/**
 * \brief Represents the life points of an entity.
 */
struct Life : public ecs::Component<Life, ComponentType::Life>
{
    Life(int _lifePoints)
    : lifePoints(_lifePoints)

    {

    }

    int lifePoints;

};