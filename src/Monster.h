#pragma once
#include "Entity.h"


class Monster : public Entity {
protected:
    float _speed;
    int health;
public:
    Monster() = delete;
    virtual ~Monster() = default;
    virtual void attack() = 0;
    void takeDamage(int damage);
    bool isAlive() const;
};