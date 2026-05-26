
#include "UISystem.h"
#include "Utils.h"
#include "raymath.h"

namespace UI {
    UISystem::UISystem() {
        // Create root UI container
        float screenWidth = static_cast<float>(GetScreenWidth());
        float screenHeight = static_cast<float>(GetScreenHeight());
        float windowMargin = 15.0f;
        uiContainers.reserve(1000);
        uiContainers.emplace_back();
        uiContainers.back().position = {windowMargin, windowMargin};
        uiContainers.back().size = {screenWidth - windowMargin * 2, screenHeight - windowMargin * 2};
    }

    int UISystem::AddChildContainerTo(const uid parentID) {
        uiContainers.emplace_back();
        UIContainer& child = uiContainers.back();
        child.parentID = parentID;
        UIContainer& parent = GetContainerFromID(parentID);
        parent.childrenIDs.push_back(child.ID);

        child.position.x = parent.position.x + parent.padding;
        child.position.y = parent.position.y + parent.padding;
        Vector2 size = GetAvailableSpaceIn(parentID);
        child.size.x = size.x;
        child.size.y = size.y;
        return child.ID;
    }

    void UISystem::AddChildrenContainersTo(const uid parentID, int numChildren, std::vector<uid>& childrenIDs, UISplitDirection splitDirection) {
        childrenIDs.clear();
        childrenIDs.reserve(numChildren);
        UIContainer& parent = GetContainerFromID(parentID);
        parent.childrenIDs.clear();

        for (int i = 0; i < numChildren; ++i) {
            childrenIDs.push_back(AddChildContainerTo(parentID));
            parent.childrenIDs.push_back(childrenIDs.back());
        }

        for (const uid& childID : childrenIDs) {
            GetContainerFromID(childID).parentID = parentID;
        }

        Vector2 availableSize = GetAvailableSpaceIn(parentID);
        Vector2 startPosition = parent.position + Vector2(parent.padding, parent.padding);
        float padding = parent.padding; // Make this a function argument instead?

        if (splitDirection == Vertical) {
            float paddingSum = padding * (numChildren - 1);
            float childWidth = (availableSize.x - paddingSum) / numChildren;
            float spacing = childWidth + padding;
            for (int i = 0; i < childrenIDs.size(); ++i) {
                UIContainer& child = GetContainerFromID(childrenIDs[i]);
                child.position.x = startPosition.x + spacing * i;
                child.position.y = startPosition.y;
                child.size.x = childWidth;
                child.size.y = availableSize.y;
            }
        } else { // Implicitly horizontal splitDirection
            float paddingSum = padding * (numChildren - 1);
            float childHeight = (availableSize.y - paddingSum) / numChildren;
            float spacing = childHeight + padding;
            for (int i = 0; i < childrenIDs.size(); ++i) {
                UIContainer& child = GetContainerFromID(childrenIDs[i]);
                child.position.x = startPosition.x;
                child.position.y = startPosition.y + spacing * i;
                child.size.x = availableSize.x;
                child.size.y = childHeight;
            }
        }
    }

    UIContainer& UISystem::GetRootContainer() {
        return uiContainers.front();
    }

    Vector2 UISystem::GetAvailableSpaceIn(const uid containerID) {
        const UIContainer& box = GetContainerFromID(containerID);
        Vector2 size = {box.size.x - box.padding * 2, box.size.y - box.padding * 2};
        return size;
    }

    void UISystem::DrawNeonBorder(uid containerID) {
        UIContainer& container = GetContainerFromID(containerID);
        int posX = static_cast<int>(container.position.x);
        int posY = static_cast<int>(container.position.y);
        int width = static_cast<int>(container.size.x);
        int height = static_cast<int>(container.size.y);
        Color mainColor = uiColor;
        Color dimmedColor = Color(mainColor.r, mainColor.g, mainColor.b, mainColor.a*0.5f);
        Color dimmedColor2 = Color(mainColor.r, mainColor.g, mainColor.b, mainColor.a*0.4f);
        DrawRectangleLines(posX, posY, width, height, dimmedColor2);
        DrawRectangleLines(posX+1, posY+1, width-2, height-2, dimmedColor);
        DrawRectangleLines(posX+2, posY+2, width-4, height-4, mainColor);
        DrawRectangleLines(posX+3, posY+3, width-6, height-6, dimmedColor);
        DrawRectangleLines(posX+4, posY+4, width-8, height-8, dimmedColor2);
    }

    void UISystem::DrawCornerCutBorder(uid containerID) {
        UIContainer& container = GetContainerFromID(containerID);
        Vector2 cornerCutWidth {40.0f, 0.0f};
        Vector2 cornerCutHeight {0.0f, 40.0f};
        float thickness = 3.0f;
        Vector2 upLeft = {container.position.x, container.position.y};
        Vector2 upRight = {container.position.x + container.size.x, container.position.y};
        Vector2 downLeft = {container.position.x, container.position.y + container.size.y};
        Vector2 downRight = {container.position.x + container.size.x, container.position.y + container.size.y};

        DrawLineEx(upLeft, upRight-cornerCutWidth, thickness, uiColor);
        DrawLineEx(upRight - cornerCutWidth, upRight + cornerCutHeight, thickness, uiColor);
        DrawLineEx(upRight + cornerCutHeight, downRight, thickness, uiColor);
        DrawLineEx(upLeft, downLeft - cornerCutHeight, thickness, uiColor);
        DrawLineEx(downLeft - cornerCutHeight, downLeft + cornerCutWidth, thickness, uiColor);
        DrawLineEx(downLeft + cornerCutWidth, downRight, thickness, uiColor);
    }

    void UISystem::DrawAll() {
        for (const UIContainer& container : uiContainers) {
            if  (container.drawBorder) {
                DrawCornerCutBorder(container.ID);
            }
        }
    }

    UIContainer& UISystem::GetContainerFromID(uid containerID) {
        for (UIContainer& container : uiContainers) {
            if (container.ID == containerID) {
                return container;
            }
        }
        return uiContainers.front();
    }

}
