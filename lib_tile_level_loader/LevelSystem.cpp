#include "LevelSystem.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <queue>
#include <vector>
#include <set>

#include "maths.h"

using namespace std;
using namespace sf;

std::unique_ptr<LevelSystem::TILE[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;
Vector2f LevelSystem::_offset(0.0f, 0.0f);

float LevelSystem::_tileSize(50.f);
vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

std::map<LevelSystem::TILE, sf::Color> LevelSystem::_colours = {
    {WALL, Color::White},
    {ENTRANCE, Color::Red},
    {START, Color::Green},
    {EMPTY, Color::Transparent}};

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

float LevelSystem::getTileSize() {
    return _tileSize;
}


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
            auto s = make_unique<RectangleShape>();
            s->setPosition(getTilePosition({x, y}));
            s->setSize(Vector2f(_tileSize, _tileSize));
            s->setFillColor(getColor(getTile({x, y})));
            _sprites.push_back(move(s));
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
    return tile == EMPTY || tile == START || tile == ENTRANCE;
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

const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

bool LevelSystem::inSameRoom(sf::Vector2f e1, sf::Vector2f e2) {
    if(findPath(e1, e2).empty()) {
        return false;
    }
    return true;
}

vector<pair<int, int>> LevelSystem::findPath(sf::Vector2f e1, sf::Vector2f e2) {
    Vector2ul e1Tile = getTileVectPos(e1);
    Vector2ul e2Tile = getTileVectPos(e2);

    pair<int,int> e1P = pair(e1Tile.x, e1Tile.y);
    pair<int,int> e2P = pair(e2Tile.x, e2Tile.y);

    queue<pair<pair<int, int>, vector<pair<int, int>>>> bfsQueue;
    set<pair<int, int>> visited; // To keep track of visited tiles

    // Initialize BFS
    bfsQueue.push({e1P, {}});
    visited.insert(e1P);

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
            pair<int,int> neighbor = pair(newRow, newCol);

            // Check bounds, walls, and if already visited
            if (visited.find(neighbor) == visited.end() && getTile({neighbor.first, neighbor.second}) != TILE::ENTRANCE &&  getTile({neighbor.first, neighbor.second}) != TILE::WALL) {
                visited.insert(neighbor);
                vector<pair<int,int>> newPath = path; // Copy the current path
                newPath.push_back(neighbor);          // Add the neighbor to the path
                bfsQueue.push({neighbor, newPath});
            }
        }
    }

    // Return an empty path if no path exists
    return {};
}

