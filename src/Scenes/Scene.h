#pragma once
#include <SFML/Graphics.hpp>
#include "../src/SceneManager/SceneManager.h"
#include "../Entities/Player/Player.h" 

class Scene {
public:
    virtual ~Scene() = default;

    virtual void handleInput(sf::RenderWindow& window) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    // Method to retrieve the Player from the Scene
    virtual std::shared_ptr<Player> getPlayer() const = 0;

    // Method to get Scene Name
    virtual std::string getSceneName() const {
        return typeid(*this).name(); // Use type info as a fallback
    }

protected:
    EntityManager _ents; // Manage entities specific to the scene
};
