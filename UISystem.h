#pragma once
#include <vector>

#include "raylib.h"
#include "Utils.h"

namespace UI {

    enum UISplitDirection {
        Horizontal,
        Vertical
    };

    struct UIContainer {
        Vector2 position;
        Vector2 size;
        Color color = {255, 255, 255, 255};
        float borderThickness = 3;
        float padding = 10;
        int ID = Utils::UIDGenerator::GetNewUID();
        int parentID = -1;
        std::vector<int> childrenIDs;
    };


    class UISystem {
        std::vector<UIContainer> uiContainers;
        Color uiColor = {50, 150, 110, 255};

    public:
        UISystem();

        int AddChildTo(int parentID);

        void AddChildrenTo(int parentID, int numChildren, std::vector<int>& childrenIDs, UISplitDirection splitDirection);

        UIContainer &GetRootContainer();

        Vector2 GetAvailableSpaceIn(int containerID);

        void DrawNeonFrame(int containerID);

        void DrawCornerCutFrame(int containerID);

        void DrawAll();

        UIContainer& GetContainerFromID(int containerID);
    };
}


