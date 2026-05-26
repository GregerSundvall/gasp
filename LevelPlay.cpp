
#include "LevelPlay.h"
#include "Utils.h"

namespace LevelPlay {
    LevelPlay::LevelPlay() {
        gameObjects.reserve(1000);

        // Vector2 playerPosition = {};
        // GameObjectData playerData = {{0, 0, 0.1f, 0.1f, {255, 255, 255, 255}, 1.0f}};

        EnemyData enemyType1 = {{0, 0, 0.1f, 0.8f,
                                {80, 235, 121, 255}, 15.0f}, Linear};
        AddWave(enemyType1, 0.0f, 2.0f, 5);

        EnemyData enemyType2 = {{0, 0, 0.1f, 1.4f,
                                {98, 70, 212, 255}, 12.0f}, Linear};
        AddWave(enemyType2, 2.0f, 2.0f, 5);

        EnemyData enemyType3 = {{0, 0, 0.1f, 2.0f,
                                {222, 42, 135, 255}, 10.0f}, Linear};
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

        //Calculate velocities

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
            if (object.position.y > containerSize.y + object.size) {
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
        Vector2 startPos {0.0f, 0.0f};
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
        object.velocity = data.velocity;
        object.color = data.color;
        object.size = data.size;
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
        enemyData.gameObjectData.position = GetRandomEnemyStartPosition(enemyData.gameObjectData.size);
        CreateGameObject(enemyData.gameObjectData);
    }

    void LevelPlay::DrawAll() {
        Vector2 screenOffset = containerPosition;
        for (const GameObject& object : gameObjects) {
            DrawCircleLinesV(object.position + screenOffset, object.size, object.color);
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
                return GameObjectData{object.position, object.velocity, object.color, object.size};
            }
        }
        return {};
    }
} // LevelPlay
