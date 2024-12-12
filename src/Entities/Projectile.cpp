#include "Projectile.h"
#include "../lib_maths/maths.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
#include <cmath>
#include "../src/Entities/Player/Player.h"  // Include the Player class

std::unique_ptr<sf::Shape> Projectile::InitializeShape(const sf::Texture* texture, const sf::Vector2f& size) {
    if (texture) {
        // Create a RectangleShape with the provided texture
        auto rectShape = std::make_unique<sf::RectangleShape>(size);
        rectShape->setTexture(texture);
        rectShape->setFillColor(sf::Color::White); // Optional: Set a default color if needed
        return rectShape;
    }
    else {
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
    _isActive(true),
    elapsedTime(0.f)  // Initialize the elapsed time
{
    // Normalize the direction using the maths.h utility
    this->direction = sf::normalize(direction);

    // Set initial position
    setPosition(position);
}

void Projectile::Update(const double dt) {
    if (_isActive) {
        Entity::Update(dt);
        sf::Vector2f velocity = direction * speed * static_cast<float>(dt);
        move(velocity);

        // Update the elapsed time
        elapsedTime += static_cast<float>(dt);

        // Calculate the distance traveled
        float distanceTraveled = speed * elapsedTime;

        // Check if the projectile has traveled beyond the maximum distance
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

    std::cout << "Projectile collided with an entity.\n";
    deactivate();
    _shape->setFillColor(sf::Color::Yellow);
}