
#include "Gasp.h"

// std::vector<GameObject> gameObjects;

Gasp::Gasp() {
    gameObjects.reserve(1000);

    EnemyData enemy1Prefab = {{0, 0, 0.1f, 0.8f,
                            {80, 235, 121, 255}, 30}, Linear, 3.0f};
    EnemyData enemy2Prefab = {{0, 0, 0.1f, 1.4f,
                            {98, 70, 212, 255}, 15}, Linear, 2.0f};
    EnemyData enemy3Prefab = {{0, 0, 0.1f, 2.0f,
                            {222, 42, 135, 255}, 10}, Linear, 1.5f};
    LevelEnemyData levelData;
    for (int i = 0; i < 5; ++i) {
        levelData.enemyData.emplace(enemy1Prefab);
    }
    for (int i = 0; i < 5; ++i) {
        levelData.enemyData.emplace(enemy2Prefab);
    }
    for (int i = 0; i < 5; ++i) {
        levelData.enemyData.emplace(enemy3Prefab);
    }
    currentLevel = levelData;
    currentLevel.timeSinceLastSpawn = 0;

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

void Gasp::Tick() {
    //Spawn enemies
    if (!currentLevel.enemyData.empty()) {
        const EnemyData& enemy = currentLevel.enemyData.front();
        if (currentLevel.timeSinceLastSpawn >= enemy.delay) {
            SpawnEnemy(enemy);
            currentLevel.enemyData.pop();
            currentLevel.timeSinceLastSpawn -= enemy.delay;
        } else {
            currentLevel.timeSinceLastSpawn += GetFrameTime();
        }
    }

    //Calculate velocities



    // Move everyone and check bounds
    std::deque<int> indicesToDestroy;
    float screenHeight = static_cast<float>(GetScreenHeight());
    double time = GetTime();
    for (int i = 0; i < gameObjects.size(); ++i) {
        GameObject& object = gameObjects[i];
        // object.position.x += object.velocity.x;
        object.position.x += object.velocity.x * speedModifierSine(time - object.timeCreated);
        // object.position.y += object.velocity.y;
        object.position.y += object.velocity.y * speedModifierChoppy(time - object.timeCreated);

        if (object.position.y > screenHeight + object.size) {
            indicesToDestroy.emplace_back(i);
        }
    }

    // Destroy out of bounds objects
    while (!indicesToDestroy.empty()) {
        if (indicesToDestroy.back() != gameObjects.size()-1) {
            gameObjects[indicesToDestroy.back()] = gameObjects.back();
        }
        gameObjects.pop_back();

        // DestroyGameObject(gameObjects[indicesToDestroy.back()].ID);
        indicesToDestroy.pop_back();
    }
}

Vector2 Gasp::GetRandomEnemyStartPosition(const float enemySize) {
    return {static_cast<float>(GetRandomValue(50, GetScreenWidth()-50)), -enemySize};
}

int Gasp::CreateGameObject(const GameObjectData& data) {
    const int ID = UIDGuy::HitMe();
    gameObjects.emplace_back(GameObject{data.position.x, data.position.y, data.velocity.x, data.velocity.y,
                            data.color, data.size, GetTime(), ID});
    return ID;
}

void Gasp::DestroyGameObject(const int ID) {
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

void Gasp::SpawnEnemy(EnemyData enemyData) {
    enemyData.gameObjectData.position = GetRandomEnemyStartPosition(enemyData.gameObjectData.size);
    CreateGameObject(enemyData.gameObjectData);
}

void Gasp::DrawObjects() const {
    for (const GameObject& object : gameObjects) {
        DrawCircleLinesV(object.position, object.size, object.color);
    }
}


GameObjectData Gasp::GetGameObjectData(const int ID) const {
    for (const GameObject& object : gameObjects) {
        if (object.ID == ID) {
            return GameObjectData{object.position, object.velocity, object.color, object.size};
        }
    }
    return {};
}