#include "../Enemies/MeleeMonster.h"

#include <SFML/Audio.hpp>  // Include the SFML Audio module

#include "../lib_maths/maths.h"

// Add these member variables at the top of your class (MeleeMonster.h)
sf::SoundBuffer collisionSoundBuffer;
sf::Sound collisionSound;

MeleeMonster::MeleeMonster(sf::Texture& spritesheet, sf::Vector2i size,
                           std::shared_ptr<Player> player,
                           sf::Vector2f position)
    : Monster(std::make_unique<sf::CircleShape>(32.f), 100.f, 100, 1),
      player(player) {
    if (!collisionSoundBuffer.loadFromFile(
            "res/audio/skeleton_melee/sword-sound-effect.mp3")) {
        std::cerr << "Error loading collision sound!" << std::endl;
    } else {
        collisionSound.setBuffer(collisionSoundBuffer);
    }
    _shape->setOrigin(sf::Vector2f(32.f, 32.f));
    _shape->setTexture(&spritesheet);
    _shape->setTextureRect(sf::IntRect(sf::Vector2i{0, 0}, size));
    _position = position;            // Set spawn position directly
    _shape->setPosition(_position);  // Ensure the shape's position matches
}

void MeleeMonster::Update(const double dt) {
    if (!player) return;  // check if player reference is valid

    sf::Vector2f diff = player->getPosition() - getPosition();
    sf::Vector2f direction = sf::normalize(diff);
    sf::Vector2f moveVect =
        sf::Vector2f(dt * _speed * direction.x, dt * _speed * direction.y);

    if (!_shape->getGlobalBounds().intersects(player->getGlobalBounds())) {
        move(moveVect);
    } else {
        // Collision detected! Play the sound
        collisionSound.play();

        // Handle collision logic, for example:
        player->OnCollision(this);        // Notify player of the collision
        this->OnCollision(player.get());  // Notify monster of the collision
    }

    // Direction looking handling
    if ((direction.x < 0 && !lookLeft) || (direction.x >= 0 && lookLeft)) {
        _shape->scale(-1.f, 1.f);
        lookLeft = !lookLeft;
    }

    // Animation handling
    static const int size = 128;
    static const float frameDuration = 0.067f;
    static int frame_i = 1;
    static float time = 0;
    int frames = _shape->getTexture()->getSize().x / size;

    time += dt;
    if (time > frameDuration) {
        _shape->setTextureRect(sf::IntRect(
            sf::Vector2i{frame_i * size + 30, 60}, sf::Vector2i{68, 68}));
        frame_i = (frame_i + 1) % frames;
        time = 0;
    }

    Monster::Update(dt);
}

void MeleeMonster::Render(sf::RenderWindow& window) const {
    window.draw(*_shape);
}

void MeleeMonster::attack() { return; }
