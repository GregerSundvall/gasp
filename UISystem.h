#pragma once
#include <vector>

#include "raylib.h"
#include "Utils.h"

namespace UI {


    struct UIArea {
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
        std::vector<UIArea> uiComponents;

    public:
        UISystem();

        void AddChildComponentTo(int parentID);
        void Draw();

        UIArea& GetComponentFromID(int id);
    };
}


