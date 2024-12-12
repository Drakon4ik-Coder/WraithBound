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
                // Add conditions for other entity types if needed
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
                //std::cout << "Collision detected between Entity " << i << " and Entity " << j << "\n";

                // Calculate the overlap
                sf::FloatRect overlap;
                entityA->getGlobalBounds().intersects(entityB->getGlobalBounds(), overlap);

                // Determine the minimal displacement
                sf::Vector2f displacement(0.f, 0.f);
                if (overlap.width < overlap.height) {
                    displacement.x = (entityA->getPosition().x < entityB->getPosition().x) ? -overlap.width : overlap.width;
                } else {
                    displacement.y = (entityA->getPosition().y < entityB->getPosition().y) ? -overlap.height : overlap.height;
                }

                // Apply displacement to separate entities
                entityA->setPosition(entityA->getPosition() + displacement);
                entityB->setPosition(entityB->getPosition() - displacement);

                // Notify both entities of the collision
                entityA->OnCollision(entityB.get());
                entityB->OnCollision(entityA.get());

                //std::cout << "Entities " << i << " and " << j << " positions adjusted to resolve collision.\n";
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
                //std::cout << "Entity colliding with WALL at position: " << entityPos.x << ", " << entityPos.y << "\n";

                // Adjust entity position to resolve collision
                sf::Vector2f tilePos = LevelSystem::getTilePosition(
                    sf::Vector2ul(entityPos / LevelSystem::getTileSize()));
                sf::FloatRect tileBounds(tilePos, sf::Vector2f(LevelSystem::getTileSize(), LevelSystem::getTileSize()));
                sf::FloatRect entityBounds = entity->getGlobalBounds();

                sf::FloatRect overlap;
                if (tileBounds.intersects(entityBounds, overlap)) {
                    sf::Vector2f displacement(0.f, 0.f);
                    if (overlap.width < overlap.height) {
                        displacement.x = (entityBounds.left < tileBounds.left) ? -overlap.width : overlap.width;
                    } else {
                        displacement.y = (entityBounds.top < tileBounds.top) ? -overlap.height : overlap.height;
                    }

                    entity->setPosition(entity->getPosition() + displacement);

                    // Notify the entity of the collision
                    entity->OnTileCollision(LevelSystem::TILE::WALL);
                }
            }
        } catch (std::out_of_range& e) {
            // Entity is outside the level bounds
            //std::cerr << "Entity out of bounds: " << entityPos.x << ", " << entityPos.y << std::endl;
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