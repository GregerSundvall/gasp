
#include "UISystem.h"
#include "Utils.h"


namespace UI {
    UISystem::UISystem() {
        // Create a main UIBox to exist inside
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        uiBoxes.reserve(1000);
        uiBoxes.emplace_back();
        float margin = 20.0f;
        uiBoxes.back().position = {margin, margin};
        uiBoxes.back().size = {screenWidth - margin * 2, screenHeight - margin * 2};
    }

    int UISystem::AddChildBoxTo(const int parentID) {
        uiBoxes.emplace_back();
        int newBoxID = uiBoxes.back().ID;
        uiBoxes.back().parentID = parentID;
        UIBox& parent = GetBoxFromID(parentID);
        parent.childrenIDs.push_back(newBoxID);

        UIBox& child = GetBoxFromID(newBoxID);
        child.position.x = parent.position.x + parent.padding;
        child.position.y = parent.position.y + parent.padding;
        Utils::Vec2 size = GetAvailableSpaceIn(parentID);
        child.size.x = size.x;
        child.size.y = size.y;
        return newBoxID;
    }

    UIBox& UISystem::GetRootBox() {
        return uiBoxes.front();
    }

    Utils::Vec2 UISystem::GetAvailableSpaceIn(const int boxID) {
        const UIBox& box = GetBoxFromID(boxID);
        Utils::Vec2 size = {box.size.x - box.padding * 2, box.size.y - box.padding * 2};
        return size;
    }

    void UISystem::DrawNeonFrame(int boxID) {
        UIBox& box = GetBoxFromID(boxID);
        int posX = static_cast<int>(box.position.x);
        int posY = static_cast<int>(box.position.y);
        int width = static_cast<int>(box.size.x);
        int height = static_cast<int>(box.size.y);
        Color mainColor = uiColor;
        Color dimmedColor = Color(mainColor.r, mainColor.g, mainColor.b, mainColor.a*0.5f);
        Color dimmedColor2 = Color(mainColor.r, mainColor.g, mainColor.b, mainColor.a*0.4f);
        DrawRectangleLines(posX, posY, width, height, dimmedColor2);
        DrawRectangleLines(posX+1, posY+1, width-2, height-2, dimmedColor);
        DrawRectangleLines(posX+2, posY+2, width-4, height-4, mainColor);
        DrawRectangleLines(posX+3, posY+3, width-6, height-6, dimmedColor);
        DrawRectangleLines(posX+4, posY+4, width-8, height-8, dimmedColor2);
    }

    void UISystem::DrawCornerCutFrame(int boxID) {
        UIBox& box = GetBoxFromID(boxID);
        int cornerSize {40};
        Vector2 cornerCutWidth {40.0f, 0.0f};
        Vector2 cornerCutHeight {0.0f, 40.0f};
        float thickness = 3.0f;
        Vector2 upLeft = {box.position.x, box.position.y};
        Vector2 upRight = {box.position.x + box.size.x, box.position.y};
        Vector2 downLeft = {box.position.x, box.position.y + box.size.y};
        Vector2 downRight = {box.position.x + box.size.x, box.position.y + box.size.y};

        DrawLineEx(upLeft, upRight-cornerCutWidth, thickness, uiColor);
        DrawLineEx(upRight-cornerCutWidth, upRight+cornerCutHeight, thickness, uiColor);
        DrawLineEx(upRight+cornerCutHeight, downRight, thickness, uiColor);
        DrawLineEx(upLeft, downLeft-cornerCutHeight, thickness, uiColor);
        DrawLineEx(downLeft-cornerCutHeight, downLeft+cornerCutWidth, thickness, uiColor);
        DrawLineEx(downLeft+cornerCutWidth, downRight, thickness, uiColor);
    }

    void UISystem::DrawAll() {
        for (const UIBox& box : uiBoxes) {
            // float absPosX = box.position.x;
            // float absPosY = box.position.y;
            // int parentID = box.parentID;
            // while (parentID != -1) {
            //     UIBox& parent = GetBoxFromID(parentID);
            //     absPosX += (parent.position.x);
            //     absPosY += (parent.position.y);
            //     parentID = parent.parentID;
            // }
            // DrawRectangleLines(static_cast<int>(box.position.x), static_cast<int>(box.position.y), static_cast<int>(box.size.x), static_cast<int>(box.size.y), box.color);
            // DrawNeonFrame(box.ID);
            DrawCornerCutFrame(box.ID);
        }
    }

    UIBox& UISystem::GetBoxFromID(int boxId) {
        for (UIBox& box : uiBoxes) {
            if (box.ID == boxId) {
                return box;
            }
        }
        return uiBoxes.front();
    }

}
