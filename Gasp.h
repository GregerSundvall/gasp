#pragma once
#include <random>
#include <vector>

#include "raylib.h"

inline std::random_device rd;
inline std::mt19937 gen(rd());

inline float GetRandomFloat(const float min, const float max) {
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(gen);
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

class Gasp {
private:
    std::vector<GameObject> gameObjects;
public:
    Gasp();


    static Vector2 GetRandomEnemyStartPosition(float enemySize);
    int CreateGameObject(const GameObjectData& data);
    void UpdatePositions();
    void DrawObjects() const;
    void SetPosition(int ID, const Vector2& newPosition);
    void SetVelocity(int ID, const Vector2& newVelocity);
    [[nodiscard]] GameObjectData GetGameObjectData(int ID) const;
};