#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Entities/Player/Player.h"
#include <fstream>

using namespace sf;
using namespace std;

constexpr uint16_t gameWidth = 1600;
constexpr uint16_t gameHeight = 800;

extern Texture melee_skeleton;
extern Texture player_sword;