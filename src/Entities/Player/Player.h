#pragma once

#include <SFML/Window.hpp>
#include <memory>

#include "../Entity.h"
#include "../src/EntityManager.h" 

class Player : public Entity {
private:
    float _speed;
    float _shootCooldown;
    float _shootTimer;
    EntityManager* _entityManager;
    sf::Sprite _sprite;
    sf::Texture _texture;
    sf::Vector2i _frameSize;
    int _currentFrame;
    float _animationTime;
    float _frameTime;
    bool _facingLeft;
    int _health;
    sf::RectangleShape _healthBar; 
    sf::RectangleShape _healthBarBackground; 

public:
    Player(EntityManager* entityManager);
    void Update(double dt) override;
    void Render(sf::RenderWindow& window) const override;
    sf::FloatRect getGlobalBounds();
    void updateAnimation(float dt);
    void autoAimAndFire(double dt);
    void takeDamage(float damage) override;
    bool isAlive() const override;
    void updateHealthBar(); 
};
