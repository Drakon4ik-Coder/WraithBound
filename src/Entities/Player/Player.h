#pragma once

#include <SFML/Window.hpp>
#include <memory>

#include "../Entity.h"
#include "../src/EntityManager.h"  // Include EntityManager header

class Player : public Entity {
private:
    float _speed;
    float _shootCooldown;           // Time between shots in seconds
    float _shootTimer;              // Timer to track shooting cooldown
    EntityManager* _entityManager;  // Pointer to EntityManager
    sf::Sprite _sprite;
    sf::Texture _texture;
    sf::Vector2i _frameSize;
    int _currentFrame;
    float _animationTime;
    float _frameTime;
    bool _facingLeft;
    int _health;  // Add health attribute

public:
    Player(EntityManager* entityManager);
    void Update(double dt) override;
    void Render(sf::RenderWindow& window) const override;
    sf::FloatRect getGlobalBounds();
    void updateAnimation(float dt);

    void autoAimAndFire(double dt);

    // Implement pure virtual functions from Entity
    void takeDamage(float damage) override;
    bool isAlive() const override;
};
