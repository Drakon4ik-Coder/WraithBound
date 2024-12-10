#include "SceneManager.h"
#include <iostream>

void SceneManager::addScene(const std::string& name, std::shared_ptr<Scene> scene) {
    _scenes[name] = scene;
    std::cout << "Scene added: " << name << std::endl;
}

void SceneManager::setActiveScene(const std::string& name) {
    if (_scenes.find(name) != _scenes.end()) {
        _activeScene = _scenes[name];
        std::cout << "Scene " << name << " found!" << std::endl;
    }
    else {
        std::cout << "Scene " << name << " not found!" << std::endl;
    }
}

std::shared_ptr<Scene> SceneManager::getActiveScene() const {
    return _activeScene;
}

void SceneManager::handleInput(sf::RenderWindow& window) {
    if (_activeScene) _activeScene->handleInput(window);
}

void SceneManager::update(float dt) {
    if (_activeScene) _activeScene->update(dt);
}

void SceneManager::render(sf::RenderWindow& window) {
    if (_activeScene) _activeScene->render(window);
}
