
#include "LevelPlay.h"

#include "raymath.h"
#include "Utils.h"

namespace LevelPlay {
    LevelPlay::LevelPlay() {
        gameObjects.reserve(1000);
        shapesToDraw.reserve(1000);
    }

    void LevelPlay::Init(const Vector2 &position, const Vector2 &size) {
        SetPositionAndSize(position, size);

        float playerDistanceFromBottom = 25.0f;
        Vector2 playerPos = {containerSize.x / 2, containerSize.y - playerDistanceFromBottom};
        std::vector<Vector2> playerVertices {{0.0f, -10.0f}, {-5.0f, 10.0f}, {5.0f, 10.0f}};
        std::vector<int> playerIndices {{0, 1, 2, 0}};
        uid playerShapeID = CreateShape(playerVertices, playerIndices);
        float playerScale = 2.0f;
        Vector2 rotationUp = {0.0f, 1.0f};
        GameObjectData playerData = {playerPos, rotationUp, Vector2Zero(), playerScale, playerShapeID};
        playerObjectID = CreateGameObject(playerData);

        Color enemyGreen {80, 235, 121, 255};
        Color enemyPurple {98, 70, 212, 255};
        Color enemyRed {222, 42, 135, 255};
        std::vector<Vector2> enemyVertices {
            {0.0f, 5.0f},
            {-5.0f, 10.0f},
            {0.0f, -10.0f},
            {5.0f, 10.0f}};
        std::vector<int> enemyIndices {{0, 1, 2, 3, 0}};
        uid enemyShapeGreenID = CreateShape(enemyVertices, enemyIndices, enemyGreen);
        uid enemyShapePurpleID = CreateShape(enemyVertices, enemyIndices, enemyPurple);
        uid enemyShapeRedID = CreateShape(enemyVertices, enemyIndices, enemyRed);
        float enemyScale = 2.0f;
        Vector2 rotationDown = {0.0f, -1.0f};
        // GameObjectData enemyData = {Vector2Zero(), rotationDown, Vector2Zero(), WHITE, enemyScale, enemyShapeID};
        EnemyData enemyType1 = {{Vector2Zero(), rotationDown, 0.1f, 0.8f,
                                enemyScale, enemyShapeGreenID}, Linear};
        AddWave(enemyType1, 0.0f, 2.0f, 5);

        EnemyData enemyType2 = {{Vector2Zero(), rotationDown, 0.1f, 1.4f,
                                enemyScale, enemyShapePurpleID}, Linear};
        AddWave(enemyType2, 2.0f, 2.0f, 5);

        EnemyData enemyType3 = {{Vector2Zero(), rotationDown, 0.1f, 2.0f,
                                enemyScale, enemyShapeRedID}, Linear};
        AddWave(enemyType3, 2.0f, 2.0f, 5);

        // speedModifier = [](float time) { return 1.0f + (time * 0.1f); };
        // speedModifierPingPong = [](float time) { return 0.5f + 0.5f * std::sin(time); };
        speedModifierSine = [](float time) { return std::sin(time); };
        speedModifierChoppy = [](float time) { return static_cast<int>(time) % 2 == 0 ? 1.5f : 0.5f; };
        movementAlgo1 = [](float time, Vector2 velocity) {
            float x = static_cast<int>(time) % 2 == 0 ? 1.0f : 0.0f;
            float y = static_cast<int>(time) % 2 == 0 ? 1.5f : 0.5f;
            return Vector2{x, y};
        };
    }

    void LevelPlay::SetPositionAndSize(Vector2 position, Vector2 size) {
        containerPosition = position;
        containerSize = size;
    }


    void LevelPlay::AddWave(const EnemyData& enemyData, float delayBeforeFirst, float delayBetween, const int enemyCount) {
        levelWaveData.enemyWaves.emplace_back();
        EnemyWaveData& waveData = levelWaveData.enemyWaves.back();
        waveData.enemyData = enemyData;
        waveData.delayBeforeFirstSpawn = delayBeforeFirst;
        waveData.delayBetweenSpawns = delayBetween;
        waveData.enemyCount = enemyCount;
    }

    void LevelPlay::HandleEnemySpawns(){
        if (levelWaveData.enemyWaves.empty()) {
            return;
        }
        if (levelWaveData.wavesSpawned == levelWaveData.enemyWaves.size()) {
            return;
        }
        EnemyWaveData& currentWave = levelWaveData.enemyWaves[levelWaveData.wavesSpawned];
        if (currentWave.enemiesSpawned == currentWave.enemyCount) {
            return;
        }

        float spawnDelay = currentWave.enemiesSpawned == 0 ? currentWave.delayBeforeFirstSpawn : currentWave.delayBetweenSpawns;
        if (levelWaveData.timeSinceLastSpawn >= spawnDelay) {
            SpawnEnemy(currentWave.enemyData);
            levelWaveData.timeSinceLastSpawn = 0;
            currentWave.enemiesSpawned++;
            if (currentWave.enemiesSpawned == currentWave.enemyCount) {
                levelWaveData.wavesSpawned++;
            }
        } else {
            levelWaveData.timeSinceLastSpawn += Utils::ClampedDeltaTime(); // TODO: REVERT THIS, TEMPORARY DEBUGGING PURPOSES
            // levelWaveData.timeSinceLastSpawn += GetFrameTime();
        }
    }

    void LevelPlay::FrameUpdate() {
        HandleEnemySpawns();

        //Calculate velocities separately first?

        // Options for rotation follows velocity, or velocity follows rotation..?

        // Move everyone
        double time = GetTime();
        for (int i = 0; i < gameObjects.size(); ++i) {
            GameObject& object = gameObjects[i];
            object.position.x += object.velocity.x * speedModifierSine(time + object.animDesync);
            object.position.y += object.velocity.y * speedModifierChoppy(time + object.animDesync);
        }

        // Enemies bounds check
        std::vector<uid> idsToDestroy;
        for (GameObject object : gameObjects) {
            if (object.position.y > containerSize.y + object.scale) {
                idsToDestroy.emplace_back(object.ID);
            }
        }

        // Destroy out of bounds objects
        while (!idsToDestroy.empty()) {
            DestroyGameObject(idsToDestroy.back());
            idsToDestroy.pop_back();
        }
    }

    Vector2 LevelPlay::GetRandomEnemyStartPosition(const float enemySize) {
        Vector2 startPos = Vector2Zero();
        float minXPos = enemySize;
        float maxXPos = containerSize.x - enemySize;
        startPos.y = -enemySize;
        startPos.x = Utils::GetRandomFloat(minXPos, maxXPos);
        return startPos;
    }

    // Creates a gameObject and returns gameObject ID.
    int LevelPlay::CreateGameObject(const GameObjectData& data) {
        gameObjects.emplace_back();
        GameObject& object = gameObjects.back();
        object.position = data.position;
        object.rotation = data.rotation;
        object.velocity = data.velocity;
        object.scale = data.scale;
        object.shapeID = data.shapeID;
        return object.ID;
    }

    void LevelPlay::DestroyGameObject(const uid ID) {
        int indexToRemove = -1;
        for (int i = 0 ; i <  gameObjects.size(); ++i) {
            if (gameObjects[i].ID == ID) {
                indexToRemove = i;
                break;
            }
        }
        if (indexToRemove == -1) {return;}

        if (indexToRemove < gameObjects.size() -1) {
            gameObjects[indexToRemove] = gameObjects.back();
        }
        gameObjects.pop_back();
    }

    void LevelPlay::SpawnEnemy(EnemyData enemyData) {
        enemyData.gameObjectData.position = GetRandomEnemyStartPosition(enemyData.gameObjectData.scale);
        CreateGameObject(enemyData.gameObjectData);
    }

    uid LevelPlay::CreateShape(const std::vector<Vector2> &vertices, const std::vector<int> &indices, Color color, float thickness) {
        shapes.emplace_back(Shape(vertices, indices, color, thickness));
        return shapes.back().id;
    }

    Shape& LevelPlay::GetShapeFromID(const uid shapeID) {
        for (Shape& shape : shapes) {
            if (shape.id == shapeID) {
                return shape;
            }
        }
        return shapes.front();
    }

    void LevelPlay::DrawAll() {
        Vector2 screenOffset = containerPosition;

        std::vector<ShapeDrawData> stuffToDraw;
        stuffToDraw.reserve(gameObjects.size());
        for (const GameObject& object : gameObjects) {
            ShapeDrawData shapeDrawData;
            Shape shape = GetShapeFromID(object.shapeID);
            shapeDrawData.vertices = shape.vertices;
            shapeDrawData.indices = shape.indices;
            shapeDrawData.thickness = shape.thickness;
            shapeDrawData.color = shape.color;
            for (Vector2& vertex : shapeDrawData.vertices) {
                // X/Y transposed(?) to rotate 90°
                float cosAngle = object.rotation.y;
                float sinAngle = -object.rotation.x;
                float rotatedX = vertex.x * cosAngle - vertex.y * sinAngle;
                float rotatedY = vertex.x * sinAngle + vertex.y * cosAngle;
                vertex.x = rotatedX;
                vertex.y = rotatedY;
                vertex *= object.scale;
                vertex += object.position + screenOffset;
            }
            stuffToDraw.emplace_back(shapeDrawData);
        }

        for (const ShapeDrawData& shapeDrawData : stuffToDraw) {
            for (int i = 0; i < shapeDrawData.indices.size() -1; i += 1) {
                int beginVertexIndex = shapeDrawData.indices[i];
                int endVertexIndex = shapeDrawData.indices[i + 1];
                DrawLineEx(shapeDrawData.vertices[beginVertexIndex], shapeDrawData.vertices[endVertexIndex], shapeDrawData.thickness, shapeDrawData.color);
            }
        }

        //Draw masking frame to hide enemies partially out of bounds
        float maskWH = 30.0f;
        Rectangle maskLeft {
            screenOffset.x - maskWH,
            screenOffset.y - maskWH,
            maskWH,
            containerSize.y + maskWH * 2};
        Rectangle maskRight {
            containerPosition.x + containerSize.x,
            containerPosition.y - maskWH,
            maskWH,
            containerSize.y + maskWH * 2};
        Rectangle maskTop {screenOffset.x, screenOffset.y - maskWH, containerSize.x, maskWH};
        Rectangle maskBottom {screenOffset.x, screenOffset.y + containerSize.y, containerSize.x, maskWH};
        DrawRectangleRec(maskLeft, BLACK);
        DrawRectangleRec(maskRight, BLACK);
        DrawRectangleRec(maskTop, BLACK);
        DrawRectangleRec(maskBottom, BLACK);
    }


    GameObjectData LevelPlay::GetGameObjectData(const uid ID) {
        for (const GameObject& object : gameObjects) {
            if (object.ID == ID) {
                return GameObjectData{object.position, object.rotation, object.velocity, object.scale};
            }
        }
        return {};
    }
} // LevelPlay
