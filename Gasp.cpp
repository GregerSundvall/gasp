
#include "Gasp.h"
#include "Utils.h"

Gasp::Gasp() {
    uiSystem = std::make_unique<UI::UISystem>();
    std::vector<uid> containerIDs;
    UI::UISystem::AddChildrenContainersTo(UI::UISystem::GetRootContainer().ID, 2, containerIDs, UI::UISplitDirection::Vertical);
    int containerIDLeft = containerIDs[0];
    int containerIDRight = containerIDs[1];
    // uiSystem.GetContainerFromID(containerIDRight).drawBorder = false;

    levelPlay = std::make_unique<LevelPlay::LevelPlay>();
    UI::UIContainer& container = UI::UISystem::GetContainerFromID(containerIDRight);
    Vector2 padding = {10.0f, 10.0f};
    levelPlay->SetPositionAndSize(container.position + padding, container.size - padding * 2);
}

void Gasp::FrameUpdate() {
    LevelPlay::LevelPlay::FrameUpdate();
}

void Gasp::DrawObjects() {
    LevelPlay::LevelPlay::DrawAll();
    UI::UISystem::DrawAll();
}