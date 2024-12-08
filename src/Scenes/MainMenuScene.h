#include "../src/Scenes/Scene.h"

class MainMenuScene : public Scene {
public:
    void handleInput(sf::RenderWindow& window) override {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            std::cout << "Start game!" << std::endl;
        }
    }

    void update(float dt) override {
        // Update logic for main menu
    }

    void render(sf::RenderWindow& window) override {
        window.clear(sf::Color::Black);
        // Render menu items
    }

    std::shared_ptr<Player> getPlayer() const {
        return player; // Return the player instance stored in this scene
    }
};
