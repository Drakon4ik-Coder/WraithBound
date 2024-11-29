#pragma once
#include "Monster.h"

class MeleeMonster : public Monster {
public:
    MeleeMonster();
    void Update(const double dt) override;
    void Render(sf::RenderWindow& window) const override;
    void attack() override;
};