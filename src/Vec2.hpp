/* Vec.hpp */
#pragma once

#include "raylib.h"
#include <type_traits>

struct Vec2 {
    float x;
    float y;

    Vec2() : x(0.0), y(0.0) {}

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    Vec2(const T x_, const T y_) : x(static_cast<float>(x_)), y(static_cast<float>(y_)) {}

    Vec2(const Vector2 &vec) : x(vec.x), y(vec.y) {}

    Vec2(const Vec2 &vec) = default;

    // Operator to implicitly convert Vec2 to Vector2
    operator Vector2() const {
        return Vector2{x,y};
    }

    Vec2 operator+ (const Vec2 &other) const {
        return Vec2{x+other.x, y+other.y};
    }

    Vec2 operator- (const Vec2 &other) const {
        return Vec2{x-other.x, y-other.y};
    }

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    Vec2 operator* (const T &num) const {
        return Vec2{x*num, y*num};
    }

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    Vec2 operator/ (const T &num) const {
        if (num == 0) return {0,0};
        return Vec2{x/num, y/num};
    }

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    friend Vec2 operator* (const T &num, const Vec2 &vec) {
        return Vec2{vec.x * num, vec.y * num};
    }

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    friend Vec2 operator+ (const T &num, const Vec2 &vec) {
        return Vec2{vec.x + num, vec.y + num};
    }

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    friend Vec2 operator- (const T &num, const Vec2 &vec) {
        return Vec2{vec.x - num, vec.y - num};
    }

};
