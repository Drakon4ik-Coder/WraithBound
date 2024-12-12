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
    Entity(make_unique<RectangleShape>(Vector2f(64, 64))),
    _currentFrame(0),
    _animationTime(0),
    _frameTime(0.1f),
    _facingLeft(false)
{
    // Load player texture
    if (!_texture.loadFromFile("res/img/Main Character/Sword_Run/Sword_Run_full.png")) {
        std::cerr << "Failed to load player spritesheet!" << std::endl;
    }
    
    // Setup sprite
    _sprite.setTexture(_texture);
    _frameSize = sf::Vector2i(64, 64); // Adjust based on your spritesheet's frame size
    _sprite.setTextureRect(sf::IntRect(0, 64, _frameSize.x, _frameSize.y));
    _sprite.setOrigin(_frameSize.x/2.f, _frameSize.y/2.f);
    
    float scale = 2.0f;
    _sprite.setScale(scale, scale);
    
    // Load projectile texture
    if (!projectileTexture.loadFromFile("res/img/Projectiles/projectile.png")) {
        std::cerr << "Failed to load projectile texture!" << std::endl;
    }
}

void Player::Update(double dt) {
    Vector2f direction = { 0.f, 0.f };
    // Move in four directions based on keys
    if (Keyboard::isKeyPressed(Keyboard::W)) direction.y--;
    if (Keyboard::isKeyPressed(Keyboard::S)) direction.y++;
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        direction.x--;
        _facingLeft = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::D)) {
        direction.x++;
        _facingLeft = false;
    }

    direction = normalize(direction);

    // Move the player
    move(Vector2f(direction.x * dt * _speed, direction.y * dt * _speed));

    // Update animation
    updateAnimation(dt);

    // Auto-aim and fire at the nearest enemy
    autoAimAndFire(dt);

    Entity::Update(dt);
}

void Player::Render(sf::RenderWindow& window) const {
    window.draw(_sprite);
}

sf::FloatRect Player::getGlobalBounds() {
    return _shape->getGlobalBounds();
}

void Player::updateAnimation(float dt) {
    _animationTime += dt;
    
    // Update animation frame
    if (_animationTime >= _frameTime) {
        _animationTime = 0;
        _currentFrame = (_currentFrame + 1) % 8; // Assuming 8 frames in spritesheet
        _sprite.setTextureRect(sf::IntRect(_currentFrame * _frameSize.x, 64*2, _frameSize.x, _frameSize.y));
    }

    // Update sprite direction
    if (_facingLeft) {
        _sprite.setScale(-3.0f, 3.0f);
    } else {
        _sprite.setScale(3.0f, 3.0f);
    }

    // Update sprite position to match entity position
    _sprite.setPosition(getPosition());
}

void Player::autoAimAndFire(double dt) {
    if (_shootTimer < _shootCooldown) {
        _shootTimer += static_cast<float>(dt);
        return;
    }

    float range = 500.0f;
    auto nearestEnemy = _entityManager->findNearestEnemy(getPosition(), range);
    if (nearestEnemy) {
        sf::Vector2f direction = nearestEnemy->getPosition() - getPosition();
        direction = normalize(direction);

        // Define projectile parameters
        sf::Vector2f playerPosition = getPosition();
        sf::Vector2f projectileDirection = direction;

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
            std::cout << "Projectile auto-aimed and fired at position ("
                << projectilePosition.x << ", "
                << projectilePosition.y << ")\n";
            std::cout << "Projectile Direction: (" << projectileDirection.x << ", "
                << projectileDirection.y << ")\n";
        }
    }
}
