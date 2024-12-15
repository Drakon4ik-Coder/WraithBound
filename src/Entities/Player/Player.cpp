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

static sf::Texture projectileTexture;

Player::Player(EntityManager* entityManager)
    : _speed(300.0f),
    _shootCooldown(0.3f),
    _shootTimer(_shootCooldown),
    _entityManager(entityManager),
    Entity(make_unique<RectangleShape>(Vector2f(64, 64))),
    _currentFrame(0),
    _animationTime(0),
    _frameTime(0.1f),
    _facingLeft(false),
    _health(100)
{

    if (!_texture.loadFromFile("res/img/Main Character/Sword_Run/Sword_Run_full.png")) {
        std::cerr << "Failed to load player spritesheet!" << std::endl;
    }

    // Setup sprite
    _sprite.setTexture(_texture);
    _frameSize = sf::Vector2i(64, 64);
    _sprite.setTextureRect(sf::IntRect(0, 64, _frameSize.x, _frameSize.y));
    _sprite.setOrigin(_frameSize.x / 2.f, _frameSize.y / 2.f);

    float scale = 2.0f;
    _sprite.setScale(scale, scale);

    // Load projectile texture
    if (!projectileTexture.loadFromFile("res/img/Projectiles/projectile.png")) {
        std::cerr << "Failed to load projectile texture!" << std::endl;
    }

    // Initialize health bar
    _healthBar.setSize(Vector2f(64, 10));
    _healthBar.setFillColor(Color::Green);
    _healthBar.setOrigin(_healthBar.getSize().x / 2.f, _healthBar.getSize().y / 2.f);

    // Initialize health bar background
    _healthBarBackground.setSize(Vector2f(64, 10));
    _healthBarBackground.setFillColor(Color::Red);
    _healthBarBackground.setOrigin(_healthBarBackground.getSize().x / 2.f, _healthBarBackground.getSize().y / 2.f);
}

void Player::Update(double dt) {
    Entity::Update(dt);
    Vector2f direction = { 0.f, 0.f };
    // Move the player
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
    move(Vector2f(direction.x * dt * _speed, direction.y * dt * _speed));
    updateAnimation(dt);
    autoAimAndFire(dt);
    updateHealthBar();
}

void Player::Render(sf::RenderWindow& window) const {
    window.draw(_sprite);
    window.draw(_healthBarBackground); 
    window.draw(_healthBar); 
}

sf::FloatRect Player::getGlobalBounds() {
    return _shape->getGlobalBounds();
}

void Player::updateAnimation(float dt) {
    _animationTime += dt;

    // Update animation frame
    if (_animationTime >= _frameTime) {
        _animationTime = 0;
        _currentFrame = (_currentFrame + 1) % 8; // 8 frames in the spritesheet
        _sprite.setTextureRect(sf::IntRect(_currentFrame * _frameSize.x, 64 * 2, _frameSize.x, _frameSize.y));
    }

    // Update sprite direction
    if (_facingLeft) {
        _sprite.setScale(-4.0f, 4.0f);
    }
    else {
        _sprite.setScale(4.0f, 4.0f);
    }

    // Update sprite position to match entity position
    _sprite.setPosition(getPosition());
}

void Player::autoAimAndFire(double dt) {
    if (_shootTimer < _shootCooldown) {
        _shootTimer += static_cast<float>(dt);
        return;
    }

    float range = 1000.0f;
    auto nearestEnemy = _entityManager->findNearestEnemy(getPosition(), range);
    if (nearestEnemy) {
        sf::Vector2f direction = nearestEnemy->getPosition() - getPosition();
        direction = normalize(direction);

        // Define projectile parameters
        sf::Vector2f playerPosition = getPosition();
        sf::Vector2f projectileDirection = direction;

        // Calculate spawn offset
        float playerRadius = 25.f;
        float projectileHalfSize = 1.f; // Half of 10.f size
        sf::Vector2f spawnOffset = projectileDirection * (playerRadius + projectileHalfSize + 1.f); // Additional 1.f to prevent overlap

        sf::Vector2f projectilePosition = playerPosition + spawnOffset;

        float projectileSpeed = 500.f;
        float projectileDamage = 10.f;

        std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(
            projectilePosition,
            projectileDirection,
            projectileSpeed,
            projectileDamage,
            &projectileTexture,
            sf::Vector2f(50.f, 50.f)
        );

        if (_entityManager) {
            _entityManager->AddEntity(projectile);
            _shootTimer = 0.f;
            std::cout << "Projectile auto-aimed and fired at position ("
                << projectilePosition.x << ", "
                << projectilePosition.y << ")\n";
            std::cout << "Projectile Direction: (" << projectileDirection.x << ", "
                << projectileDirection.y << ")\n";
        }
    }
}

void Player::takeDamage(float damage) {
    _health -= static_cast<int>(damage);
    if (_health < 0) _health = 0;
    updateHealthBar();
}

bool Player::isAlive() const {
    return _health > 0;
}

void Player::updateHealthBar() {
    _healthBar.setSize(Vector2f(64 * (_health / 100.f), 10));
    _healthBar.setPosition(_sprite.getPosition().x, _sprite.getPosition().y - 50);
    _healthBarBackground.setPosition(_sprite.getPosition().x, _sprite.getPosition().y - 50);
}
