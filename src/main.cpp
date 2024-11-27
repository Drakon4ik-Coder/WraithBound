#include "Main.h"
#include "MeleeMonster.h"

using namespace sf;

std::unique_ptr<Player> player;

std::unique_ptr<MeleeMonster> monster;

void Load()
{
    player = std::make_unique<Player>();
    monster = std::make_unique<MeleeMonster>();
}
void Update(RenderWindow &window)
{
    static Clock clock;
    float dt = clock.restart().asSeconds();
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            window.close();
            return;
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    player->Update(dt);
    monster->Update(dt);
}
void Render(RenderWindow &window)
{
    player->Render(window);
    monster->Render(window);
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