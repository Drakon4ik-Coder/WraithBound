#include "../Player/Player.h"
#include "../src/Entities/Entity.h"
#include "../lib_maths/maths.h"
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

void Player::Update(double dt) {
    Vector2f direction = {0, 0};
    // Move in four directions based on keys
    if (Keyboard::isKeyPressed(Keyboard::W)) direction.y--;
    if (Keyboard::isKeyPressed(Keyboard::S)) direction.y++;
    if (Keyboard::isKeyPressed(Keyboard::A)) direction.x--;
    if (Keyboard::isKeyPressed(Keyboard::D)) direction.x++;

    direction = normalize(direction);
    move(Vector2f(direction.x * dt * _speed, direction.y * dt * _speed));

    Entity::Update(dt);
}

Player::Player() : _speed(200.0f), Entity(make_unique<CircleShape>(25.f)) {
    _shape->setFillColor(Color::Magenta);
    _shape->setOrigin(Vector2f(25.f, 25.f));
}

void Player::Render(sf::RenderWindow& window) const { window.draw(*_shape); }

sf::FloatRect Player::getGlobalBounds() {
    return _shape->getGlobalBounds();
}