#pragma once
#include "../src/Scenes/Scene.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

class SceneManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes; // Map of scenes by name
    std::shared_ptr<Scene> _activeScene; // Pointer to the current active scene

public:
    void addScene(const std::string& name, std::shared_ptr<Scene> scene) {
        _scenes[name] = scene;
        std::cout << "Scene added: " << name << std::endl;
    }

    void setActiveScene(const std::string& name) {
        if (_scenes.find(name) != _scenes.end()) {
            _activeScene = _scenes[name];
            std::cout << "Scene " << name << " found!" << std::endl;
        }
        else {
            std::cout << "Scene " << name << " not found!" << std::endl;
        }
    }

    // Added the getActiveScene function here
    std::shared_ptr<Scene> getActiveScene() const {
        return _activeScene;
    }

    void handleInput(sf::RenderWindow& window) {
        if (_activeScene) _activeScene->handleInput(window);
    }

    void update(float dt) {
        if (_activeScene) _activeScene->update(dt);
    }

    void render(sf::RenderWindow& window) {
        if (_activeScene) _activeScene->render(window);
    }

    void removeScene(const std::string& name) {
        auto it = _scenes.find(name);
        if (it != _scenes.end()) {
            if (_activeScene == it->second) {
                _activeScene.reset();  // Reset active scene if it's being removed
            }
            _scenes.erase(it);
        }
    }

    std::shared_ptr<Scene> getScene(const std::string& name) const {
        auto it = _scenes.find(name);
        if (it != _scenes.end()) {
            return it->second;
        }
        return nullptr;
    }
};
