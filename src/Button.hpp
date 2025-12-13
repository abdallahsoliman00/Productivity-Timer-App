/* Button.hpp */
#pragma once

#include "Vec2.hpp"

class Button {
    Vec2 _dims = {140.0f, 56.0f};
    Color _color = {203, 166, 102, 255};

public:
    Rectangle _btn_rect = {};

    Button() = default;

    explicit Button(const Color color) : _color(color) {}

    explicit Button(const Vec2 &dims) : _dims(dims) {}

    explicit Button(const Vec2 &dims, const Color color) : _dims(dims), _color(color) {}

    void Draw(const Vec2 &pos) {
        const auto StartPos = pos - _dims/2;
        _btn_rect = {StartPos.x, StartPos.y, _dims.x, _dims.y};
        DrawRectangleRounded(_btn_rect, 0.3f, 10, _color);
    }

    Button& operator=(const Button &other) = default;
    Button& operator=(Button &&other) = default;
};
