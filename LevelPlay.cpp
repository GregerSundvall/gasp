
#include "LevelPlay.h"
#include "Utils.h"

namespace LevelPlay {
    LevelPlay::LevelPlay() {
        gameObjects.reserve(1000);

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



        // Move everyone and check bounds
        std::deque<int> indicesToDestroy;
        float screenHeight = static_cast<float>(GetScreenHeight());
        double time = GetTime();
        for (int i = 0; i < gameObjects.size(); ++i) {
            GameObject& object = gameObjects[i];
            // object.position.x += object.velocity.x;
            object.position.x += object.velocity.x * speedModifierSine(time + object.animDesync);
            // object.position.y += object.velocity.y;
            object.position.y += object.velocity.y * speedModifierChoppy(time + object.animDesync);

            // if (object.position.y > screenHeight + object.size) {
            //     indicesToDestroy.emplace_back(i);
            // }
        }

        // Destroy out of bounds objects
        // while (!indicesToDestroy.empty()) {
        //     if (indicesToDestroy.back() != gameObjects.size()-1) {
        //         gameObjects[indicesToDestroy.back()] = gameObjects.back();
        //     }
        //     gameObjects.pop_back();
        //
        //     // DestroyGameObject(gameObjects[indicesToDestroy.back()].ID);
        //     indicesToDestroy.pop_back();
        // }
    }

    Vector2 LevelPlay::GetRandomEnemyStartPosition(const float enemySize) {
        Vector2 startPos {0.0f, 0.0f};
        float minXPos = containerPosition.x + enemySize;
        float maxXPos = containerPosition.x + containerSize.x - enemySize;
        startPos.y = containerPosition.y - enemySize;
        startPos.x = Utils::GetRandomFloat(minXPos, maxXPos);
        return startPos;
    }

    // Creates gameObject and returns gameObject ID.
    int LevelPlay::CreateGameObject(const GameObjectData& data) {
        gameObjects.emplace_back();
        GameObject& object = gameObjects.back();
        object.position = data.position;
        object.velocity = data.velocity;
        object.color = data.color;
        object.size = data.size;
        return object.ID;
    }

    void LevelPlay::DestroyGameObject(const int ID) {
        int indexToRemove;
        for (int i = 0 ; i <  gameObjects.size(); ++i) {
            if (gameObjects[i].ID == ID) {
                indexToRemove = i;
                break;
            }
        }
        if (indexToRemove != gameObjects.size() -1) {
            gameObjects[indexToRemove] = gameObjects.back();
        }
        gameObjects.pop_back();
    }

    void LevelPlay::SpawnEnemy(EnemyData enemyData) {
        enemyData.gameObjectData.position = GetRandomEnemyStartPosition(enemyData.gameObjectData.size);
        CreateGameObject(enemyData.gameObjectData);
    }

    void LevelPlay::DrawAll() {
        for (const GameObject& object : gameObjects) {
            DrawCircleLinesV(object.position, object.size, object.color);
        }

        //Draw masking frame to hide enemies partially out of bounds
        float maskWH = 30.0f;
        Rectangle maskLeft {
            containerPosition.x - maskWH,
            containerPosition.y - maskWH,
            maskWH,
            containerSize.y + maskWH * 2};
        Rectangle maskRight {
            containerPosition.x + containerSize.x,
            containerPosition.y - maskWH,
            maskWH,
            containerSize.y + maskWH * 2};
        Rectangle maskTop {containerPosition.x, containerPosition.y - maskWH, containerSize.x, maskWH};
        Rectangle maskBottom {containerPosition.x, containerPosition.y + containerSize.y, containerSize.x, maskWH};
        DrawRectangleRec(maskLeft, BLACK);
        DrawRectangleRec(maskRight, BLACK);
        DrawRectangleRec(maskTop, BLACK);
        DrawRectangleRec(maskBottom, BLACK);
    }


    GameObjectData LevelPlay::GetGameObjectData(const int ID) {
        for (const GameObject& object : gameObjects) {
            if (object.ID == ID) {
                return GameObjectData{object.position, object.velocity, object.color, object.size};
            }
        }
        return {};
    }
} // LevelPlay
