/* Button.hpp */
#pragma once

#include "TextBox.hpp"
#include "Vec2.hpp"

#ifndef BUTTONCOL
#define BUTTONCOL Color{203, 166, 102, 255}
#endif


class Button {
    Color _color = BUTTONCOL;
    const char *_text = nullptr;

public:
    Rectangle _btn_rect = {};

    Button() = default;

    explicit Button(const Color color) : _color(color) {}

    explicit Button(const char* txt, const Color color) : _color(color), _text(txt) {}

    template <typename Func>
    void Draw(const Vec2 &pos, const Vec2 &dims, const float fontSize, Func DrawTextFunc, const Color textCol = TEXTCOL) {
        const auto StartPos = pos - dims/2;
        _btn_rect = {StartPos.x, StartPos.y, dims.x, dims.y};
        DrawRectangleRounded(_btn_rect, 0.3f, 10, _color);
        DrawTextFunc(_text, pos, fontSize, textCol);
    }

    Button& operator=(const Button &other) = default;
    Button& operator=(Button &&other) = default;
};
