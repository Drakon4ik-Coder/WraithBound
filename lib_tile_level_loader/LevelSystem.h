#pragma once

#include <../lib_maths/maths.h>

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

#define ls LevelSystem

using Vector2ul = sf::Vector2<size_t>;

class LevelSystem {
   public:
    enum TILE { EMPTY, START, ENTRANCE, WALL, ENEMY };

    static void loadLevelFile(const std::string&, float tileSize = 50.f);
    static void Render(sf::RenderWindow& window);
    static sf::Color getColor(TILE t);
    static void setColor(TILE t, sf::Color c);
    //Get Tile at grid coordinate
    static TILE getTile(sf::Vector2ul);
    //Get Screenspace coordinate of tile
    static sf::Vector2f getTilePosition(sf::Vector2ul);

    static sf::Vector2ul getTileVectPos(sf::Vector2f p);
    //get the tile at screenspace pos
    static TILE getTileAt(sf::Vector2f);
    static std::vector<std::pair<int, int>>findPath(sf::Vector2f e1, sf::Vector2f e2);
    static bool inSameRoom(sf::Vector2f e1, sf::Vector2f e2);
    static bool isPassable(TILE tile);

    static int getWidth();
    static int getHeight();
    static float getTileSize();
    static std::vector<Vector2ul> getMonsterSpawnPoints();

   protected:
    static std::unique_ptr<TILE[]> _tiles;  // Internal array of tiles
    static size_t _width;                   // how many tiles wide is level
    static size_t _height;                  // how many tile high is level
    static sf::Vector2f _offset;            // Screenspace offset of level
    static float _tileSize;                 // Screenspace size of each tile
    static std::map<TILE, sf::Color>
        _colours;  // color to render each tile type

    // array of sfml sprites of each tile
    static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites;
    // generate the _sprites array
    static void buildSprites();

   private:
    LevelSystem() = delete;
    ~LevelSystem() = delete;
};
