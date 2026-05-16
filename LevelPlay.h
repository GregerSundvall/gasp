#pragma once
#include <functional>
#include <queue>

#include "raylib.h"
#include "Utils.h"


namespace LevelPlay {

    struct GameObjectData {
        Vector2 position;
        Vector2 velocity;
        Color color;
        float size;
    };

    struct GameObject {
        Vector2 position;
        Vector2 positionAnimOffset;
        Vector2 velocity;
        Color color;
        float size;
        double timeCreated;
        int ID = Utils::UIDGenerator::GetNewUID();
    };

    enum EnemyMovementType {
        Linear,
        Choppy,
        PingPong
    };

    struct EnemyData {
        GameObjectData gameObjectData;
        EnemyMovementType moveType {Linear};
    };

    struct EnemyWaveData {
        EnemyData enemyData;
        float delayBeforeFirstSpawn = 0;
        float delayBetweenSpawns = 1;
        int enemyCount = 1;
        int enemiesSpawned = 0;
    };

    struct LevelWaveData {
        std::vector<EnemyWaveData> enemyWaves;
        int wavesSpawned = 0;
        float timeSinceLastSpawn = 0;
    };

    // using SpeedAlgorithm = std::function<float(float)>;
    // using MovementAlgo = std::function<Vector2(float, Vector2)>;

    class LevelPlay {
    private:
        std::vector<GameObject> gameObjects;
        LevelWaveData levelWaveData {};

    public:
        std::function<float(float)> speedModifierChoppy;
        std::function<float(float)> speedModifierSine;
        std::function<Vector2(float, Vector2)> movementAlgo1;

        LevelPlay();

        void AddWave(const EnemyData &enemyData, float delayBeforeFirst, float delayBetween, int enemyCount);

        static Vector2 GetRandomEnemyStartPosition(float enemySize);
        int CreateGameObject(const GameObjectData& data);
        void DestroyGameObject(int ID);

        void SpawnEnemy(EnemyData enemyData);

        void FrameUpdate();
        void DrawAll();
        [[nodiscard]] GameObjectData GetGameObjectData(int ID) const;
    };
} // LevelPlay
