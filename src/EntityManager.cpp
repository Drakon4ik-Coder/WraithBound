#include "EntityManager.h"
#include <iostream>
#include "../src/Entities/Projectile.h"

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
            [](const std::shared_ptr<Entity>& entity) {
                // Check if the entity is a Projectile and inactive
                auto projectile = std::dynamic_pointer_cast<Projectile>(entity);
                if (projectile) {
                    return !projectile->isActive();
                }
                // Add conditions for other entity types if needed
                return false;
            }),
        entities.end());
}


void EntityManager::HandleCollisions() {
    for (size_t i = 0; i < entities.size(); ++i) {
        for (size_t j = i + 1; j < entities.size(); ++j) {
            auto& entityA = entities[i];
            auto& entityB = entities[j];

            if (entityA->getGlobalBounds().intersects(entityB->getGlobalBounds())) {
                std::cout << "Collision detected between Entity " << i << " and Entity " << j << "\n";

                // Calculate the overlap
                sf::FloatRect overlap;
                entityA->getGlobalBounds().intersects(entityB->getGlobalBounds(), overlap);

                // Determine the minimal displacement
                sf::Vector2f displacement(0.f, 0.f);
                if (overlap.width < overlap.height) {
                    displacement.x = (entityA->getPosition().x < entityB->getPosition().x) ? -overlap.width / 2.f : overlap.width / 2.f;
                }
                else {
                    displacement.y = (entityA->getPosition().y < entityB->getPosition().y) ? -overlap.height / 2.f : overlap.height / 2.f;
                }

                // Apply displacement to separate entities
                entityA->setPosition(entityA->getPosition() + displacement);
                entityB->setPosition(entityB->getPosition() - displacement);

                // Notify both entities of the collision
                entityA->OnCollision(entityB.get());
                entityB->OnCollision(entityA.get());

                std::cout << "Entities " << i << " and " << j << " positions adjusted to resolve collision.\n";
            }
        }
    }
}