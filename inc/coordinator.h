#pragma once

#include <ecs.h>
#include <logs.h>

class GameStateCoordinator {
 public:
  GameStateCoordinator();

  template <typename T>
  void RegisterComponent() {
    component_manager_.RegisterComponent<T>();
  }

  template <typename T>
  T* RegisterSystem() {
    const char *type_name = typeid(T).name();
    fassert(systems_.find(type_name) == systems_.end(), "RegisterSystem() : system already registered");

    systems_[type_name] = new T();
    return static_cast<T*>(systems_[type_name]);
  }

  template <typename T>
  T* GetSystem() {
    const char *type_name = typeid(T).name();
    fassert(systems_.find(type_name) != systems_.end(), "GetSystem() : system not registered");

    return static_cast<T*>(systems_[type_name]);
  }

  template <typename T>
  void AddComponent(Entity entity, T component) {
    component_manager_.AddComponent(entity, component);
  }

  template <typename T>
  void AddComponent(Entity entity) {
    component_manager_.AddComponent(entity, T());
  }

  // creates entity and adds it to relevant systems
  Entity CreateEntity(/*EntityType*/);
  // removes entity and updates relevant systems
  void RemoveEntity(Entity);

  void Update();

 private:
  EntityManager    entity_manager_;
  ComponentManager component_manager_;
  std::unordered_map<const char *, System*> systems_;
};
