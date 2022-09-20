#pragma once

#include "Component.h"
#include "ComponentTypes.h"
#include <string>

/**
 * \brief This component is used to give a name and a description to an entity or other components.
 */
struct Info : public ecs::Component<Info, ComponentType::Info>
{
    Info(std::string _name, std::string _description) : name(std::move(_name)), description(std::move(_description))
    {

    }

    std::string name;
    std::string description;
};