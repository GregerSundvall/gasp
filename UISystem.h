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
        uid parentID = -1;
        std::vector<uid> childrenIDs;
        bool drawBorder = true;
        bool isOverlay = false;
    };


    class UISystem {
        inline static std::vector<UIContainer> uiContainers;
        inline static Color uiColor = {50, 150, 110, 255};

    public:
        UISystem();

        static uid CreateChildContainerIn(uid parentID, bool overlay = false);
        static void CreateChildrenContainersIn(uid parentID, int numChildren, std::vector<uid>& childrenIDs, UISplitDirection splitDirection);
        // static void CreateOverlayContainerIn(uid parentID);
        static UIContainer &GetRootContainer();
        static Vector2 GetAvailableSpaceIn(uid containerID);
        static void DrawNeonBorder(uid containerID);
        static void DrawCornerCutBorder(uid containerID);
        static void Draw();
        static UIContainer& GetContainerFromID(uid containerID);
    };
}


