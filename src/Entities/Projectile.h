#pragma once

#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Projectile : public Entity {
public:
    Projectile(const sf::Vector2f& position,
        const sf::Vector2f& direction,
        float speed,
        float damage,
        const sf::Texture* texture = nullptr,
        const sf::Vector2f& size = sf::Vector2f(10.f, 10.f));


    virtual void Update(const double dt) override;

    virtual void Render(sf::RenderWindow& window) const override;

    float getDamage() const { return damage; }

    void OnCollision(Entity* other) override;
    bool isActive() const { return _isActive; }
    void deactivate() { _isActive = false; }

private:
    float damage;                     
	sf::Vector2f direction;   // Normalized direction vector
    float speed;                     
    sf::Vector2f size;   
    bool _isActive;

    // Initializes the shape based on whether a texture is provided.
    static std::unique_ptr<sf::Shape> InitializeShape(const sf::Texture* texture, const sf::Vector2f& size);
};

