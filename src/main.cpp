#include <iomanip>
#include <string>
#include <vector>
#include "raylib.h"

#include "Vec2.hpp"
#include "Timer.hpp"
#include "Button.hpp"
#include "TextBox.hpp"
#include "StringManip.h"


#ifndef BGCOL
#define BGCOL Color{22, 20, 17, 255}
#endif

#ifndef TEXTCOL
#define TEXTCOL Color{180, 180, 180, 255}
#endif

#ifndef EXTRASCOL
#define EXTRASCOL Color{41, 47, 51, 255}
#endif

#ifndef TIMERCOL
#define TIMERCOL Color{80, 190, 190, 255}
#endif

#ifndef TIMERINNERRINGCOL
#define TIMERINNERRINGCOL Color{0, 40, 40, 255}
#endif


struct TodoItem {
    char _text[256] = {};
    bool _completed = false;
};


int main () {
    constexpr int InitScreenWidth = 1400, InitScreenHeight = 900;

    // Initialise window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(InitScreenWidth, InitScreenHeight, "Productivity Timer");
    SetExitKey(KEY_NULL);
    SetWindowMinSize(1400,900);

    // Add custom font and smoothing for scaling
    const Font FontLarge = LoadFontEx("../Resources/segoeui.ttf", 128, nullptr, 0);
    const Font FontMedium = LoadFontEx("../Resources/segoeui.ttf", 96, nullptr, 0);
    const Font FontSmall = LoadFontEx("../Resources/segoeui.ttf", 54, nullptr, 0);
    const Font FontVSmall = LoadFontEx("../Resources/segoeui.ttf", 28, nullptr, 0);
    SetTextureFilter(FontLarge.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(FontMedium.texture, TEXTURE_FILTER_BILINEAR);

    // Create Timer instance
    auto Tmr = Timer();
    Tmr.SetTimer(10);
    TextBox TimerTBx("10", true, 4);

    // Create todo items
    constexpr int todo_count = 10;
    TodoItem Todos[todo_count] = {};
    TextBox TodoTBx("", true, 149);
    bool toggledTodoTBx = false;

    // Add buttons
    Button ResetBtn("Reset", {35,35,35,255});
    Button StartBtn("Start", {0, 170, 70, 255});
    Button StopBtn("Stop", {145, 25, 25, 255});

    Button AddTaskBtn("+ Add Task", BUTTONCOL);


// Create function to add text to scene
    auto AddText = [&FontVSmall, &FontSmall, &FontMedium, &FontLarge](
        const char* text, const Vec2 &pos, const float fontSize, const Color color = TEXTCOL, const float maxWidth = 0.0f
    ) {
        const Font *font_ptr;
        if (fontSize < 28)
            font_ptr = &FontVSmall;
        else if (fontSize < 40)
            font_ptr = &FontSmall;
        else if (fontSize < 70)
            font_ptr = &FontMedium;
        else
            font_ptr = &FontLarge;

        const Vector2 textSize = MeasureTextEx(*font_ptr, text, fontSize, 0.0f);
        if (maxWidth == 0.0f || textSize.x <= maxWidth) {
            DrawTextEx(*font_ptr, text, (pos - Vec2(textSize)/2), fontSize, 0.0f, color);
        } else {

        }
    };



    while (!WindowShouldClose()) {
        // Gather constants
        const Vec2 ScreenDims = {GetScreenWidth(), GetScreenHeight()};
        const Vec2 MousePos = GetMousePosition();
        const float scale = fminf(ScreenDims.x / 1200.0f, ScreenDims.y / 700.0f);
        const float PanelWidth = ScreenDims.x / 2;

        // Screen setup
        BeginDrawing();
        ClearBackground(BGCOL);

        DrawLineEx({ScreenDims.x/2,20.0f*scale}, {ScreenDims.x/2,ScreenDims.y - 20.0f*scale}, 2, EXTRASCOL);

        AddText("TODO LIST", {PanelWidth/2.0f, 60.0f * scale}, 60);
        AddText("FOCUS TIMER", {PanelWidth*1.5f, 60.0f * scale}, 60);



        /* === TIMER === */
        /* Buttons and UI */
        // Add timer
        Vec2 TimerCentre = Vec2{PanelWidth*1.5f, ScreenDims.y/2.0f} + Vec2{0,-30};

        // Timer ring
        DrawRing(TimerCentre, 115*scale, 140*scale, 0.0f, 360.0f, 0, TIMERINNERRINGCOL);
        DrawRing(TimerCentre, 120*scale, 140*scale, -90.0f, 270.0f - (360.0f*Tmr.Progress()), 100, TIMERCOL);

        // Add timer text
        AddText(Tmr.GetCountdownTime().c_str(), TimerCentre, 40 * scale);

        // Add timer Start Stop
        auto strtBtnPos = TimerCentre + Vec2{0.0f,220.0f * scale};
        if (!Tmr.isRunning) {
            StartBtn.Draw(strtBtnPos, {140.0f*scale, 56.0f*scale}, 30.0f*scale, AddText);
        } else {
            StopBtn.Draw(strtBtnPos, {140.0f*scale, 56.0f*scale}, 30.0f*scale, AddText);
        }

        // Add Reset button
        auto rstBtnPos = TimerCentre + Vec2{0.0f,300.0f * scale};
        ResetBtn.Draw(rstBtnPos, {140.0f*scale, 56.0f*scale}, 30.0f*scale, AddText);

        // Add timer textbox
        TimerTBx.Draw(TimerCentre + Vec2{0.0f, -190.0f*scale}, Vec2{130.0f, 40.0f}*scale, 30.0f*scale, AddText);
        AddText("Set time (minutes):", TimerCentre + Vec2{0.0f, -225.0f*scale}, 20*scale, {200,200,200,180});


        /* Controls */
        if ( (CheckCollisionPointRec(MousePos, StartBtn._btn_rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                || IsKeyPressed(KEY_SPACE) && !TimerTBx.isSelected() )
        {
            if (!Tmr.isRunning) {
                Tmr.Resume();
            } else {
                Tmr.Pause();
            }
        }

        if ( (CheckCollisionPointRec(MousePos, ResetBtn._btn_rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R)) )
        {
            Tmr.Reset();
        }

        if (Tmr.Complete()) {
            Tmr.Pause();
        }

        if ( CheckCollisionPointRec(MousePos, TimerTBx.TBoxRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
            TimerTBx.Select();
        }

        if ( IsKeyPressed(KEY_ESCAPE) ) {
            TimerTBx.Deselect();
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            bool clickedOnUI = CheckCollisionPointRec(MousePos, StartBtn._btn_rect) ||
                               CheckCollisionPointRec(MousePos, ResetBtn._btn_rect) ||
                               CheckCollisionPointRec(MousePos, TimerTBx.TBoxRect);

            if (!clickedOnUI) {
                TimerTBx.Deselect();
            }
        }

        if (TimerTBx.isSelected()) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= '0' && key <= '9') {
                    TimerTBx.AppendChar(key);
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                TimerTBx.Backspace();
            }
            if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) && TimerTBx.lenText() != 0)) {
                if (!Tmr.isRunning) {
                    Tmr.Reset();
                    int num = TimerTBx.GetNumFromTxt();
                    Tmr.SetTimer(num);
                    TimerTBx.Deselect();
                }
            }
        }

        /* === TODO LIST === */
        const Vec2 todoBtnPos = {125.0f*scale, 160.0f};
        constexpr float taskBtnWidth = 120.0f;
        const float todoTbxWidth = (PanelWidth - 2.0f*todoBtnPos.x)/scale + taskBtnWidth;

        AddTaskBtn.Draw(todoBtnPos, {taskBtnWidth*scale, 40.0f*scale}, 24.0f*scale, AddText, BGCOL);

        if ( (CheckCollisionPointRec(MousePos, AddTaskBtn._btn_rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_T)) )
        {
            toggledTodoTBx = !toggledTodoTBx;
        }

        if (toggledTodoTBx) {
            TodoTBx.Draw(todoBtnPos + Vec2{(todoTbxWidth-taskBtnWidth)*0.5f*scale, 55.0f*scale}, Vec2{todoTbxWidth, 40.0f}*scale, 30.0f*scale, AddText);
            TodoTBx.Select();
        }

        AddText("HELLO HI HELLO WORLD THIS IS TEXT", {400.0f, 400.0f}, 40.0f, TEXTCOL, 300.0f);

        EndDrawing();
    }

    UnloadFont(FontLarge);
    UnloadFont(FontMedium);
    CloseWindow();
}


//
// // Create function to add text to scene
//     auto AddText = [&FontVSmall, &FontSmall, &FontMedium, &FontLarge](
//         const char* text, const Vec2 &pos, const float fontSize, const Color color = TEXTCOL, const float maxWidth = 0.0f
//     ) {
//         const Font *font_ptr;
//         if (fontSize < 28)
//             font_ptr = &FontVSmall;
//         else if (fontSize < 40)
//             font_ptr = &FontSmall;
//         else if (fontSize < 70)
//             font_ptr = &FontMedium;
//         else
//             font_ptr = &FontLarge;
//
//         const Vector2 textSize = MeasureTextEx(*font_ptr, text, fontSize, 0.0f);
//         if (maxWidth == 0.0f || textSize.x <= maxWidth) {
//             DrawTextEx(*font_ptr, text, (pos - Vec2(textSize)/2), fontSize, 0.0f, color);
//         } else {
//             // Word wrapping logic
//             std::string remainingText = text;
//             std::vector<std::string> lines;
//
//             // First, split text into lines
//             while (!remainingText.empty()) {
//                 std::string currentLine;
//                 int lastSpaceIdx = -1;
//                 int currentIdx = 0;
//
//                 // Find the longest substring that fits within maxWidth
//                 while (currentIdx <= static_cast<int>(remainingText.length())) {
//                     std::string testLine = remainingText.substr(0, currentIdx);
//                     Vector2 testSize = MeasureTextEx(*font_ptr, testLine.c_str(), fontSize, 0.0f);
//
//                     if (testSize.x > maxWidth) {
//                         // Exceeded maxWidth - break at last space if available
//                         if (lastSpaceIdx > 0) {
//                             currentLine = remainingText.substr(0, lastSpaceIdx);
//                             remainingText = remainingText.substr(lastSpaceIdx + 1); // Skip the space
//                         } else {
//                             // No space found, force break at current position
//                             currentLine = remainingText.substr(0, currentIdx - 1);
//                             remainingText = remainingText.substr(currentIdx - 1);
//                         }
//                         break;
//                     }
//
//                     // Track last space position
//                     if (currentIdx < static_cast<int>(remainingText.length()) && remainingText[currentIdx] == ' ') {
//                         lastSpaceIdx = currentIdx;
//                     }
//
//                     currentIdx++;
//
//                     // If we've processed all text, this is the last line
//                     if (currentIdx > static_cast<int>(remainingText.length())) {
//                         currentLine = remainingText;
//                         remainingText = "";
//                         break;
//                     }
//                 }
//
//                 if (!currentLine.empty()) {
//                     lines.push_back(currentLine);
//                 }
//             }
//
//             // Calculate the x-position based on the first line (as if it was centered alone)
//             auto [x, y] = MeasureTextEx(*font_ptr, lines[0].c_str(), fontSize, 0.0f);
//             const float leftEdgeX = pos.x - x / 2.0f;
//
//             // Calculate total height for centering
//             const float totalHeight = textSize.y * lines.size();
//             const float startY = pos.y - totalHeight / 2.0f + textSize.y / 2.0f;
//
//             // Draw each line with left alignment at the first line's left edge
//             for (size_t i = 0; i < lines.size(); i++) {
//                 Vec2 linePos = {leftEdgeX, startY + i * textSize.y};
//                 DrawTextEx(*font_ptr, lines[i].c_str(), linePos, fontSize, 0.0f, color);
//             }
//         }
//     };