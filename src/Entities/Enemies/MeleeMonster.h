#pragma once
#include "../Player/Player.h"
#include "Monster.h"

class MeleeMonster : public Monster {
   private:
    std::shared_ptr<Player> player;
    bool lookLeft = false;

   public:
    MeleeMonster(sf::Texture& spritesheet, sf::Vector2i size,
                 std::shared_ptr<Player> player, sf::Vector2f position);
    void Update(const double dt) override;
    void Render(sf::RenderWindow& window) const override;
    void attack();

    void takeDamage(float damage) override;
    bool isAlive() const override;
};
