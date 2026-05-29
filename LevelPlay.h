#pragma once
#include <functional>
#include <queue>

#include "raylib.h"
#include "Utils.h"


namespace LevelPlay {

    struct GameObjectData {
        Vector2 position;
        Vector2 rotation;
        Vector2 velocity;
        float scale;
        uid shapeID;
    };

    struct GameObject {
        Vector2 position;
        Vector2 rotation;
        Vector2 positionAnimOffset;
        Vector2 velocity;
        float scale;
        uid shapeID;
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

    struct Shape {
        std::vector<Vector2> vertices;
        std::vector<int> indices;
        Color color = WHITE;
        float thickness = 2.0f;
        uid id = Utils::UIDGenerator::GetNewUID();
    };

    struct ShapeDrawData {
        std::vector<Vector2> vertices;
        std::vector<int> indices;
        float thickness = 2.0f;
        Color color = WHITE;
    };

    struct ShapeDrawList {
        uid shapeID;
        std::vector<GameObjectData> instances;
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
        inline static uid playerObjectID;
        inline static std::vector<Shape> shapes;
        inline static std::vector<ShapeDrawList> shapesToDraw;


    public:
        LevelPlay();

        static void Init(const Vector2& position, const Vector2& size);
        static void SetPositionAndSize(Vector2 position, Vector2 size);
        static void AddWave(const EnemyData &enemyData, float delayBeforeFirst, float delayBetween, int enemyCount);
        static void HandleEnemySpawns();
        static Vector2 GetRandomEnemyStartPosition(float enemySize);
        static int CreateGameObject(const GameObjectData& data);
        static void DestroyGameObject(uid ID);
        static void SpawnEnemy(EnemyData enemyData);
        static uid CreateShape(const std::vector<Vector2> &vertices, const std::vector<int> &indices, Color color = WHITE, float thickness = 2.0f);
        static void FrameUpdate();
        static Shape& GetShapeFromID(const uid shapeID);
        static void DrawAll();
        static GameObjectData GetGameObjectData(uid ID);
    };
} // LevelPlay
