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
    : _speed(200.0f),
    _shootCooldown(0.5f),
    _shootTimer(_shootCooldown),
    _entityManager(entityManager),
    Entity(make_unique<CircleShape>(25.f))
{
    // Load player texture
    if (!_texture.loadFromFile("res/img/Main Character/Sword_Run/Sword_Run_full.png")) {
        std::cerr << "Failed to load player texture!" << std::endl;
    }
    else {
        std::cout << "Player texture loaded successfully!" << std::endl;
    }

    // Apply the texture to the sprite
    _sprite.setTexture(_texture);
    _sprite.setOrigin(sf::Vector2f(_texture.getSize()) / 2.f);  // Center the sprite origin

    // Load projectile texture
    if (!projectileTexture.loadFromFile("res/img/Projectiles/projectile.png")) {
        std::cerr << "Failed to load projectile texture!" << std::endl;
    }

    // Frame dimensions for the sprite sheet
    int frameWidth = 64; // Assuming 64x64 frames
    int frameHeight = 64;

    // Vectors to store frames for each direction (W, A, S, D)
    int numFrames = 4; // Adjust according to how many frames per direction

    // Create sprite sheets for each direction
    for (int i = 0; i < numFrames; ++i) {
        // Moving Down (S) is in the first row
        _downFrames.push_back(sf::Sprite(_texture, sf::IntRect(i * frameWidth, 0, frameWidth, frameHeight)));
        // Moving Left (A) is in the second row
        _leftFrames.push_back(sf::Sprite(_texture, sf::IntRect(i * frameWidth, frameHeight, frameWidth, frameHeight)));
        // Moving Right (D) is in the third row
        _rightFrames.push_back(sf::Sprite(_texture, sf::IntRect(i * frameWidth, 2 * frameHeight, frameWidth, frameHeight)));
        // Moving Up (W) is in the fourth row
        _upFrames.push_back(sf::Sprite(_texture, sf::IntRect(i * frameWidth, 3 * frameHeight, frameWidth, frameHeight)));
    }
}

void Player::setTexture(const sf::Texture& texture) {
    _sprite.setTexture(texture);
}

void Player::Update(double dt) {
    Vector2f direction = { 0.f, 0.f };

    // Move in four directions based on keys
    if (Keyboard::isKeyPressed(Keyboard::W)) direction.y--;
    if (Keyboard::isKeyPressed(Keyboard::S)) direction.y++;
    if (Keyboard::isKeyPressed(Keyboard::A)) direction.x--;
    if (Keyboard::isKeyPressed(Keyboard::D)) direction.x++;

    direction = normalize(direction);  // Normalize the direction vector

    // Scale the player to make it bigger during the game 
    _sprite.setScale(1.5f, 1.5f);  

    // Move the player
    _sprite.move(Vector2f(direction.x * dt * _speed, direction.y * dt * _speed));

    // Handle shooting cooldown
    if (_shootTimer < _shootCooldown) {
        _shootTimer += static_cast<float>(dt);
    }

    // Handle shooting
    if (Keyboard::isKeyPressed(Keyboard::Space) && _shootTimer >= _shootCooldown) {
        // Define projectile parameters
        sf::Vector2f projectilePosition = getPosition();
        sf::Vector2f projectileDirection = direction;

        // If the player is not moving, set a default direction
        if (projectileDirection.x == 0.f && projectileDirection.y == 0.f) {
            projectileDirection = sf::Vector2f(1.f, 0.f);
        }

        float projectileSpeed = 400.f;
        float projectileDamage = 20.f;

        // Create and initialize the projectile
        std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(
            projectilePosition,
            projectileDirection,
            projectileSpeed,
            projectileDamage,
            nullptr,
            sf::Vector2f(10.f, 10.f)
        );

        // Add the projectile to the entity manager
        if (_entityManager) {
            _entityManager->AddEntity(projectile);
            _shootTimer = 0.f; // Reset the shoot timer
        }

        std::cout << "Projectile Direction: (" << projectileDirection.x << ", " << projectileDirection.y << ")\n";
    }

    // Update animation frame
    updateAnimation(dt);

    Entity::Update(dt);
}


void Player::updateAnimation(double dt) {
    // Change frame based on movement direction
    switch (_currentDirection) {
    case Direction::Down:
        _sprite.setTextureRect(_downFrames[_animationFrame].getTextureRect());
        break;
    case Direction::Left:
        _sprite.setTextureRect(_leftFrames[_animationFrame].getTextureRect());
        break;
    case Direction::Right:
        _sprite.setTextureRect(_rightFrames[_animationFrame].getTextureRect());
        break;
    case Direction::Up:
        _sprite.setTextureRect(_upFrames[_animationFrame].getTextureRect());
        break;
    }

    // Animate the sprite (cycle through frames)
    _animationTimer += dt;
    if (_animationTimer >= 0.1f) { // Adjust the frame rate (0.1 seconds per frame)
        _animationTimer = 0.f;
        // Now cycle through the frames of the currently active direction
        switch (_currentDirection) {
        case Direction::Down:
            _animationFrame = (_animationFrame + 1) % _downFrames.size(); // Loop through frames
            break;
        case Direction::Left:
            _animationFrame = (_animationFrame + 1) % _leftFrames.size(); // Loop through frames
            break;
        case Direction::Right:
            _animationFrame = (_animationFrame + 1) % _rightFrames.size(); // Loop through frames
            break;
        case Direction::Up:
            _animationFrame = (_animationFrame + 1) % _upFrames.size(); // Loop through frames
            break;
        }
    }
}




void Player::Render(sf::RenderWindow& window) const {
    window.draw(_sprite);
}

sf::FloatRect Player::getGlobalBounds() {
    return _sprite.getGlobalBounds();
}
