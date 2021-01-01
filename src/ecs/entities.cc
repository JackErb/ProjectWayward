#include <ecs.h>                   
#include <logs.h>                         

EntityManager::EntityManager() {          
  for (Entity entity = 0; entity < MaxEntities; entity++) { 
    available_entities_.push(entity);     
  }                                       
}

EntityManager::~EntityManager() {}        

Entity EntityManager::CreateEntity() {    
  fassert(available_entities_.size() > 0, "CreateEntity() : ran out of space for entities");
  Entity id = available_entities_.front();
  available_entities_.pop();              
  
  return id;                              
}
