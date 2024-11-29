#include "EntityManager.h"

void EntityManager::AddEntity(std::shared_ptr<Entity> entity) {
    entities.push_back(std::move(entity));
}

void EntityManager::Update(double dt) {
    for (auto& entity : entities) {
        entity->Update(dt);
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
