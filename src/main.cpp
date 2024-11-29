#include "Main.h"
#include "EntityManager.h"
#include "MeleeMonster.h"
#include "Player.h"

std::shared_ptr<EntityManager> entityManager;
std::shared_ptr<Player> player;

void Load() {
    entityManager = std::make_unique<EntityManager>();
    player = std::make_shared<Player>();

    entityManager->AddEntity(make_unique<MeleeMonster>());
    entityManager->AddEntity(make_unique<MeleeMonster>());
}

void Update(RenderWindow& window) {
    static Clock clock;
    float dt = clock.restart().asSeconds();
    Event event;

    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    // Update all groups
    entityManager->Update(dt);

    // Optionally, update a specific group
    // entityManager->UpdateGroup("Enemies", dt);
}

void Render(RenderWindow& window) {
    // Render all groups
    entityManager->Render(window);

    // Optionally, render a specific group
    // entityManager->RenderGroup("Players", window);
}


int main()
{
    RenderWindow window(VideoMode(gameWidth, gameHeight), "Space Invaders");
    Load();
    while (window.isOpen())
    {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}