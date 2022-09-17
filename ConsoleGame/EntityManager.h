#pragma once

#include <vector>
#include <array>

#include "ComponentContainer.h"
#include "EntityContainer.h"
#include "System.h"

namespace ecs
{
    /**
     * \brief The EntityManager ties all together. It has three member variables:
     * - mComponentContainers which stores std::unique_ptrs to BaseComponentContainer,
     * - mEntities which is just an instance of EntityContainer and
     * - mSystems which stores unique_ptrs to System
     * \tparam ComponentCount Number of components
     * \tparam SystemCount Number of systems
     */
    template<std::size_t ComponentCount, std::size_t SystemCount>
    class EntityManager
    {
    public:
        /**
         * \brief Creates a new component container for the given type
         * \tparam T Component type
         */
        template<typename T>
        void registerComponent();
        /**
         * \brief Creates a new system of the given type and sets its type
         * \tparam T System type
         * \tparam Args System arguments
         * \param args System arguments
         * \return The new system of the given type
         */
        template<typename T, typename ...Args>
        T* createSystem(Args&& ...args);
        /**
         * \brief Calls the reserve methods of ComponentContainer and EntityContainer:
         * \param size memory to be reserved
         */
        void reserve(std::size_t size);
        Entity createEntity();
        void removeEntity(Entity entity);
        /**
         * \brief Uses the bit set of an entity to quickly check if this entity has a component of the given type
         * \tparam T Component type
         * \param entity Entity's id
         * \return true if the entity contains the component of the given type, false otherwise
         */
        template<typename T>
        [[nodiscard]] bool hasComponent(Entity entity) const;
        /**
         * \brief Same as hasComponent() but for multiple component types
         * \tparam Ts Component types
         * \param entity Entity's id
         * \return true if the entity has all the components of the given types, false otherwise
         */
        template<typename ...Ts>
        [[nodiscard]] bool hasComponents(Entity entity) const;
        template<typename T>
        T& getComponent(Entity entity);
        template<typename T>
        [[nodiscard]] const T& getComponent(Entity entity) const;
        template<typename ...Ts>
        std::tuple<Ts&...> getComponents(Entity entity);
        template<typename ...Ts>
        std::tuple<const Ts&...> getComponents(Entity entity) const;
        template<typename T, typename... Args>
        void addComponent(Entity entity, Args&&... args);
        template<typename T>
        void removeComponent(Entity entity);
        template<typename T>
        Entity getOwner(const T& component) const;

    private:
        std::array<std::unique_ptr<BaseComponentContainer>, ComponentCount> mComponentContainers;
        EntityContainer<ComponentCount, SystemCount> mEntities;
        std::vector<std::unique_ptr<System<ComponentCount, SystemCount>>> mSystems;

        /**
         * \brief Checks that the component type id is below the maximum number of components
         * \tparam T Component type
         */
        template<typename T>
        void checkComponentType() const;
        /**
         * \brief Checks that the component type ids are below the maximum number of components
         * \tparam Ts List of component types
         */
        template<typename ...Ts>
        void checkComponentTypes() const;

        /**
         * \brief Gets the component container of type T
         * \tparam T Component type
         * \return A pointer to the component container which manages the components of type T
         */
        template<typename T>
        auto getComponentContainer();
        template<typename T>
        [[nodiscard]] auto getComponentContainer() const;
    };

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename T>
    auto EntityManager<ComponentCount, SystemCount>::getComponentContainer()
    {
        return static_cast<ComponentContainer<T, ComponentCount, SystemCount>*>(mComponentContainers[T::type].get());
    }

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename T>
    auto EntityManager<ComponentCount, SystemCount>::getComponentContainer() const
    {
        return static_cast<ComponentContainer<T, ComponentCount, SystemCount>*>(mComponentContainers[T::type].get());
    }

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename T>
    void EntityManager<ComponentCount, SystemCount>::checkComponentType() const
    {
        static_assert(T::type < ComponentCount);
    }

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename ...Ts>
    void EntityManager<ComponentCount, SystemCount>::checkComponentTypes() const
    {
        (checkComponentType<Ts>(), ...);
    }

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename T>
    void EntityManager<ComponentCount, SystemCount>::registerComponent()
    {
        checkComponentType<T>();
        mComponentContainers[T::type] = std::make_unique<ComponentContainer<T, ComponentCount, SystemCount>>(
            mEntities.getEntityToBitset());
    }

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename T, typename ...Args>
    T* EntityManager<ComponentCount, SystemCount>::createSystem(Args&& ...args)
    {
        auto type = mSystems.size();
        auto& system = mSystems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        system->setUp(type);
        return static_cast<T*>(system.get());
    }

    template<std::size_t ComponentCount, std::size_t SystemCount>
    void EntityManager<ComponentCount, SystemCount>::reserve(std::size_t size)
    {
        for (auto i = static_cast<std::size_t>(0); i < ComponentCount; ++i)
        {
            if (mComponentContainers[i])
            {
                mComponentContainers[i]->reserve(size);
            }
        }
        mEntities.reserve(size);
    }

    template<std::size_t ComponentCount, std::size_t SystemCount>
    Entity EntityManager<ComponentCount, SystemCount>::createEntity()
    {
        return mEntities.create();
    }

    template <std::size_t ComponentCount, std::size_t SystemCount>
    void EntityManager<ComponentCount, SystemCount>::removeEntity(Entity entity)
    {
        // Remove components
        for (auto i = static_cast<std::size_t>(0); i < ComponentCount; ++i)
        {
            if (mComponentContainers[i])
                mComponentContainers[i]->tryRemove(entity);
        }
        // Send message to systems
        for (auto& system : mSystems)
            system->onEntityRemoved(entity);
        // Remove entity
        mEntities.remove(entity);
    }

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename T>
    bool EntityManager<ComponentCount, SystemCount>::hasComponent(Entity entity) const
    {
        checkComponentType<T>();
        return mEntities.getBitset(entity)[T::type];
    }

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename ...Ts>
    bool EntityManager<ComponentCount, SystemCount>::hasComponents(Entity entity) const
    {
        checkComponentTypes<Ts...>();
        auto requirements = std::bitset<ComponentCount>();
        (requirements.set(Ts::type), ...);
        return (requirements & mEntities.getBitset(entity)) == requirements;
    }

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename T>
    T& EntityManager<ComponentCount, SystemCount>::getComponent(Entity entity)
    {
        checkComponentType<T>();
        return getComponentContainer<T>()->get(entity);
    }

    template<std::size_t ComponentCount, std::size_t SystemCount>
    template<typename T>
    inline const T & EntityManager<ComponentCount, SystemCount>::getComponent(Entity entity) const
    {
        checkComponentType<T>();
        return getComponentContainer<T>()->get(entity);
    }

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename ...Ts>
    std::tuple<Ts&...> EntityManager<ComponentCount, SystemCount>::getComponents(Entity entity)
    {
        checkComponentTypes<Ts...>();
        return std::tie(getComponentContainer<Ts>()->get(entity)...);
    }

    template <std::size_t ComponentCount, std::size_t SystemCount>
    template <typename ... Ts>
    std::tuple<const Ts&...> EntityManager<ComponentCount, SystemCount>::getComponents(Entity entity) const
    {
        checkComponentTypes<Ts...>();
        return std::tie(getComponentContainer<Ts>()->get(entity)...);
    }

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename T, typename... Args>
    void EntityManager<ComponentCount, SystemCount>::addComponent(Entity entity, Args&&... args)
    {
        checkComponentType<T>();
        getComponentContainer<T>()->add(entity, std::forward<Args>(args)...);
        // Send message to systems
        const auto& bitset = mEntities.getBitset(entity);
        for (auto& system : mSystems)
        {
            system->onEntityUpdated(entity, bitset);
        }
    }

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename T>
    void EntityManager<ComponentCount, SystemCount>::removeComponent(Entity entity)
    {
        checkComponentType<T>();
        getComponentContainer<T>()->remove(entity);
        // Send message to systems
        const auto& bitset = mEntities.getBitset(entity);
        for (auto& system : mSystems)
            system->onEntityUpdated(entity, bitset);
    }

    template<std::size_t ComponentCount, std::size_t SystemCount> template<typename T>
    Entity  EntityManager<ComponentCount, SystemCount>::getOwner(const T& component) const
    {
        checkComponentType<T>();
        return getComponentContainer<T>()->getOwner(component);
    }
}