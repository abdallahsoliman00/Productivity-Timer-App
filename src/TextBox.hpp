/* TextBox.hpp */
#pragma once

#include "raylib.h"
#include <string>

#ifndef TBXCOL
#define TBXCOL Color{30, 30, 30, 255}
#endif

#ifndef BUTTONCOL
#define BUTTONCOL Color{203, 166, 102, 255}
#endif

#ifndef TEXTCOL
#define TEXTCOL Color{180, 180, 180, 255}
#endif


class TextBox {
    Color _color = TBXCOL;
    char _text[256] = {};
    int _text_len = 0;
    bool _selected = false;
    bool _cleared = false;
    bool _auto_clear = false;
    int _max_length = 255;

public:
    Rectangle TBoxRect = {};

    TextBox() = default;

    explicit TextBox(const Color color) : _color(color) {}

    explicit TextBox(const char* text) {
        int idx = 0;
        while (text[idx] != '\0') {
            _text[idx] = text[idx];
            idx++;
        }
        _text[idx] = '\0';
    }

    explicit TextBox(const char* text, const bool autoClear, const int maxLength = 149)
        : _auto_clear(autoClear), _max_length(maxLength < 150 ? maxLength : 149) {
        int idx = 0;
        while (text[idx] != '\0') {
            _text[idx] = text[idx];
            idx++;
        }
        _text[idx] = '\0';
    }

    int lenText() const {
        return _text_len;
    }

    bool isSelected() const {
        return _selected;
    }

    void Select() {
        _selected = true;
    }

    void Deselect() {
        _selected = false;
        _cleared = false;
    }

    void AppendChar(const char c) {
        if (_auto_clear && !_cleared) {
            Clear();
        }
        if (_text_len < _max_length) {
            _text[_text_len++] = c;
            _text[_text_len] = '\0';
        }
    }

    void Clear() {
        if (!_cleared) {
            _text_len = 0;
            _text[0] = '\0';
            _cleared = true;
        }
    }

    void Backspace() {
        if (_text_len > 0) {
            _text[--_text_len] = '\0';
        }
    }

    int GetNumFromTxt() const {
        return std::stoi(_text);
    }

    template <typename Func>
    void Draw(const Vec2 &pos, const Vec2 &dims, const float fontSize, const Func &DrawTextFunc, const Color &textCol = TEXTCOL) {
        const auto StartPos = pos - dims/2;
        TBoxRect = {StartPos.x, StartPos.y, dims.x, dims.y};
        DrawRectangle(StartPos.x, StartPos.y, dims.x, dims.y, _color);
        if (_selected)
            DrawRectangleLines(StartPos.x, StartPos.y, dims.x, dims.y, BUTTONCOL);
        else
            DrawRectangleLines(StartPos.x, StartPos.y, dims.x, dims.y, TEXTCOL);
        DrawTextFunc(_text, pos, fontSize, textCol);
    }
};
