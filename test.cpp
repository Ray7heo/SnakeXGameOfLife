// #include "lib/RayLib/raylib.h"
// #include <vector>
// #include <queue>
// #include <unordered_map>
// #include <algorithm>
//
// #define TILE_SIZE 30
// #define GRID_WIDTH 50
// #define GRID_HEIGHT 30
//
// struct Node {
//     Vector2 position;
//     Node* parent;
//     int gCost;
//     int hCost;
//     int fCost() const { return gCost + hCost; }
// };
//
// bool operator==(const Vector2& a, const Vector2& b) {
//     return (a.x == b.x && a.y == b.y);
// }
//
//
//
// bool operator==(const Node& a, const Node& b) {
//     return a.position.x == b.position.x && a.position.y == b.position.y;
// }
//
// namespace std {
//     template <>
//     struct hash<Vector2> {
//         size_t operator()(const Vector2& vec) const {
//             return hash<int>()(vec.x) ^ hash<int>()(vec.y);
//         }
//     };
// }
//
// std::vector<Vector2> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
//
// class SnakeGame {
// public:
//     SnakeGame();
//     ~SnakeGame();
//
//     void Update();
//     void Draw();
//
// private:
//     std::vector<Vector2> snake;
//     Vector2 food;
//     bool gameOver;
//     std::vector<Vector2> path;
//
//     void GenerateFood();
//     std::vector<Vector2> FindPath(const Vector2& start, const Vector2& end);
// };
//
// SnakeGame::SnakeGame() : gameOver(false) {
//     InitWindow(GRID_WIDTH * TILE_SIZE, GRID_HEIGHT * TILE_SIZE, "Snake Game");
//     SetTargetFPS(10);
//
//     // Initialize snake
//     snake.push_back({GRID_WIDTH / 2, GRID_HEIGHT / 2});
//
//     GenerateFood();
// }
//
// SnakeGame::~SnakeGame() {
//     CloseWindow();
// }
//
//
//
// void SnakeGame::Update() {
//     if (!gameOver) {
//         path = FindPath(snake[0], food);
//
//         // Move the snake
//         if (!path.empty() && path.size() >= 2) {
//             snake.insert(snake.begin(), path[1]); // 将蛇的头部移动到路径的第二个位置
//             if (snake[0].x == food.x && snake[0].y == food.y) {
//                 GenerateFood();
//             } else {
//                 snake.pop_back();
//             }
//         } else {
//             // 如果路径为空或长度不足2，则随机移动蛇的头部
//             int direction = GetRandomValue(0, 3);
//             Vector2 nextPos = {snake[0].x + directions[direction].x, snake[0].y + directions[direction].y};
//             snake.insert(snake.begin(), nextPos);
//             snake.pop_back();
//         }
//
//         // Check collision with walls or self
//         Vector2& head = snake[0];
//         if (head.x < 0 || head.x >= GRID_WIDTH || head.y < 0 || head.y >= GRID_HEIGHT) {
//             gameOver = true;
//         } else {
//             for (size_t i = 1; i < snake.size(); ++i) {
//                 if (head.x == snake[i].x && head.y == snake[i].y) {
//                     gameOver = true;
//                     break;
//                 }
//             }
//         }
//     }
// }
//
// void SnakeGame::Draw() {
//     BeginDrawing();
//     ClearBackground(RAYWHITE);
//
//     // Draw food
//     DrawRectangle(food.x * TILE_SIZE, food.y * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
//
//     // Draw snake
//     for (const auto& segment : snake) {
//         DrawRectangle(segment.x * TILE_SIZE, segment.y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLACK);
//     }
//
//     if (gameOver) {
//         DrawText("Game Over!", 10, 10, 20, BLACK);
//     }
//
//     EndDrawing();
// }
//
// void SnakeGame::GenerateFood() {
//     bool validLocation = false;
//     while (!validLocation) {
//         food.x = GetRandomValue(0, GRID_WIDTH - 1);
//         food.y = GetRandomValue(0, GRID_HEIGHT - 1);
//         validLocation = true;
//         for (const auto& segment : snake) {
//             if (segment.x == food.x && segment.y == food.y) {
//                 validLocation = false;
//                 break;
//             }
//         }
//     }
// }
//
// std::vector<Vector2> SnakeGame::FindPath(const Vector2& start, const Vector2& end) {
//     // Implement A* algorithm to find path from start to end
//     std::vector<Vector2> result;
//     std::vector<Node> openList;
//     std::unordered_map<Vector2, Node> nodeMap;
//
//     int hCost = static_cast<int>(abs(start.x - end.x) + abs(start.y - end.y)); // 将浮点数强制转换为整数
//
//     Node startNode = {start, nullptr, 0, hCost};
//     openList.push_back(startNode);
//     nodeMap[start] = startNode;
//
//     while (!openList.empty()) {
//         // Find node with lowest fCost in openList
//         auto currentNode = std::min_element(openList.begin(), openList.end(), [](const Node& a, const Node& b) {
//             return a.fCost() < b.fCost();
//         });
//
//         // Move current node from open list to closed list
//         Node current = *currentNode;
//         openList.erase(currentNode);
//
//         // Check if current node is end
//         if (current.position.x == end.x && current.position.y == end.y) {
//             // Reconstruct path
//             result.push_back(current.position);
//             while (current.parent != nullptr) {
//                 current = *current.parent;
//                 result.push_back(current.position);
//             }
//             std::reverse(result.begin(), result.end());
//             break;
//         }
//
//         // Generate children for current node
//         for (const auto& dir : directions) {
//             Vector2 nextPos = {current.position.x + dir.x, current.position.y + dir.y};
//             // Check if next position is within bounds and not part of snake
//             if (nextPos.x >= 0 && nextPos.x < GRID_WIDTH && nextPos.y >= 0 && nextPos.y < GRID_HEIGHT) {
//                 bool validMove = true;
//                 for (const auto& segment : snake) {
//                     if (segment.x == nextPos.x && segment.y == nextPos.y) {
//                         validMove = false;
//                         break;
//                     }
//                 }
//                 if (validMove) {
//                     int gCost = current.gCost + 1;
//                     int hCost = static_cast<int>(abs(nextPos.x - end.x) + abs(nextPos.y - end.y)); // 将浮点数强制转换为整数
//                     Node newNode = {nextPos, &nodeMap[current.position], gCost, hCost};
//                     auto it = nodeMap.find(nextPos);
//                     if (it == nodeMap.end() || gCost < it->second.gCost) {
//                         nodeMap[nextPos] = newNode;
//                         openList.push_back(newNode);
//                     }
//                 }
//             }
//         }
//     }
//
//     return result;
// }
//
// int main() {
//     SnakeGame game;
//
//     while (!WindowShouldClose()) {
//         game.Update();
//         game.Draw();
//     }
//
//     return 0;
// }
