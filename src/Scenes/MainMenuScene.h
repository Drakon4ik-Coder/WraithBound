#pragma once
#include "Scene.h"
#include "../Entities/Player/Player.h"

class MainMenuScene : public Scene {
private:
    std::shared_ptr<Player> player; // Declare player

public:
    MainMenuScene() {
        player = std::make_shared<Player>(&_ents); // Initialize player if needed
    }

    void handleInput(sf::RenderWindow& window) override {
        // Example usage
        if (player) {
            // Handle player input
        }
    }

    void update(float dt) override {
        _ents.Update(dt); // Update entities in the scene
    }

    void render(sf::RenderWindow& window) override {
        _ents.Render(window); // Render entities in the scene
    }
};
