#include "Projectile.h"

// Projectile.cpp
#include "Projectile.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
#include <cmath>
#include "../lib_maths/maths.h"

std::unique_ptr<sf::Shape> Projectile::InitializeShape(const sf::Texture* texture, const sf::Vector2f& size) {
    if (texture) {
        // Create a RectangleShape with the provided texture
        auto rectShape = std::make_unique<sf::RectangleShape>(size);
        rectShape->setTexture(texture);
        rectShape->setFillColor(sf::Color::White); // Optional: Set a default color if needed
        return rectShape;
    } else {
        // Create a CircleShape with a default color
        float radius = size.x / 2.f;
        auto circleShape = std::make_unique<sf::CircleShape>(radius);
        circleShape->setFillColor(sf::Color::Yellow); // Choose a default color
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
    _isActive(true)
{
    // Normalize the direction using the maths.h utility
    this->direction = sf::normalize(direction);

    // Set initial position
    setPosition(position);
}

void Projectile::Update(const double dt) {
    if (_isActive) {
        sf::Vector2f velocity = direction * speed * static_cast<float>(dt);
        move(velocity);
        Entity::Update(dt);
        //std::cout << "Projectile updated to position (" << _position.x << ", " << _position.y << ")" << std::endl;
    }
}


void Projectile::Render(sf::RenderWindow& window) const {
    if (_isActive)
        window.draw(*_shape);
}

void Projectile::OnCollision(Entity* other) {
	// TODO: remove the projectile from the game on collision
	std::cout << "Projectile collided with an entity.\n";
    deactivate();
    _shape->setFillColor(sf::Color::Yellow);

}