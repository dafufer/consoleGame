#pragma once

#include "Entity.h"
#include <vector>
#include <bitset>
#include <unordered_map>

namespace ecs
{
    template<std::size_t ComponentCount, std::size_t SystemCount>
    class EntityManager;

    /**
     * \brief Each system has a bit set mRequirements which describes the components it requires.
     * Then, it will maintain a set of entities that satisfy these requirements called mManagedEntities.
     * \tparam ComponentCount Number of components
     * \tparam SystemCount Number of systems
     */
    template<std::size_t ComponentCount, std::size_t SystemCount>
    class System
    {
    public:
        virtual ~System() = default;

    protected:
        template<typename ...Ts>
        void setRequirements();

        [[nodiscard]] const std::vector<Entity>& getManagedEntities() const;
        std::vector<Entity>& getManagedEntities();
        [[nodiscard]] bool isEntityManaged(Entity entity) const { return mEntityToManagedEntity.find(entity) != std::end(mEntityToManagedEntity); }

        /**
         * \brief Called when an entity is added
         * \param entity Entity's id
         */
        virtual void onManagedEntityAdded([[maybe_unused]] Entity entity) {}
        /**
         * \brief Called when an entity is removed
         * \param entity Entity's id
         */
        virtual void onManagedEntityRemoved([[maybe_unused]] Entity entity) {}

    private:
        friend EntityManager<ComponentCount, SystemCount>;

        std::bitset<ComponentCount> mRequirements;
        std::size_t mType{};
        std::vector<Entity> mManagedEntities;
        std::unordered_map<Entity, Index> mEntityToManagedEntity;

        /**
         * \brief Called by the entity manager to assign an id to the system
         * \param type Id of the system
         */
        void setUp(std::size_t type);
        /**
         * \brief Called when an entity is modified (i.e. a component is added or removed).
         * The system checks if the requirements are satisfied and if the entity is already managed.
         * If it satisfies the requirements and does not already manage it, it will add it.
         * However, if it does not satisfy the requirements and it was managed, it will remove it.
         * In the other cases, the system does nothing
         * \param entity Entity's id
         * \param components Components required by the system
         */
        void onEntityUpdated(Entity entity, const std::bitset<ComponentCount>& components);
        /**
         * \brief Called by the entity manager when an entity is removed.
         * If the entity was managed by the system, it removes it
         * \param entity Entity's id
         */
        void onEntityRemoved(Entity entity);
        /**
         * \brief Sets up the link to go from the added entity to its index, adds the entity, and calls onManagedEntityAdded
         * \param entity Entity's id
         */
        void addEntity(Entity entity);

        /**
         * \brief Calls onManagedEntityRemoved, moves the last managed entity to the index of the one to remove.
         * It updates the link of the moved entity.
         * Finally it removes the entity to remove from
         * \param entity Entity's id
         */
        void removeEntity(Entity entity);
    };

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename ...Ts>
    void System<ComponentCount, SystemCount>::setRequirements()
    {
        (mRequirements.set(Ts::type), ...);
    }

    template<std::size_t ComponentCount, std::size_t SystemCount>
    const std::vector<Entity>&  System<ComponentCount, SystemCount>::getManagedEntities() const
    {
        return mManagedEntities;
    }

    template<std::size_t ComponentCount, std::size_t SystemCount>
    std::vector<Entity>&  System<ComponentCount, SystemCount>::getManagedEntities()
    {
        return mManagedEntities;
    }

    template<std::size_t ComponentCount, std::size_t SystemCount>
    void System<ComponentCount, SystemCount>::setUp(std::size_t type)
    {
        mType = type;
    }

    template<std::size_t ComponentCount, std::size_t SystemCount>
    void System<ComponentCount, SystemCount>::onEntityUpdated(Entity entity, const std::bitset<ComponentCount>& components)
    {
        auto satisfied = (mRequirements & components) == mRequirements;
        auto managed = isEntityManaged(entity);
        if (satisfied && !managed)
        {
            addEntity(entity);
        }
        else if (!satisfied && managed)
        {
            removeEntity(entity);
        }
    }

    template<std::size_t ComponentCount, std::size_t SystemCount>
    void System<ComponentCount, SystemCount>::onEntityRemoved(Entity entity)
    {
        if (isEntityManaged(entity))
        {
            removeEntity(entity);
        }
    }

    template<std::size_t ComponentCount, std::size_t SystemCount>
    void System<ComponentCount, SystemCount>::addEntity(Entity entity)
    {
        mEntityToManagedEntity[entity] = static_cast<Index>(mManagedEntities.size());
        mManagedEntities.emplace_back(entity);
        onManagedEntityAdded(entity);
    }

    template<std::size_t ComponentCount, std::size_t SystemCount>
    void System<ComponentCount, SystemCount>::removeEntity(Entity entity)
    {
        onManagedEntityRemoved(entity);
        auto index = mEntityToManagedEntity[entity];
        mEntityToManagedEntity[mManagedEntities.back()] = index;
        mEntityToManagedEntity.erase(entity);
        mManagedEntities[index] = mManagedEntities.back();
        mManagedEntities.pop_back();
    }
}