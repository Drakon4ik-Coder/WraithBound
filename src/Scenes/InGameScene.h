#pragma once
#include "../Entities/Player/Player.h"
#include "../EntityManager.h"
#include "Scene.h"

class InGameScene : public Scene {
   private:
    std::shared_ptr<Player> player;
    std::shared_ptr<EntityManager> entityManager;

   public:
    InGameScene(std::shared_ptr<EntityManager> entityManager)
        : entityManager(entityManager) {
        // Create the Player with the entity manager
        player = std::make_shared<Player>(entityManager.get());
        entityManager->AddEntity(player);
    }

    std::shared_ptr<Player> getPlayer() const override { return player; }

    void handleInput(sf::RenderWindow& window) override {
        // Handle input for player and other entities
    }

    void update(float dt) override {
        entityManager->Update(dt);  // Update all entities
    }

    void render(sf::RenderWindow& window) override {
        entityManager->Render(window);  // Render all entities
    }
    void InGameScene::spawnMonsters() {
        // Example of spawning a monster
        auto monster =
            std::make_shared<MeleeMonster>(melee_skeleton, sf::Vector2i(68, 68),
                                           player, gameWidth, gameHeight);
        entityManager->AddEntity(monster);
    }
};
