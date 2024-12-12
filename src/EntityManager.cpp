#include "EntityManager.h"
#include <iostream>
#include "../src/Entities/Projectile.h"
#include "../lib_tile_level_loader/LevelSystem.h"
#include "../src/Entities/Enemies/Monster.h"

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
    HandleCollisions();
    RemoveEntities();
}

void EntityManager::Render(sf::RenderWindow& window) const {
    for (const auto& entity : entities) {
        entity->Render(window);
    }
}

void EntityManager::RemoveEntities() {
    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
            [this](const std::shared_ptr<Entity>& entity) {
                // Check if the entity is a Projectile and inactive
                auto projectile = std::dynamic_pointer_cast<Projectile>(entity);
                if (projectile) {
                    return !projectile->isActive();
                }
                // Check if the entity is outside the map bounds
                if (isEntityOutsideBounds(entity)) {
                    std::cout << "Entity removed for being outside bounds at position: " << entity->getPosition().x << ", " << entity->getPosition().y << std::endl;
                    return true;
                }
                // Check if the entity is dead
                if (!entity->isAlive()) {
                    std::cout << "Entity removed for being dead.\n";
                    return true;
                }
                return false;
            }),
        entities.end());
}


void EntityManager::HandleCollisions() {
    // Entity-Entity Collisions
    for (size_t i = 0; i < entities.size(); ++i) {
        for (size_t j = i + 1; j < entities.size(); ++j) {
            auto& entityA = entities[i];
            auto& entityB = entities[j];
            
            if (entityA->getGlobalBounds().intersects(entityB->getGlobalBounds())) {
                // Calculate centers
                sf::Vector2f centerA = entityA->getPosition();
                sf::Vector2f centerB = entityB->getPosition();
                
                // Calculate separation vector
                sf::Vector2f separation = centerA - centerB;
                float distance = std::sqrt(separation.x * separation.x + separation.y * separation.y);
                
                if (distance == 0.0f) {
                    separation = sf::Vector2f(1.0f, 0.0f); // Default push direction if centers overlap
                } else {
                    separation /= distance; // Normalize
                }
                
                // Calculate minimum separation distance based on entity bounds
                float minSeparation = (entityA->getGlobalBounds().width + entityB->getGlobalBounds().width) * 0.5f;
                
                // Apply separation
                if (distance < minSeparation) {
                    float pushDistance = (minSeparation - distance) * 0.5f;
                    entityA->setPosition(centerA + separation * pushDistance);
                    entityB->setPosition(centerB - separation * pushDistance);
                }
                
                // Notify both entities of the collision
                entityA->OnCollision(entityB.get());
                entityB->OnCollision(entityA.get());
            }
        }
    }

    // Entity-Tile Collisions
    for (const auto& entity : entities) {
        if (!entity) continue;

        // Get the entity's current position
        sf::Vector2f entityPos = entity->getPosition();

        try {
            // Determine the tile at the entity's position
            LevelSystem::TILE tile = LevelSystem::getTileAt(entityPos);

            // Check if the tile is a WALL
            if (tile == LevelSystem::TILE::WALL) {
                // Reset to previous position instead of calculating displacement
                entity->setPosition(entity->getPreviousPosition());
                
                // Notify the entity of the collision
                entity->OnTileCollision(LevelSystem::TILE::WALL);
            }
        } catch (std::out_of_range& e) {
            // If out of bounds, reset to previous position
            entity->setPosition(entity->getPreviousPosition());
        }
    }
}

std::shared_ptr<Entity> EntityManager::findNearestEnemy(const sf::Vector2f& position, float range) {
    std::shared_ptr<Entity> nearestEnemy = nullptr;
    float minDistance = range;

    for (const auto& entity : entities) {
        auto monster = std::dynamic_pointer_cast<Monster>(entity);
        if (monster && monster->isAlive()) {
            float distance = sf::length(position - monster->getPosition());
            if (distance < minDistance) {
                minDistance = distance;
                nearestEnemy = monster;
            }
        }
    }

    return nearestEnemy;
}

bool EntityManager::isEntityOutsideBounds(const std::shared_ptr<Entity>& entity) const {
    sf::Vector2f position = entity->getPosition();
    return position.x < 0 || position.y < 0 || position.x >= LevelSystem::getWidth() * LevelSystem::getTileSize() || position.y >= LevelSystem::getHeight() * LevelSystem::getTileSize();
}