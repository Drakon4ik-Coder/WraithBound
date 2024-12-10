#include "../src/Scenes/Scene.h"
#include "../EntityManager.h"
#include "../Entities/Player/Player.h"
#include "../Entities/Enemies/MeleeMonster.h"
//#include "../src/EntityManager.h"

class InGameScene : public Scene {
private:
    std::shared_ptr<EntityManager> entityManager;
    std::shared_ptr<Player> player;

public:
    InGameScene() {
        entityManager = std::make_shared<EntityManager>();

        // Create player and store a reference to it
        player = std::make_shared<Player>(entityManager.get());
        entityManager->AddEntity(player);  // Add player to the entity manager

        // Add other entities (e.g., enemies)
        for (int i = 0; i < 10; i++) {
            entityManager->AddEntity(std::make_unique<MeleeMonster>(melee_skeleton, sf::Vector2i{ 128, 128 }));
        }
    }

    void handleInput(sf::RenderWindow& window) override {
        // Handle input for in-game logic
    }

    void update(float dt) override {
        // Update in-game entities
        entityManager->Update(dt);
    }

    void render(sf::RenderWindow& window) override {
        // Render in-game entities
        entityManager->Render(window);
    }

    std::shared_ptr<Player> getPlayer() const {
        return player; // Return the player instance stored in this scene
    }
};