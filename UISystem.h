#pragma once
#include <vector>

#include "raylib.h"
#include "Utils.h"

namespace UI {

    enum UIBoxAlignment {
        Centered,
        Left,
        Right,
        Up,
        Down,
    };

    struct UIBox {
        Vector2 position;
        Vector2 size;
        Color color = {255, 255, 255, 255};
        float borderThickness = 3;
        float padding = 10;
        UIBoxAlignment alignment = Centered;
        int ID = Utils::UIDGenerator::GetNewUID();
        int parentID = -1;
        std::vector<int> childrenIDs;
        std::vector<int> siblingIDs;
    };


    class UISystem {
        std::vector<UIBox> uiBoxes;
        Color uiColor = {50, 150, 110, 255};

    public:
        UISystem();

        int AddChildBoxTo(int parentID);

        UIBox &GetRootBox();

        Vector2 GetAvailableSpaceIn(int boxID);

        void DrawNeonFrame(int boxID);

        void DrawCornerCutFrame(int boxID);

        void DrawAll();

        UIBox& GetBoxFromID(int boxId);
    };
}


