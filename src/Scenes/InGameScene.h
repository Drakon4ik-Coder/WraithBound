#include "../src/Scenes/Scene.h"

class InGameScene : public Scene {
public:
    void handleInput(sf::RenderWindow& window) override {
        // Handle input for in-game logic
    }

    void update(float dt) override {
        // Update in-game entities
        _ents.Update(dt);
    }

    void render(sf::RenderWindow& window) override {
        // Render in-game entities
        _ents.Render(window);
    }
};
