#pragma once
#include <SFML/Graphics.hpp>
#include "../src/SceneManager/SceneManager.h"

class Scene {
public:
    virtual ~Scene() = default;

    virtual void handleInput(sf::RenderWindow& window) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

protected:
    EntityManager _ents; // Manage entities specific to the scene
};