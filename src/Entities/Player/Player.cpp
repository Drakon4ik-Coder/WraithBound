// Player.cpp

#include "../Player/Player.h"
#include "../src/Entities/Projectile.h"
#include "../lib_maths/maths.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <iostream>
#include "../src/EntityManager.h"

using namespace sf;
using namespace std;

// Initialize static members if any (e.g., projectile texture)
static sf::Texture projectileTexture;

Player::Player(EntityManager* entityManager)
    : _speed(200.0f),
    _shootCooldown(0.5f),
    _shootTimer(_shootCooldown),
    _entityManager(entityManager),
    Entity(make_unique<CircleShape>(25.f)) // Player radius = 25.f
{
    _shape->setFillColor(Color::Magenta);
    _shape->setOrigin(Vector2f(25.f, 25.f));

    // Load projectile texture
    if (!projectileTexture.loadFromFile("../../res/img/Projectiles/projectile.png")) {
        std::cerr << "Failed to load projectile texture!" << std::endl;
    }
}

void Player::Update(double dt) {
    Vector2f direction = { 0.f, 0.f };
    // Move in four directions based on keys
    if (Keyboard::isKeyPressed(Keyboard::W)) direction.y--;
    if (Keyboard::isKeyPressed(Keyboard::S)) direction.y++;
    if (Keyboard::isKeyPressed(Keyboard::A)) direction.x--;
    if (Keyboard::isKeyPressed(Keyboard::D)) direction.x++;

    direction = normalize(direction);

    // Move the player
    move(Vector2f(direction.x * dt * _speed, direction.y * dt * _speed));

    // Handle shooting cooldown
    if (_shootTimer < _shootCooldown) {
        _shootTimer += static_cast<float>(dt);
    }

    // Handle shooting
    if (Keyboard::isKeyPressed(Keyboard::Space) && _shootTimer >= _shootCooldown) {
        // Define projectile parameters
        sf::Vector2f playerPosition = getPosition();
        sf::Vector2f projectileDirection = direction;

        // If the player is not moving, set a default direction
        if (projectileDirection.x == 0.f && projectileDirection.y == 0.f) {
            projectileDirection = sf::Vector2f(1.f, 0.f); // Default to right
        }

        // Normalize the projectile direction
        projectileDirection = normalize(projectileDirection);

        // Calculate spawn offset
        float playerRadius = 25.f;
        float projectileHalfSize = 5.f; // Half of 10.f size
        sf::Vector2f spawnOffset = projectileDirection * (playerRadius + projectileHalfSize + 1.f); // Additional 1.f to prevent overlap

        // Set projectile spawn position
        sf::Vector2f projectilePosition = playerPosition + spawnOffset;

        float projectileSpeed = 400.f;
        float projectileDamage = 20.f;

        // Create and initialize the projectile
        std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(
            projectilePosition,
            projectileDirection,
            projectileSpeed,
            projectileDamage,
            &projectileTexture, // Use loaded texture
            sf::Vector2f(10.f, 10.f)
        );

        // Add the projectile to the entity manager
        if (_entityManager) {
            _entityManager->AddEntity(projectile);
            _shootTimer = 0.f; // Reset the shoot timer
            std::cout << "Projectile spawned at position ("
                << projectilePosition.x << ", "
                << projectilePosition.y << ")\n";
            std::cout << "Projectile Direction: (" << projectileDirection.x << ", "
                << projectileDirection.y << ")\n";
        }
    }

    Entity::Update(dt);
}

void Player::Render(sf::RenderWindow& window) const {
    window.draw(*_shape);
}

sf::FloatRect Player::getGlobalBounds() {
    return _shape->getGlobalBounds();
}