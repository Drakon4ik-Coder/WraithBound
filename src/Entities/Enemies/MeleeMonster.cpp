#include "../Enemies/MeleeMonster.h"
#include "../lib_maths/maths.h"
#include "../../../lib_tile_level_loader/LevelSystem.h"
#include <SFML/Audio.hpp>

sf::SoundBuffer collisionSoundBuffer;
sf::Sound collisionSound;
std::string musicPath = "res/audio/skeleton_melee/sword-sound-effect.mp3";

MeleeMonster::MeleeMonster(sf::Texture& spritesheet, sf::Vector2i size,
    std::shared_ptr<Player> player,
    sf::Vector2f position)
    : Monster(std::make_unique<sf::CircleShape>(32.f), 100.f, 100, 1),
    player(player), attackTimer(1.0f) {
    if (!collisionSoundBuffer.loadFromFile(musicPath)) {
        std::cerr << "Error loading collision sound!" << std::endl;
    }
    else {
        collisionSound.setBuffer(collisionSoundBuffer);
    }

    _shape->setOrigin(sf::Vector2f(32.f, 32.f));
    _shape->setTexture(&spritesheet);
    _shape->setTextureRect(sf::IntRect(sf::Vector2i{ 0, 0 }, size));
    _position = position;            
    _shape->setPosition(_position); 
    _shape->setScale(1.5f, 1.5f);  
}

void MeleeMonster::Update(const double dt) {
    Monster::Update(dt);
    
    if (!player) return;

    sf::Vector2f diff;

    if (LevelSystem::getTileVectPos(getPosition()) == LevelSystem::getTileVectPos(player->getPosition())) {
        diff = player->getPosition() - getPosition();
    }
    else {
        std::vector<std::pair<int, int>> path = LevelSystem::findPath(getPosition(), player->getPosition());
        if (path.empty()) {
            return;
        }
        std::pair<int,int> nextTile = path.front();
        diff = LevelSystem::getTilePosition(sf::Vector2ul{ static_cast<unsigned long>(nextTile.first), static_cast<unsigned long>(nextTile.second) }) + sf::Vector2f{LevelSystem::getTileSize()/2,LevelSystem::getTileSize()/2} - getPosition();
    }

    sf::Vector2f direction = sf::normalize(diff);
    sf::Vector2f moveVect =
        sf::Vector2f(dt * _speed * direction.x, dt * _speed * direction.y);

    if (!_shape->getGlobalBounds().intersects(player->getGlobalBounds())) {
        move(moveVect);
    }
    else {
        attackTimer += static_cast<float>(dt);
        if (attackTimer >= attackCooldown) {
            collisionSound.play();
            attack();
            attackTimer = 0.0f;
        }
    }

    // Handle direction looking
    if ((direction.x < 0 && !lookLeft) || (direction.x >= 0 && lookLeft)) {
        _shape->scale(-1.f, 1.f);
        lookLeft = !lookLeft;
    }

    // Animation handling
    static const int size = 128;
    static const float frameDuration = 0.067f;
    static int frame_i = 1;
    static float time = 0;
    int textureWidth =
        _shape->getTexture() ? _shape->getTexture()->getSize().x : 0;
    int frames = (textureWidth >= size) ? (textureWidth / size) : 1;

    time += dt;
    if (time > frameDuration) {
        _shape->setTextureRect(sf::IntRect(
            sf::Vector2i{ frame_i * size + 30, 60 }, sf::Vector2i{ 68, 68 }));
        frame_i = (frame_i + 1) % frames;
        time = 0;
    }
}

void MeleeMonster::Render(sf::RenderWindow& window) const {
    window.draw(*_shape);
}

void MeleeMonster::attack() {
    if (player) {
        player->takeDamage(10.0f);
        std::cout << "MeleeMonster attacked the player and dealt 10 damage.\n";
    }
}

void MeleeMonster::takeDamage(float damage) {
    _health -= static_cast<int>(damage);
}

bool MeleeMonster::isAlive() const {
    return _health > 0;
}
