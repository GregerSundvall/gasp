#pragma once
#include "LevelPlay.h"
#include "UISystem.h"

class Gasp {
private:
    UI::UISystem uiSystem {};
    LevelPlay::LevelPlay levelPlay;

public:
    Gasp();
    void FrameUpdate();
    void DrawObjects();
};