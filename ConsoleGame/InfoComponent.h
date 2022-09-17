#pragma once

#include "Component.h"
#include "Components.h"
#include <string>

struct Info : public ecs::Component<Info, ComponentType::Info>
{
    Info(std::string _name, std::string _description) : name(std::move(_name)), description(std::move(_description))
    {

    }

    std::string name;
    std::string description;
};