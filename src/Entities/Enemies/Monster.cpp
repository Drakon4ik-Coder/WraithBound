#include "../Enemies/Monster.h"

void Monster::takeDamage(int damage) {
    _health -= damage;
}

bool Monster::isAlive() const {
    return _health > 0;
}

Monster::Monster(std::unique_ptr<sf::Shape> shp, float speed, int health, int level)
    : Entity(std::move(shp)), _speed(speed), _health(health), _level(level) {}