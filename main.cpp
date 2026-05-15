#include <iostream>
// #include "raylib.h"
#include "cmake-build-debug/_deps/raylib-src/src/raylib.h"
#include "Gasp.h"

int main() {
    const int windowWidth = 2000;
    const int windowHeight = 1000;
    const Color backgroundColor = {0, 0, 0, 0};
    InitWindow(windowWidth, windowHeight, "gasp");
    SetTargetFPS(60);

    Gasp gasp {};

    while (!WindowShouldClose()) {

        gasp.Tick();
        BeginDrawing();
        ClearBackground(backgroundColor);
        gasp.DrawObjects();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
