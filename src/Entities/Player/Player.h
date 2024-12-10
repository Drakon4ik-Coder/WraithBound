#pragma once

#include "../Entity.h"
#include "../src/EntityManager.h" // Include EntityManager header
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Player : public Entity {
private:
    float _speed;
    float _shootCooldown; // Time between shots in seconds
    float _shootTimer;    // Timer to track shooting cooldown
    EntityManager* _entityManager; // Pointer to EntityManager

    sf::Texture _texture;   // Texture for the player
    sf::Sprite _sprite;     // Sprite to display the texture

    // Animation variables
    std::vector<sf::Sprite> _downFrames;   // Frames for moving down
    std::vector<sf::Sprite> _leftFrames;   // Frames for moving left
    std::vector<sf::Sprite> _rightFrames;  // Frames for moving right
    std::vector<sf::Sprite> _upFrames;     // Frames for moving up

    int _animationFrame = 0;  // Current animation frame index
    float _animationTimer = 0.f; // Timer to control frame switching speed

    enum class Direction { Down, Left, Right, Up }; // Enum to track directions
    Direction _currentDirection = Direction::Down; // Default direction is down

public:
    Player(EntityManager* entityManager);

    void setTexture(const sf::Texture& texture);

    void Update(double dt) override;
    void Render(sf::RenderWindow& window) const override;
    sf::FloatRect getGlobalBounds();

private:
    void updateAnimation(double dt); // Helper function to update the animation
};
