#pragma once
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

class Animator {
    int targetID;
    float maxValue;
    float minValue;

    float currentValue;

    void Tick() {
        currentValue = 0.5f + 0.5f * std::sin(GetTime());
    }

    float GetCurrentValue(){ return currentValue; }
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
    int ID;
};

struct EnemyData {
    GameObjectData gameObjectData;
    float delay;
};

struct LevelEnemyData {
    std::queue<EnemyData> enemyData;
    float timeSinceLastSpawn = 0;
};

class Gasp {
private:
    std::vector<GameObject> gameObjects;
    std::vector<int> iDToIndexLookup;
    std::queue<int> freeIDs;

    std::vector<Animator> animators;

    LevelEnemyData currentLevel;
public:
    Gasp();


    static Vector2 GetRandomEnemyStartPosition(float enemySize);
    int CreateGameObject(const GameObjectData& data);
    void DestroyGameObject(int ID);

    void SpawnEnemy(EnemyData enemyData);

    void Tick();
    void UpdatePositions();
    void DrawObjects() const;
    void SetPosition(int ID, const Vector2& newPosition);
    void SetVelocity(int ID, const Vector2& newVelocity);
    [[nodiscard]] GameObjectData GetGameObjectData(int ID) const;
};