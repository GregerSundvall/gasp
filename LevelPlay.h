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
        float animDesync = Utils::GetRandomFloat((0.0f), (1.0f));
        uid ID = Utils::UIDGenerator::GetNewUID();
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
        inline static std::vector<GameObject> gameObjects;
        inline static LevelWaveData levelWaveData {};
        inline static std::function<float(float)> speedModifierChoppy;
        inline static std::function<float(float)> speedModifierSine;
        inline static std::function<Vector2(float, Vector2)> movementAlgo1;
        inline static Vector2 containerPosition {};
        inline static Vector2 containerSize {};

    public:
        LevelPlay();

        static void SetPositionAndSize(Vector2 position, Vector2 size);
        static void AddWave(const EnemyData &enemyData, float delayBeforeFirst, float delayBetween, int enemyCount);
        static void HandleEnemySpawns();
        static Vector2 GetRandomEnemyStartPosition(float enemySize);
        static int CreateGameObject(const GameObjectData& data);
        static void DestroyGameObject(uid ID);
        static void SpawnEnemy(EnemyData enemyData);
        static void FrameUpdate();
        static void DrawAll();
        static GameObjectData GetGameObjectData(uid ID);
    };
} // LevelPlay
