#include "../src/Scenes/Scene.h"
#include "../EntityManager.h"
#include "../Entities/Player/Player.h"
#include "../Entities/Enemies/MeleeMonster.h"



class InGameScene : public Scene {
private:
    std::shared_ptr<EntityManager> entityManager;
    std::shared_ptr<Player> player;
public:
    InGameScene()
    {
        entityManager = std::make_unique<EntityManager>();
        entityManager->AddEntity(std::make_unique<Player>());


        for (int i = 0; i < 10; i++) {
            entityManager->AddEntity(make_unique<MeleeMonster>(melee_skeleton, Vector2i{ 128,128 }));
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
};
