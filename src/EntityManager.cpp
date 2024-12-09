#include "EntityManager.h"
#include <iostream>

void EntityManager::AddEntity(std::shared_ptr<Entity> entity) {
    if (entity) {
        entities.push_back(entity);
        std::cout << "Entity added. Total entities: " << entities.size() << std::endl;
    }
    else {
        std::cerr << "Attempted to add a null entity!" << std::endl;
    }
}

void EntityManager::Update(double dt) {
    for (size_t i = 0; i < entities.size(); ++i) {
        auto& entity = entities[i];
        if (entity) {
            entity->Update(dt);
        }
        else {
            std::cerr << "Encountered a null entity at index " << i << " during update!" << std::endl;
        }
    }
}

void EntityManager::Render(sf::RenderWindow& window) const {
    for (const auto& entity : entities) {
        entity->Render(window);
    }
}

void EntityManager::RemoveEntities() {
    // Example: Remove entities that meet a specific condition
    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
            [](const std::shared_ptr<Entity>& entity) {
                // Replace with your own condition, e.g., if entity is inactive
                return false; // e.g., entity->isDead();
            }),
        entities.end());
}
