#pragma once
#include <vector>

#include "raylib.h"
#include "Utils.h"

namespace UI {


    struct UIComponent {
        Utils::Vec2 position;
        Utils::Vec2 size;
        Color color = {255, 255, 255, 255};
        float borderThickness = 3;
        float padding = 5;
        int ID = Utils::UIDGenerator::GetNewUID();
        int parentID = -1;
        std::vector<int> childrenIDs;
        std::vector<int> siblingIDs;
    };


    class UISystem {
        // static std::vector<UIComponent> uiComponents;

    public:
        UISystem();

        static void AddChildComponentTo(const int parentID);
        static void Draw();
    };
}


