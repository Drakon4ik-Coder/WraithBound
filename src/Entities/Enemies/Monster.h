#pragma once
#include "../Entity.h"


class Monster : public Entity {
protected:
    float _speed;
    int _health;
    int _level;
    Monster(std::unique_ptr<sf::Shape> shp, float speed, int health, int level);
public:
    Monster() = delete;
    virtual ~Monster() = default;

    virtual void attack() = 0;

    void takeDamage(int damage);
    bool isAlive() const;

};