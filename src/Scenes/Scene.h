#pragma once
#include <SFML/Graphics.hpp>

#include "../src/SceneManager/SceneManager.h"

class Scene {
   public:
    virtual ~Scene() = default;

    // Provide a default implementation for getPlayer
    virtual std::shared_ptr<Player> getPlayer() const {
        return nullptr;  // Default: No player in the scene
    }
    virtual void onActivate() = 0;  // Pure virtual function
    virtual void handleInput(sf::RenderWindow& window) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

   protected:
    EntityManager _ents;  // Manage entities specific to the scene
};
