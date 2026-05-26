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
        uid ID = Utils::UIDGenerator::GetNewUID();
        int parentID = -1;
        std::vector<int> childrenIDs;
        bool drawBorder = true;
    };


    class UISystem {
        inline static std::vector<UIContainer> uiContainers;
        inline static Color uiColor = {50, 150, 110, 255};

    public:
        UISystem();

        static uid AddChildContainerTo(uid parentID);

        static void AddChildrenContainersTo(uid parentID, int numChildren, std::vector<uid>& childrenIDs, UISplitDirection splitDirection);

        static UIContainer &GetRootContainer();

        static Vector2 GetAvailableSpaceIn(uid containerID);

        static void DrawNeonBorder(uid containerID);

        static void DrawCornerCutBorder(uid containerID);

        static void DrawAll();

        static UIContainer& GetContainerFromID(uid containerID);
    };
}


