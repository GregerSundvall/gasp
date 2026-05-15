#pragma once
#include <random>

#include "raylib.h"


// Operator overloads for raylib's Vector2
inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
    return Vector2{ lhs.x + rhs.x, lhs.y + rhs.y };
}
inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs) {
    return Vector2{ lhs.x - rhs.x, lhs.y - rhs.y };
}
inline Vector2& operator+=(Vector2& lhs, const Vector2& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}
inline Vector2& operator-=(Vector2& lhs, const Vector2& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}


namespace Utils {

    inline std::random_device rd;
    inline std::mt19937 gen(rd());

    inline float GetRandomFloat(const float min, const float max) {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(gen);
    };


    class UIDGenerator {
    public:
        static int GetNewUID() {
            static int currentId = 0;
            return currentId++;
        }
        UIDGenerator() = delete;
    };


    class Utils {
    };
} // Utils
