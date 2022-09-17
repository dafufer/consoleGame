#pragma once

#include <cstddef>

namespace ecs
{
    /**
     * \brief The component class just stores a type id that will be used later to index some data structures by component type.
     * \tparam T Type of the component
     * \tparam Type Value convertible to std::size_t and will serve to set the component's type id.
     *
     * Example of use:
     * \code
     * enum class ComponentType
        {
            Position
        };

        struct Position : Component<Position, ComponentType::Position>
        {
            float x;
            float y;
        };
     * \endcode
     */
    template<typename T, auto Type>
    class Component
    {
    public:
        static constexpr auto type = static_cast<std::size_t>(Type);
    };
}

