#pragma once

#include <limits>

namespace ecs
{
    /**
     * \brief Entity's id
     */
    using Entity = std::size_t;

    using Index = std::size_t;
    static constexpr auto invalidIndex = std::numeric_limits<Index>::max();
}
