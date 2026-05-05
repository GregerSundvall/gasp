
#include "Gasp.h"

std::vector<GameObject> gameObjects;

Gasp::Gasp() {
    gameObjects.reserve(1000);

    GameObjectData enemyPrefab = {0, 0, 0, 1, RED, 20};
    enemyPrefab.position = GetRandomEnemyStartPosition(enemyPrefab.size);
    CreateGameObject(enemyPrefab);
}

Vector2 Gasp::GetRandomEnemyStartPosition(const float enemySize) {
    float StartPosY = 20;
    return {static_cast<float>(GetRandomValue(50, GetScreenWidth()-50)), enemySize*0.5f};
}

int Gasp::CreateGameObject(const GameObjectData& data) {
    const int ID = static_cast<int>(gameObjects.size());
    gameObjects.emplace_back(GameObject{data.position.x, data.position.y, data.velocity.x, data.velocity.y, data.color, data.size, ID});
    return ID;
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