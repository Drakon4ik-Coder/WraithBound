#include "../Enemies/MeleeMonster.h"
#include "../src/Main.h"
#include "../lib_maths/maths.h"

void MeleeMonster::Update(const double dt) {
    Vector2f diff = player->getPosition() - getPosition();
    Vector2f direction = sf::normalize(diff);
    Vector2f moveVect = Vector2f(dt * _speed * direction.x, dt * _speed * direction.y);
    if (!_shape->getGlobalBounds().intersects(player->getGlobalBounds())) {
        move(moveVect);
    }
    // direction looking handling
    if(direction.x < 0 && !lookLeft){
        _shape->scale(-1.f,1.f);
        lookLeft = true;
    } else if(direction.x >= 0 && lookLeft) {
        _shape->scale(-1.f,1.f);
        lookLeft = false;
    }
    static int frame_i = 1;
    static float time = 0;
    int size = 128;
    int frames = _shape->getTexture()->getSize().x/size;
    time+=dt;
    if(time > 0.067) {
        _shape->setTextureRect(IntRect(Vector2i{frame_i*size+30, 60}, Vector2i{68, 68}));
        frame_i++;
        if(frame_i == frames) {
            frame_i = 0;
        }
        time = 0;
    }
    
    Monster::Update(dt);
}

void MeleeMonster::Render(sf::RenderWindow& window) const {
    window.draw(*_shape);
}

MeleeMonster::MeleeMonster(sf::Texture& spritesheet, sf::Vector2i size)
    : Monster(make_unique<CircleShape>(32.f), 100.f, 100, 1) {
    _shape->setOrigin(Vector2f(32.f, 32.f));
    _shape->setTexture(&spritesheet);
    _shape->setTextureRect(IntRect(Vector2i{0,0}, size));
}
void MeleeMonster::attack() {
    return;
}