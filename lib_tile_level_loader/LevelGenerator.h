#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <sstream>
#include <vector>

using namespace std;

const int TEMPLATE_SIZE = 15;  // Each template (room, tunnel, blank) is 15x15
const int DUNGEON_WIDTH = 5;   // Dungeon grid width
const int DUNGEON_HEIGHT = 5;  // Dungeon grid height

// Function Declarations
bool validateDungeonLayout(const vector<vector<char>>& layout);
vector<string> readTemplateFromFile(const string& filePath);
vector<vector<vector<string>>> expandDungeon(
    const vector<vector<char>>& layout);
vector<vector<char>> generateDungeonLayout(int width, int height,
                                           int roomCount);

// Read layout template from file
vector<string> readTemplateFromFile(const string& filePath) {
    vector<string> layout;
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return layout;
    }
    string line;
    while (getline(file, line)) {
        layout.push_back(line);
    }
    return layout;
}

// Write the expanded dungeon to a file
void writeExpandedDungeonToFile(
    const vector<vector<vector<string>>>& expandedDungeon,
    const string& filePath) {
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not write to file " << filePath << endl;
        return;
    }

    for (size_t gridY = 0; gridY < expandedDungeon.size(); ++gridY) {
        for (size_t templateLine = 0; templateLine < TEMPLATE_SIZE;
             ++templateLine) {
            for (size_t gridX = 0; gridX < expandedDungeon[gridY].size();
                 ++gridX) {
                const auto& templateContent = expandedDungeon[gridY][gridX];
                if (templateLine < templateContent.size()) {
                    file << templateContent[templateLine];
                } else {
                    file << string(TEMPLATE_SIZE,
                                   ' ');  // Fill blank if the line is missing
                }
            }
            file << endl;
        }
    }
}

void printDungeonLayout(const vector<vector<char>>& layout) {
    for (const auto& row : layout) {
        for (char cell : row) {
            cout << cell;
        }
        cout << endl;
    }
}

// Generate 2D Dungeon Layout
vector<vector<char>> generateDungeonLayout(int width, int height,
                                           int roomCount) {
    vector<vector<char>> layout(height, vector<char>(width, ' '));
    srand(static_cast<unsigned>(time(0)));

    // List of existing room coordinates
    vector<pair<int, int>> roomPositions;

    // Place the first room at a random even coordinate
    int startX = (rand() % ((width + 1) / 2)) * 2;
    int startY = (rand() % ((height + 1) / 2)) * 2;
    layout[startY][startX] = 'r';
    roomPositions.push_back({startY, startX});

    // Directions for placement (N, S, W, E)
    vector<pair<int, int>> directions = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};

    // Place additional rooms
    for (int i = 1; i < roomCount; ++i) {
        bool placed = false;

        while (!placed) {
            // Pick a random room from the list
            auto [currentY, currentX] =
                roomPositions[rand() % roomPositions.size()];

            // Shuffle directions to randomize placement
            std::random_device rd;
            std::default_random_engine rng(rd());
            std::shuffle(directions.begin(), directions.end(), rng);

            // Try placing a new room in a valid direction
            for (const auto& [dy, dx] : directions) {
                int newY = currentY + dy;
                int newX = currentX + dx;

                // Check if the position is valid and within bounds
                if (newY >= 0 && newY < height && newX >= 0 && newX < width &&
                    layout[newY][newX] == ' ') {
                    // Place the room
                    layout[newY][newX] = 'r';
                    roomPositions.push_back({newY, newX});

                    // Connect the new room to the current room with a tunnel
                    int tunnelY = (currentY + newY) / 2;
                    int tunnelX = (currentX + newX) / 2;

                    // Determine tunnel type
                    layout[tunnelY][tunnelX] = (dy != 0) ? 'v' : 'h';

                    placed = true;
                    break;  // Exit direction loop
                }
            }
        }
    }

    // Randomly select a room to replace with spawn (s)
    if (!roomPositions.empty()) {
        int spawnIndex = rand() % roomPositions.size();
        auto [spawnY, spawnX] = roomPositions[spawnIndex];
        layout[spawnY][spawnX] = 's';  // Place spawn room
    }

    return layout;
}

// Validate dungeon layout to ensure all rooms are reachable
bool validateDungeonLayout(const vector<vector<char>>& layout) {
    int height = layout.size();
    int width = layout[0].size();

    // Locate all rooms and tunnels
    vector<pair<int, int>> roomPositions;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (layout[y][x] == 'r' || layout[y][x] == 's') {
                roomPositions.push_back({y, x});
            }
        }
    }

    // Perform BFS to ensure all rooms are connected
    vector<vector<bool>> visited(height, vector<bool>(width, false));
    queue<pair<int, int>> q;

    if (!roomPositions.empty()) {
        q.push(roomPositions[0]);
        visited[roomPositions[0].first][roomPositions[0].second] = true;
    }

    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int connectedRooms = 0;

    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();
        connectedRooms++;

        for (const auto& [dy, dx] : directions) {
            int newY = y + dy;
            int newX = x + dx;

            if (newY >= 0 && newY < height && newX >= 0 && newX < width &&
                !visited[newY][newX] &&
                (layout[newY][newX] == 'r' || layout[newY][newX] == 's' ||
                 layout[newY][newX] == 'v' || layout[newY][newX] == 'h')) {
                visited[newY][newX] = true;
                q.push({newY, newX});
            }
        }
    }

    return connectedRooms == roomPositions.size();
}

// Expand the dungeon based on the layout
vector<vector<vector<string>>> expandDungeon(
    const vector<vector<char>>& layout) {
    vector<vector<vector<string>>> expandedDungeon(
        layout.size(), vector<vector<string>>(layout[0].size()));

    for (size_t y = 0; y < layout.size(); ++y) {
        for (size_t x = 0; x < layout[y].size(); ++x) {
            if (layout[y][x] == 'r') {
                expandedDungeon[y][x] =
                    readTemplateFromFile("../res/levels/room.txt");
            } else if (layout[y][x] == 's') {
                expandedDungeon[y][x] =
                    readTemplateFromFile("../res/levels/room_spawn.txt");
            } else if (layout[y][x] == 'v') {
                expandedDungeon[y][x] =
                    readTemplateFromFile("../res/levels/tunnelV.txt");
            } else if (layout[y][x] == 'h') {
                expandedDungeon[y][x] =
                    readTemplateFromFile("../res/levels/tunnelH.txt");
            } else {
                expandedDungeon[y][x] =
                    readTemplateFromFile("../res/levels/blank.txt");
            }
        }
    }

    return expandedDungeon;
}

void generateLevelToFile() {
    int roomCount = 6;

    // Generate dungeon layout
    vector<vector<char>> dungeon =
        generateDungeonLayout(DUNGEON_WIDTH, DUNGEON_HEIGHT, roomCount);

    // Expand dungeon
    vector<vector<vector<string>>> expandedDungeon = expandDungeon(dungeon);

    // Write expanded dungeon to file
    writeExpandedDungeonToFile(expandedDungeon, "../res/levels/maze.txt");
    cout << "Dungeon written to maze.txt" << endl;
    printDungeonLayout(dungeon);
    // Validate layout
    if (!validateDungeonLayout(dungeon)) {
        cerr << "Dungeon layout is invalid: not all rooms are reachable."
             << endl;
    }
}
