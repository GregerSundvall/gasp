
#include "Gasp.h"
#include "Utils.h"
#include "raymath.h"

Gasp::Gasp() {
    uiSystem = std::make_unique<UI::UISystem>();
    std::vector<uid> containerIDs;
    UI::UISystem::CreateChildrenContainersIn(UI::UISystem::GetRootContainer().ID, 2, containerIDs, UI::UISplitDirection::Vertical);
    int containerIDLeft = containerIDs[0];
    int containerIDRight = containerIDs[1];
    // uiSystem.GetContainerFromID(containerIDRight).drawBorder = false;

    StartLevelPlayIn(containerIDRight);



}

void Gasp::FrameUpdate() {
    HandleInput();
    if (levelPlay->isAlive) {
        levelPlay->HandleInput();
        levelPlay->FrameUpdate();
    }
}

void Gasp::Draw() {
    LevelPlay::LevelPlay::DrawAll();
    UI::UISystem::Draw();
}

void Gasp::HandleInput() {
}

void Gasp::StartLevelPlayIn(uid containerID) {
    levelPlay = std::make_unique<LevelPlay::LevelPlay>();
    UI::UIContainer& container = UI::UISystem::GetContainerFromID(containerID);
    Vector2 padding = {10.0f, 10.0f};
    levelPlay->Init(container.position + padding, container.size - padding * 2);
}
