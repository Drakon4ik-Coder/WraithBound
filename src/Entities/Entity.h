// Entity.h
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Entity {
   protected:
    std::unique_ptr<sf::Shape> _shape;
    sf::Vector2f _position;
    sf::Vector2f _previousPosition;

   public:
    Entity(std::unique_ptr<sf::Shape> shape);
    virtual ~Entity();

    virtual void Update(const double dt);
    virtual void Render(sf::RenderWindow& window) const = 0;
    virtual void OnCollision(Entity* other) {} 
    virtual void OnTileCollision(int tileType) {}

    const sf::Vector2f getPosition();
    void setPosition(const sf::Vector2f& pos);
    void move(const sf::Vector2f& delta);
    virtual sf::FloatRect getGlobalBounds() const;

    const sf::Vector2f& getPreviousPosition() const;
};