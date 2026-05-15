
#include "UISystem.h"

namespace UI {
    UISystem::UISystem() {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        uiComponents = std::vector<UIArea>(1000);
        uiComponents.emplace_back();
        uiComponents.back().position = {5, 5};
        uiComponents.back().size = {static_cast<float>(screenWidth -10), static_cast<float>(screenHeight -10)};
    }

    void UISystem::AddChildComponentTo(const int parentID) {

    }


    void UISystem::Draw() {
        for (const UIArea& component : uiComponents) {
            float absPosX = component.position.x;
            float absPosY = component.position.y;
            int parentID = component.parentID;
            while (parentID != -1) {
                UIArea& parent = GetComponentFromID(parentID);
                absPosX += (parent.position.x + parent.padding);
                absPosY += (parent.position.y + parent.padding);
                parentID = parent.parentID;
            }
            DrawRectangleLines(static_cast<int>(absPosX), static_cast<int>(absPosY), static_cast<int>(component.size.x), static_cast<int>(component.size.y), component.color);
        }
    }

    UIArea& UISystem::GetComponentFromID(int id) {
        for (UIArea& component : uiComponents) {
            if (component.ID == id) {
                return component;
            }
        }
        return uiComponents.front();
    }

}