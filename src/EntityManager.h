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
    void AddEntity(std::shared_ptr<Entity> entity);
    void Update(double dt);
    void Render(sf::RenderWindow& window) const;
    void RemoveEntities();
    void HandleCollisions();
    std::shared_ptr<Entity> findNearestEnemy(const sf::Vector2f& position, float range);
    bool isEntityOutsideBounds(const std::shared_ptr<Entity>& entity) const;

    template <typename T>
    bool hasEntitiesOfType() const {
        for (const auto& entity : entities) {
            if (std::dynamic_pointer_cast<T>(entity)) {
                return true;
            }
        }
        return false;
    }
};