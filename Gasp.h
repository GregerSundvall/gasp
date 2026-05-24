#pragma once
#include "LevelPlay.h"
#include "UISystem.h"

class Gasp {
private:
    inline static std::unique_ptr<UI::UISystem> uiSystem;
    inline static std::unique_ptr<LevelPlay::LevelPlay> levelPlay;

public:
    Gasp();
    static void FrameUpdate();
    static void DrawObjects();
};