#include "MeleeMonster.h"
#include "Main.h"
#include "../lib_math/Math.h"

void MeleeMonster::Update(const double dt) {
    Vector2f diff = player->getPosition() - getPosition();
    Vector2f direction = sf::normalize(diff);
    Vector2f moveVect = Vector2f(dt*_speed*direction.x, dt*_speed*direction.y);
    if (length(diff) > 30) {
        move(moveVect);
    }
    Monster::Update(dt);
}

void MeleeMonster::Render(sf::RenderWindow& window) const {
    window.draw(*_shape);
}

MeleeMonster::MeleeMonster()
    : Monster(make_unique<CircleShape>(25.f), 150.f, 100, 1) {
    _shape->setFillColor(Color::Blue);
    _shape->setOrigin(Vector2f(25.f, 25.f));
}
void MeleeMonster::attack() {
    return;
}