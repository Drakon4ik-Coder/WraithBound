#include "Projectile.h"
#include "../lib_maths/maths.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
#include <cmath>
#include "Player/Player.h"
#include "Enemies/Monster.h"

std::unique_ptr<sf::Shape> Projectile::InitializeShape(const sf::Texture* texture, const sf::Vector2f& size) {
    if (texture) {
        // Create a RectangleShape with the provided texture
        auto rectShape = std::make_unique<sf::RectangleShape>(size);
        rectShape->setTexture(texture);
        rectShape->setFillColor(sf::Color::White);
        return rectShape;
    }
    else {
        float radius = size.x / 4.f;
        auto circleShape = std::make_unique<sf::CircleShape>(radius);
        circleShape->setFillColor(sf::Color::Yellow);
        return circleShape;
    }
}

Projectile::Projectile(const sf::Vector2f& position,
    const sf::Vector2f& direction,
    float speed,
    float damage,
    const sf::Texture* texture,
    const sf::Vector2f& size)
    : Entity(InitializeShape(texture, size)),
    damage(damage),
    direction(direction),
    speed(speed),
    size(size),
    _isActive(true),
    elapsedTime(0.f)
{
    this->direction = sf::normalize(direction);
    setPosition(position);
}

void Projectile::Update(const double dt) {
    if (_isActive) {
        Entity::Update(dt);
        sf::Vector2f velocity = direction * speed * static_cast<float>(dt);
        move(velocity);
        elapsedTime += static_cast<float>(dt);
        float distanceTraveled = speed * elapsedTime;
        if (distanceTraveled > maxTravelDistance) {
            deactivate();
        }
    }
}

void Projectile::Render(sf::RenderWindow& window) const {
    if (_isActive)
        window.draw(*_shape);
}

void Projectile::OnCollision(Entity* other) {
    // Exclude player from collision
    if (dynamic_cast<Player*>(other) != nullptr) {
        return;
    }
    other->takeDamage(damage);
    deactivate();
    _shape->setFillColor(sf::Color::Yellow);
    std::cout << "Projectile collided with an entity and dealt " << damage << " damage.\n";
}

void Projectile::takeDamage(float damage) {}

bool Projectile::isAlive() const {
    return _isActive;
}