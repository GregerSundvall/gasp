
#include "Gasp.h"
#include "Utils.h"

Gasp::Gasp() {
    uiSystem = std::make_unique<UI::UISystem>();
    std::vector<int> containerIDs;
    UI::UISystem::AddChildrenContainersTo(UI::UISystem::GetRootContainer().ID, 2, containerIDs, UI::UISplitDirection::Vertical);
    int containerIDLeft = containerIDs[0];
    int containerIDRight = containerIDs[1];
    // uiSystem.GetContainerFromID(containerIDRight).drawBorder = false;

    levelPlay = std::make_unique<LevelPlay::LevelPlay>();
    UI::UIContainer& container = UI::UISystem::GetContainerFromID(containerIDRight);
    levelPlay->SetPositionAndSize(container.position, container.size);
}

void Gasp::FrameUpdate() {
    LevelPlay::LevelPlay::FrameUpdate();
}

void Gasp::DrawObjects() {
    UI::UISystem::DrawAll();
    LevelPlay::LevelPlay::DrawAll();
}