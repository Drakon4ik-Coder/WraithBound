#include "LevelSystem.h"

#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <stdexcept>
#include <vector>
#include <random>


#include "maths.h"

using namespace std;
using namespace sf;

sf::Texture LevelSystem::_spritesheet;
std::map<LevelSystem::TILE, sf::IntRect> LevelSystem::_textureRects = {
    {WALL, sf::IntRect(128, 0, 32, 32)},
    {ROOM, sf::IntRect(160, 64, 32, 32)},
    {ENTRANCE, sf::IntRect(96, 64, 32, 32)},
    {START, sf::IntRect(32, 32, 32, 32)},
    {EMPTY, sf::IntRect(160, 160, 32, 32)},
    {ENEMY, sf::IntRect(160, 128, 32, 32)}};

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
    {WALL, Color::White},  {ROOM, Color::Transparent},  {ENTRANCE, Color::Red},
    {START, Color::Green}, {EMPTY, Color::Transparent}, {ENEMY, Color::Blue}};

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
            case 'r':
                temp_tiles.push_back(ROOM);
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
            case 'm':
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

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 2);

    for (size_t y = 0; y < getHeight(); ++y) {
        for (size_t x = 0; x < getWidth(); ++x) {
            auto s = std::make_unique<sf::RectangleShape>();
            s->setPosition(getTilePosition({x, y}));
            s->setSize(sf::Vector2f(_tileSize, _tileSize));

            TILE tile = getTile({x, y});
            sf::IntRect textureRect;

            if (tile == ROOM || tile == ENTRANCE || tile == ENEMY) {
                int randomIndex = dist(rng);
                switch (randomIndex) {
                    case 0:
                        textureRect = sf::IntRect(96, 64, 32, 32);
                        break;
                    case 1:
                        textureRect = sf::IntRect(128, 64, 32, 32);
                        break;
                    case 2:
                        textureRect = sf::IntRect(160, 64, 32, 32);
                        break;
                }
            } else {
                textureRect = _textureRects[tile];
            }

            if (_textureRects.find(tile) != _textureRects.end() || tile == ROOM) {
                s->setTexture(&_spritesheet);
                s->setTextureRect(textureRect);
            } else {
                s->setFillColor(getColor(tile));
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

Vector2ul LevelSystem::getTileVectPos(Vector2f v) {
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0) {
        throw out_of_range("Tile out of range");
    }
    return Vector2ul((v - _offset) / _tileSize);
}

bool LevelSystem::isPassable(TILE tile) {
    return tile == EMPTY || tile == START || tile == ENTRANCE || tile == ROOM;
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

const vector<pair<int, int>> directions = {{-1, 0}, {1, 0},  {0, -1}, {0, 1},
                                           {1, 1},  {1, -1}, {-1, 1}, {-1, -1}};

bool LevelSystem::inSameRoom(sf::Vector2f e1, sf::Vector2f e2) {
    if (findPath(e1, e2).empty()) {
        return false;
    }
    return true;
}

vector<pair<int, int>> LevelSystem::findPath(sf::Vector2f e1, sf::Vector2f e2) {
    Vector2ul e1Tile = getTileVectPos(e1);
    Vector2ul e2Tile = getTileVectPos(e2);

    pair<int, int> e1P = pair(e1Tile.x, e1Tile.y);
    pair<int, int> e2P = pair(e2Tile.x, e2Tile.y);

    queue<pair<pair<int, int>, vector<pair<int, int>>>> bfsQueue;
    set<pair<int, int>> visited;  // To keep track of visited tiles

    // Initialize BFS
    if (visited.find(e1P) == visited.end() &&
        getTile({static_cast<size_t>(e1P.first),
                 static_cast<size_t>(e1P.second)}) != TILE::ENTRANCE &&
        getTile({static_cast<size_t>(e1P.first),
                 static_cast<size_t>(e1P.second)}) != TILE::WALL) {
        bfsQueue.push({e1P, {}});
        visited.insert(e1P);
    }

    while (!bfsQueue.empty()) {
        auto [current, path] = bfsQueue.front();
        bfsQueue.pop();

        // Check if we have reached the goal
        if (current == e2P) {
            return path;
        }

        // Explore neighbors
        for (const auto& direction : directions) {
            int newRow = current.first + direction.first;
            int newCol = current.second + direction.second;
            pair<int, int> neighbor = pair(newRow, newCol);

            // Check bounds, walls, and if already visited
            if (visited.find(neighbor) == visited.end() &&
                getTile({static_cast<size_t>(neighbor.first),
                         static_cast<size_t>(neighbor.second)}) !=
                    TILE::ENTRANCE &&
                getTile({static_cast<size_t>(neighbor.first),
                         static_cast<size_t>(neighbor.second)}) != TILE::WALL) {
                visited.insert(neighbor);
                vector<pair<int, int>> newPath = path;  
                newPath.push_back(neighbor);  
                bfsQueue.push({neighbor, newPath});
            }
        }
    }

    // Return an empty path if no path exists
    return {};
}
