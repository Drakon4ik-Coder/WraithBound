// Entity.cpp
#include "../src/Entities/Entity.h"

using namespace std;
using namespace sf;

Entity::Entity(unique_ptr<Shape> shape) : _shape(std::move(shape)), _position(0.f, 0.f) {}

Entity::~Entity() {}

void Entity::Update(const double dt) {
    _previousPosition = _position;
    _shape->setPosition(_position);
}

const Vector2f Entity::getPosition() { return _position; }

void Entity::setPosition(const Vector2f& pos) { _position = pos; }

void Entity::move(const Vector2f& delta) { _position += delta; }

sf::FloatRect Entity::getGlobalBounds() const {
    return _shape->getGlobalBounds();
}

const sf::Vector2f& Entity::getPreviousPosition() const {
    return _previousPosition;
}
