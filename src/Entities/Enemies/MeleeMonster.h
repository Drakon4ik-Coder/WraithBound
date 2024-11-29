#pragma once
#include "../Enemies/Monster.h"

class MeleeMonster : public Monster {
public:
    MeleeMonster() = delete;
    MeleeMonster(sf::Texture& texture, sf::Vector2i size);
    void Update(const double dt) override;
    void Render(sf::RenderWindow& window) const override;
    void attack() override;
};