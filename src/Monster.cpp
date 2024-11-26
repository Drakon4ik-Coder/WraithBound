#include "Monster.h"

void Monster::takeDamage(int damage) { 
    health -= damage; 
}

bool Monster::isAlive() const {
    return health > 0; 
}