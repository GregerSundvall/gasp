#pragma once
#include <random>
#include "raylib.h"

using uid = int;

namespace Utils {

    inline std::random_device rd;
    inline std::mt19937 gen(rd());

    inline float GetRandomFloat(const float min, const float max) {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(gen);
    };


    class UIDGenerator {
    public:
        static uid GetNewUID() {
            static uid currentId = 0;
            return currentId++;
        }
        UIDGenerator() = delete;
    };

    inline float ClampedDeltaTime() { return std::min(0.1f, GetFrameTime()); }

    class Utils {
    };
} // Utils
