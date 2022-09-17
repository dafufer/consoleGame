#pragma once

#include <bitset>
#include <numeric>
#include <vector>

namespace ecs
{
    /**
     * \brief The EntityContainer class will be responsible for managing the entities and storing a std::bitset for each one.
     * This bit set will represent the components that are owned by an entity.
     * \tparam ComponentCount Number of components
     * \tparam SystemCount Number of systems
     */
    template<std::size_t ComponentCount, std::size_t SystemCount>
    class EntityContainer
    {
    public:
        /**
         * \brief Reserves memory for the different containers.
         * \param size Size of the memory to be reserved
         */
        void reserve(std::size_t size);

        std::vector<std::bitset<ComponentCount>>& getEntityToBitset() { return mEntityToBitset; }

        const std::bitset<ComponentCount>& getBitset(Entity entity) const { return mEntityToBitset[entity]; }

        /**
         * \brief If there is a free entity, it recycles it. Otherwise, it creates a new one.
         * \return The entity's id
         */
        Entity create();


        /**
         * \brief Adds the entity to remove in mFreeEntities
         * \param entity Entity's id
         */
        void remove(const Entity entity) { mFreeEntities.push_back(entity); }

    private:
        std::vector<std::bitset<ComponentCount>> mEntityToBitset;
        std::vector<Entity> mFreeEntities;
    };

    template <std::size_t ComponentCount, std::size_t SystemCount>
    void EntityContainer<ComponentCount, SystemCount>::reserve(std::size_t size)
    {
        mFreeEntities.resize(size);
        std::iota(std::begin(mFreeEntities), std::end(mFreeEntities), 0);
        mEntityToBitset.resize(size);
    }

    template <std::size_t ComponentCount, std::size_t SystemCount>
    Entity EntityContainer<ComponentCount, SystemCount>::create()
    {
        auto entity = Entity();
        if (mFreeEntities.empty())
        {
            entity = static_cast<Entity>(mEntityToBitset.size());
            mEntityToBitset.emplace_back();
        }
        else
        {
            entity = mFreeEntities.back();
            mFreeEntities.pop_back();
            mEntityToBitset[entity].reset();
        }
        return entity;
    }
}
