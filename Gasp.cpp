
#include "Gasp.h"
#include "Utils.h"

Gasp::Gasp() {
    std::vector<int> containerIDs;
    uiSystem.AddChildrenContainersTo(uiSystem.GetRootContainer().ID, 2, containerIDs, UI::UISplitDirection::Vertical);
    uiSystem.GetContainerFromID(containerIDs[0]).drawBorder = false;

    levelPlay = {};
}

void Gasp::FrameUpdate() {
    levelPlay.FrameUpdate();
}

void Gasp::DrawObjects() {
    uiSystem.DrawAll();
    levelPlay.DrawAll();
}