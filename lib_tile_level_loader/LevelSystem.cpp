#include "LevelSystem.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "maths.h"

using namespace std;
using namespace sf;

sf::Texture LevelSystem::_spritesheet;
std::map<LevelSystem::TILE, sf::IntRect> LevelSystem::_textureRects = {
    {WALL, sf::IntRect(128, 0, 32, 32)},
    {ENTRANCE, sf::IntRect(96, 64, 32, 32)},
    {START, sf::IntRect(32, 32, 32, 32)},
    {EMPTY, sf::IntRect(160, 128, 32, 32)},
    {ENEMY, sf::IntRect(160, 128, 32, 32)}  // Modify as per your sprite sheet
};

std::unique_ptr<LevelSystem::TILE[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;
Vector2f LevelSystem::_offset(0.0f, 0.0f);

float LevelSystem::_tileSize(25.f);
vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

void LevelSystem::loadSpritesheet(const std::string& path) {
    if (!_spritesheet.loadFromFile("res/img/Dungeon/dungeon.png")) {
        throw std::runtime_error("Failed to load spritesheet: " + path);
    }
}

std::map<LevelSystem::TILE, sf::Color> LevelSystem::_colours = {
    {WALL, Color::White},
    {ENTRANCE, Color::Red},
    {START, Color::Green},
    {EMPTY, Color::Transparent},
    {ENEMY, Color::Blue}};

sf::Color LevelSystem::getColor(LevelSystem::TILE t) {
    auto it = _colours.find(t);
    if (it == _colours.end()) {
        _colours[t] = Color::Transparent;
    }
    return _colours[t];
}

void LevelSystem::setColor(LevelSystem::TILE t, sf::Color c) {
    _colours[t] = c;
}

float LevelSystem::getTileSize() { return _tileSize; }

void LevelSystem::loadLevelFile(const std::string& path, float tileSize) {
    _tileSize = tileSize;
    size_t w = 0, h = 0;
    string buffer;

    ifstream f(path);
    if (f.good()) {
        f.seekg(0, std::ios::end);
        buffer.resize(f.tellg());
        f.seekg(0);
        f.read(&buffer[0], buffer.size());
        f.close();
    } else {
        throw runtime_error("Couldn't open level file: " + path);
    }

    vector<TILE> temp_tiles;
    for (size_t i = 0; i < buffer.size(); ++i) {
        const char c = buffer[i];
        switch (c) {
            case 'w':
                temp_tiles.push_back(WALL);
                break;
            case 's':
                temp_tiles.push_back(START);
                break;
            case 'e':
                temp_tiles.push_back(ENTRANCE);
                break;
            case ' ':
                temp_tiles.push_back(EMPTY);
                break;
            case 'm':  // Add support for monster tiles
                temp_tiles.push_back(ENEMY);
                break;
            case '\n':
                if (w == 0) {
                    w = i;
                }
                h++;
                break;
            default:
                cerr << "Unknown tile type in level file: " << c << endl;
        }
    }
    if (temp_tiles.size() != w * h) {
        throw runtime_error("Level file parsing error: " + path);
    }

    _tiles = std::make_unique<TILE[]>(w * h);
    _width = w;
    _height = h;
    std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);

    cout << "Level " << path << " loaded: " << w << "x" << h << endl;
    buildSprites();
}


void LevelSystem::buildSprites() {
    _sprites.clear();

    for (size_t y = 0; y < getHeight(); ++y) {
        for (size_t x = 0; x < getWidth(); ++x) {
            auto s = std::make_unique<sf::RectangleShape>();
            s->setPosition(getTilePosition({x, y}));
            s->setSize(sf::Vector2f(_tileSize, _tileSize));

            TILE tile = getTile({x, y});
            auto& textureRect = _textureRects[tile];

            // Set texture rect for the specific tile type
            if (_textureRects.find(tile) != _textureRects.end()) {
                s->setTexture(&_spritesheet);  // Set the spritesheet texture
                s->setTextureRect(textureRect);  // Set the specific tile region
            } else {
                s->setFillColor(getColor(tile));  // Default to color if no texture
            }

            _sprites.push_back(std::move(s));
        }
    }
}



int LevelSystem::getWidth() { return _width; }

int LevelSystem::getHeight() { return _height; }

Vector2f LevelSystem::getTilePosition(Vector2ul p) {
    return Vector2f(p.x, p.y) * _tileSize;
}

LevelSystem::TILE LevelSystem::getTile(Vector2ul p) {
    if (p.x >= _width || p.y >= _height) {
        throw out_of_range("Tile out of range: " + to_string(p.x) + ", " +
                           to_string(p.y));
    }
    return _tiles[(p.y * _width) + p.x];
}
bool LevelSystem::isPassable(TILE tile) {
    return tile == EMPTY || tile == START || tile == ENTRANCE;
}

std::vector<sf::Vector2ul> LevelSystem::getMonsterSpawnPoints() {
    std::vector<sf::Vector2ul> spawnPoints;
    for (size_t y = 0; y < 75; ++y) {
        for (size_t x = 0; x < 75; ++x) {
            if (getTile({x, y}) == ENEMY) {     // Check for ENEMY tile
                spawnPoints.push_back({x, y});  // Add to spawn list
            }
        }
    }
    return spawnPoints;
}

LevelSystem::TILE LevelSystem::getTileAt(Vector2f v) {
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0) {
        throw out_of_range("Tile out of range");
    }
    return getTile(Vector2ul((v - _offset) / _tileSize));
}

void LevelSystem::Render(RenderWindow& window) {
    for (auto& sprite : _sprites) {
        window.draw(*sprite);
    }
}
