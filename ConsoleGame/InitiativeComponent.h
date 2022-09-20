#pragma once

#include "ComponentTypes.h"
#include "Component.h"

/**
 * \brief Represents the initiative of an entity. The entity with the highest initiative starts the combat
 */
struct Initiative : public ecs::Component<Initiative, ComponentType::Initiative>
{
    explicit Initiative(const int _initiative)
    : initiative(_initiative)

    {

    }

    int initiative;
};