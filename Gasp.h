#pragma once
#include <functional>
#include <queue>
#include <random>
#include <vector>

#include "raylib.h"

inline std::random_device rd;
inline std::mt19937 gen(rd());

inline float GetRandomFloat(const float min, const float max) {
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(gen);
};

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
    Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
    Vec2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
    Vec2 operator*(const Vec2& other) const { return {x * other.x, y * other.y}; }
    Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
};

class UIDGuy {
public:
    static int HitMe() {
        static int currentId = 0;
        return currentId++;
    }
    UIDGuy() = delete;
};

struct GameObjectData {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float size;
};

struct GameObject {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float size;
    double timeCreated;
    int ID;
};

enum EnemyMovementType {
    Linear,
    Choppy,
    PingPong
};

struct EnemyData {
    GameObjectData gameObjectData;
    EnemyMovementType moveType {Linear};
    float delay;
};

struct LevelEnemyData {
    std::queue<EnemyData> enemyData;
    float timeSinceLastSpawn = 0;
};

// using SpeedAlgorithm = std::function<float(float)>;
// using MovementAlgo = std::function<Vector2(float, Vector2)>;

class Gasp {
private:
    std::vector<GameObject> gameObjects;

    LevelEnemyData currentLevel;

public:
    std::function<float(float)> speedModifierChoppy;
    std::function<float(float)> speedModifierSine;
    std::function<Vector2(float, Vector2)> movementAlgo1;
    Gasp();

    static Vector2 GetRandomEnemyStartPosition(float enemySize);
    int CreateGameObject(const GameObjectData& data);
    void DestroyGameObject(int ID);

    void SpawnEnemy(EnemyData enemyData);

    void Tick();
    // void UpdatePositions();
    void DrawObjects() const;
    // void SetPosition(int ID, const Vector2& newPosition);
    // void SetVelocity(int ID, const Vector2& newVelocity);
    [[nodiscard]] GameObjectData GetGameObjectData(int ID) const;
};