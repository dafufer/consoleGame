#pragma once

#include <bitset>
#include <vector>
#include "Entity.h"
#include <unordered_map>

namespace ecs
{
    /**
     * \brief The purpose of this base class is to be able to store all the ComponentContainer instances in a container.
     */
    class BaseComponentContainer
    {
    public:
        BaseComponentContainer() = default;
        virtual ~BaseComponentContainer() = default;

        virtual void reserve(std::size_t size) = 0;
        virtual bool tryRemove(Entity entity) = 0;
    };

    /**
     * \brief The ComponentContainer class will be responsible for storing all the components of a given type.
     * \tparam T Type of the component
     * \tparam ComponentCount Number of components
     * \tparam SystemCount Number of systems
     */
    template<typename T, std::size_t ComponentCount, std::size_t SystemCount>
    class ComponentContainer : public BaseComponentContainer
    {
    public:
        /**
         * \brief The constructor takes a reference to the container which contains the bit sets of the entities.
         * This class will use it to check if an entity has a component and to update the bit set of an entity when a component is added or removed.
         * \param entityToBitset A reference to the container which contains the bit sets of the entities
         */
        ComponentContainer(std::vector<std::bitset<ComponentCount>>& entityToBitset);

        /**
         * \brief Reserves memory for the different containers.
         * \param size Size of the memory to be reserved
         */
        virtual void reserve(std::size_t size) override;

        /**
         * \brief Gets the index of entity's component
         * \param entity Entity's id
         * \return The index of the entity's component
         */
        T& get(const Entity entity) { return mComponents[mEntityToComponent[entity]]; }
        const T& get(const Entity entity) const { return mComponents[mEntityToComponent.at(entity)]; }

        /**
         * \brief Emplaces a new component at the end of mComponents
         * then it sets up the links to go from the entity to the component and from the component to the entity.
         * Finally, it sets the bit corresponding to the component to true in entity's bitset
         * \tparam Args Component types
         * \param entity Entity's id
         * \param args List of components
         */
        template<typename... Args>
        void add(Entity entity, Args&&... args);

        /**
         * \brief Sets the bit corresponding to the component to false,
         * then it moves the last component in mComponents to the index of the one we want to remove.
         * It updates the links of the component we just moved and removes the one of the component we want to destroy
         * \param entity Entity's id
         */
        void remove(Entity entity);

        /**
         * \brief Tests if the entity has the component before trying to remove it
         * \param entity Entity's id
         * \return true if the entity has been removed, false otherwise
         */
        virtual bool tryRemove(Entity entity) override;

        /**
         * \brief Gets the entity that owns a component
         * \param component The component which we want to get the owner
         * \return The entity that owns a component
         */
        Entity getOwner(const T& component) const;

    private:
        std::vector<T> mComponents;
        std::vector<Entity> mComponentToEntity;
        std::unordered_map<Entity, Index> mEntityToComponent;
        std::vector<std::bitset<ComponentCount>>& mEntityToBitset;
    };

    template <typename T, std::size_t ComponentCount, std::size_t SystemCount>
    ComponentContainer<T, ComponentCount, SystemCount>::ComponentContainer(std::vector<std::bitset<ComponentCount>>& entityToBitset):
        mEntityToBitset(entityToBitset)
    {

    }

    template <typename T, std::size_t ComponentCount, std::size_t SystemCount>
    void ComponentContainer<T, ComponentCount, SystemCount>::reserve(std::size_t size)
    {
        mComponents.reserve(size);
        mComponentToEntity.reserve(size);
        mEntityToComponent.reserve(size);
    }

    template <typename T, std::size_t ComponentCount, std::size_t SystemCount>
    template <typename ... Args>
    void ComponentContainer<T, ComponentCount, SystemCount>::add(Entity entity, Args&&... args)
    {
        auto index = static_cast<Index>(mComponents.size());
        mComponents.emplace_back(std::forward<Args>(args)...);
        mComponentToEntity.emplace_back(entity);
        mEntityToComponent[entity] = index;
        mEntityToBitset[entity][T::type] = true;
    }

    template <typename T, std::size_t ComponentCount, std::size_t SystemCount>
    void ComponentContainer<T, ComponentCount, SystemCount>::remove(Entity entity)
    {
        mEntityToBitset[entity][T::type] = false;
        auto index = mEntityToComponent[entity];
        // Update mComponents
        mComponents[index] = std::move(mComponents.back());
        mComponents.pop_back();
        // Update mEntityToComponent
        mEntityToComponent[mComponentToEntity.back()] = index;
        mEntityToComponent.erase(entity);
        // Update mComponentToEntity
        mComponentToEntity[index] = mComponentToEntity.back();
        mComponentToEntity.pop_back();
    }

    template <typename T, std::size_t ComponentCount, std::size_t SystemCount>
    bool ComponentContainer<T, ComponentCount, SystemCount>::tryRemove(Entity entity)
    {
        if (mEntityToBitset[entity][T::type])
        {
            remove(entity);
            return true;
        }
        return false;
    }

    template <typename T, std::size_t ComponentCount, std::size_t SystemCount>
    Entity ComponentContainer<T, ComponentCount, SystemCount>::getOwner(const T& component) const
    {
        auto begin = mComponents.data();
        const auto index = static_cast<std::size_t>(&component - begin);
        return mComponentToEntity[index];
    }
}
