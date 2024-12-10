#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include "../Scenes/Scene.h"

class SceneManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;
    std::shared_ptr<Scene> _activeScene;

public:
    void addScene(const std::string& name, std::shared_ptr<Scene> scene);
    void setActiveScene(const std::string& name);
    std::shared_ptr<Scene> getActiveScene() const;
    void handleInput(sf::RenderWindow& window);
    void update(float dt);
    void render(sf::RenderWindow& window);
};
