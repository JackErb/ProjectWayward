#pragma once

#include <logs.h>
#include <cstdint>
#include <queue>
#include <unordered_map>
#include <set>

const int MaxEntities = 10000;
const int MaxComponents = 64;

class GameStateCoordinator;


/* ENTITIES */
using Entity = std::uint16_t;

class EntityManager {
 public:
  EntityManager();
  ~EntityManager();

  Entity CreateEntity();

 private:
  std::queue<Entity> available_entities_;
};


/* COMPONENTS */
class AbstractComponentArray {
 public:
  virtual ~AbstractComponentArray() = default;
};

template <typename T>
class ComponentArray: public AbstractComponentArray {
 public:
   ComponentArray() { size_ = 0; }
   ~ComponentArray() {}

   void Insert(Entity entity, const T &component) {
     fassert(entity_to_index_.find(entity) == entity_to_index_.end(),
             "Insert() : Component added to same entity twice.");

     int index = size_;
     components_[index] = component;
     entity_to_index_[index] = entity;
     size_++;
   }

   T* GetData(Entity entity) {
     fassert(entity_to_index_.find(entity) != entity_to_index_.end(),
             "GetData() : entity does not have component");

     return &components_[entity_to_index_[entity]];
   }

 private:
  int size_;
  T   components_[MaxEntities];
  std::unordered_map<Entity, int> entity_to_index_;
};

class ComponentManager {
 public:
  ComponentManager();
  ~ComponentManager();

  template <typename T>
  void RegisterComponent() {
    const char *type_name = typeid(T).name();

    fassert(component_arrays_.find(type_name) == component_arrays_.end(),
            "RegisterComponent() : component already registered");
    component_arrays_.insert({type_name, new ComponentArray<T>()});
  }

  template <typename T>
  void AddComponent(Entity entity, const T &component) {
    GetComponentArray<T>()->Insert(entity, component);
  }

  template <typename T>
  T* GetComponent(Entity entity) {
    return GetComponentArray<T>()->GetData(entity);
  }

 private:
  // Maps from component class name string pointer to component array
  std::unordered_map<const char*, AbstractComponentArray*> component_arrays_;

  template <typename T>
  ComponentArray<T>* GetComponentArray() {
    const char *type_name = typeid(T).name();
    fassert(component_arrays_.find(type_name) != component_arrays_.end(),
            "GetComponentArray() : component is not registered");
    return static_cast<ComponentArray<T>*>(component_arrays_[type_name]);
  }
};



/* SYSTEMS */
class System {
 public:
  GameStateCoordinator *coordinator;
  std::set<Entity> entities;
};
