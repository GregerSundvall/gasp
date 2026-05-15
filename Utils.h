#pragma once
#include <random>

namespace Utils {

    inline std::random_device rd;
    inline std::mt19937 gen(rd());

    inline float GetRandomFloat(const float min, const float max) {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(gen);
    };


    struct Vec2 {
        float x = 0.0f;
        float y = 0.0f;

        Vec2() = default;
        Vec2(float x, float y) : x(x), y(y) {}

        Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
        Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
        Vec2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
        Vec2 operator*(const Vec2& other) const { return {x * other.x, y * other.y}; }
        Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
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
