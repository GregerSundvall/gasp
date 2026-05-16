#pragma once
#include <functional>
#include <queue>

#include "raylib.h"


namespace LevelPlay {

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
        float spawnDelay {0};
    };

    struct LevelEnemyData {
        std::queue<EnemyData> enemyData;
        float timeSinceLastSpawn = 0;
    };

    // using SpeedAlgorithm = std::function<float(float)>;
    // using MovementAlgo = std::function<Vector2(float, Vector2)>;

    class LevelPlay {
    private:
        std::vector<GameObject> gameObjects;
        LevelEnemyData currentLevel;

    public:
        std::function<float(float)> speedModifierChoppy;
        std::function<float(float)> speedModifierSine;
        std::function<Vector2(float, Vector2)> movementAlgo1;

        LevelPlay();
        static Vector2 GetRandomEnemyStartPosition(float enemySize);
        int CreateGameObject(const GameObjectData& data);
        void DestroyGameObject(int ID);

        void SpawnEnemy(EnemyData enemyData);

        void FrameUpdate();
        void DrawAll();
        [[nodiscard]] GameObjectData GetGameObjectData(int ID) const;
    };
} // LevelPlay
