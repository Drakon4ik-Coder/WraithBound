// Player.h
#pragma once

#include "../Entity.h"
#include "../src/EntityManager.h" // Include EntityManager header
#include <SFML/Window.hpp>
#include <memory>

class Player : public Entity {
private:
    float _speed;
    float _shootCooldown; // Time between shots in seconds
    float _shootTimer;    // Timer to track shooting cooldown
    EntityManager* _entityManager; // Pointer to EntityManager

public:
    /**
     * @brief Constructs a Player with a reference to the EntityManager.
     *
     * @param entityManager Pointer to the EntityManager.
     */
    Player(EntityManager* entityManager);

    void Update(double dt) override;
    void Render(sf::RenderWindow& window) const override;
    sf::FloatRect getGlobalBounds();
};