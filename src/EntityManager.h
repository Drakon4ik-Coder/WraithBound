// EntityManager.h
#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Entities/Entity.h"

class EntityManager {
private:
    std::vector<std::shared_ptr<Entity>> entities;

public:
    EntityManager() = default;
    ~EntityManager() = default;

    // Add an entity to the manager
    void AddEntity(std::shared_ptr<Entity> entity);

    // Update all entities
    void Update(double dt);

    // Render all entities
    void Render(sf::RenderWindow& window) const;

    // Remove entities based on some criteria (e.g., out of bounds, health <= 0)
    void RemoveEntities();

    // Check and handle collisions between entities
    void HandleCollisions();
};