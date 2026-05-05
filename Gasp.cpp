
#include "Gasp.h"

// std::vector<GameObject> gameObjects;

Gasp::Gasp() {
    gameObjects.reserve(1000);
    iDToIndexLookup.reserve(1000);

    EnemyData enemy1Prefab = {{0, 0, 0, 0.8f, GREEN, 30}, 3.0f};
    EnemyData enemy2Prefab = {{0, 0, 0, 1.4f, BLUE, 15}, 2.0f};
    EnemyData enemy3Prefab = {{0, 0, 0, 2.0f, RED, 10}, 1.5f};
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

    // enemyPrefab.position = GetRandomEnemyStartPosition(enemyPrefab.size);
    // int enemyID = CreateGameObject(enemyPrefab);
}

Vector2 Gasp::GetRandomEnemyStartPosition(const float enemySize) {
    return {static_cast<float>(GetRandomValue(50, GetScreenWidth()-50)), -enemySize};
}

int Gasp::CreateGameObject(const GameObjectData& data) {
    int ID;
    if (!freeIDs.empty()) {
        ID = freeIDs.front();
        freeIDs.pop();
    } else {
        ID = static_cast<int>(gameObjects.size());
    }
    gameObjects.emplace_back(GameObject{data.position.x, data.position.y, data.velocity.x, data.velocity.y, data.color, data.size, ID});
    const int gameObjectIndex = static_cast<int>(gameObjects.size()-1);
    if (ID < iDToIndexLookup.size()) {
        iDToIndexLookup[ID] = gameObjectIndex;
    } else {
        iDToIndexLookup.push_back(gameObjectIndex);
    }
    return ID;
}

void Gasp::DestroyGameObject(const int ID) {
    // Make sure nothing is referring to this ID! Animation.?
    //Copy last object into index of the object to be destroyed
    gameObjects[iDToIndexLookup[ID]] = gameObjects.back();
    // Delete last object (that was just copied)
    gameObjects.pop_back();
    // Designate ID as unused with -1
    iDToIndexLookup[ID] = -1;
    //Add ID to queue with available IDs
    freeIDs.emplace(ID);
}

void Gasp::SpawnEnemy(EnemyData enemyData) {
    enemyData.gameObjectData.position = GetRandomEnemyStartPosition(enemyData.gameObjectData.size);
    CreateGameObject(enemyData.gameObjectData);
}

void Gasp::Tick() {
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

    for (GameObject& object : gameObjects) {
        object.position.x += object.velocity.x;
        object.position.y += object.velocity.y;

        if (object.position.y > GetScreenHeight() + object.size) {
            DestroyGameObject(object.ID);
        }
    }
    // update animations
    // for (Animator& animator : animators) {
    //
    // }
    // UpdatePositions();
}

void Gasp::UpdatePositions() {
    for (GameObject& object : gameObjects) {
        object.position.x += object.velocity.x;
        object.position.y += object.velocity.y;
    }
}

void Gasp::DrawObjects() const {
    for (const GameObject& object : gameObjects) {
        DrawCircleLinesV(object.position, object.size, object.color);
    }
}

void Gasp::SetPosition(const int ID, const Vector2& newPosition) {
    for (GameObject& object : gameObjects) {
        if (object.ID == ID) {
            object.position = newPosition;
            break;
        }
    }
}

void Gasp::SetVelocity(const int ID, const Vector2& newVelocity) {
    for (GameObject& object : gameObjects) {
        if (object.ID == ID) {
            object.velocity = newVelocity;
            break;
        }
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