#pragma once
#include <SFML/Audio.hpp>  // Include the SFML Audio module
#include "../Player/Player.h"
#include "Monster.h"

class MeleeMonster : public Monster {
   private:
    std::shared_ptr<Player> player;  // Reference to the player
    bool lookLeft = false;

   public:
    MeleeMonster(sf::Texture& spritesheet, sf::Vector2i size,
                 std::shared_ptr<Player> player, sf::Vector2f position);
    void Update(const double dt) override;
    void Render(sf::RenderWindow& window) const override;
    void attack();
};
