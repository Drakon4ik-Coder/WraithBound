#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Entities/Player/Player.h"
#include <fstream>

extern std::shared_ptr<Player> player;
using namespace sf;
using namespace std;

constexpr uint16_t gameWidth = 800;
constexpr uint16_t gameHeight = 600;

extern Texture melee_skeleton;