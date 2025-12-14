#include <iomanip>
#include "raylib.h"

#include "Vec2.hpp"
#include "Timer.hpp"
#include "Button.hpp"
#include "TextBox.hpp"

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


int main () {
    constexpr int InitScreenWidth = 1200, InitScreenHeight = 800;

    // Initialise window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(InitScreenWidth, InitScreenHeight, "Productivity Timer");
    SetExitKey(KEY_NULL);
    SetWindowMinSize(800,600);

    // Add custom font and smoothing for scaling
    const Font FontLarge = LoadFontEx("../Resources/segoeui.ttf", 128, nullptr, 0);
    const Font FontMedium = LoadFontEx("../Resources/segoeui.ttf", 96, nullptr, 0);
    const Font FontSmall = LoadFontEx("../Resources/segoeui.ttf", 54, nullptr, 0);
    SetTextureFilter(FontLarge.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(FontMedium.texture, TEXTURE_FILTER_BILINEAR);

    // Create Timer instance
    auto Tmr = Timer();
    Tmr.SetTimer(10);
    TextBox TimerTBx("10");

    // Add buttons
    Button ResetBtn("Reset", {35,35,35,255});
    Button StartBtn("Start", {0, 170, 70, 255});
    Button StopBtn("Stop", {145, 25, 25, 255});

    // Create function to add text to scene
    auto AddText = [&FontSmall, &FontMedium, &FontLarge](const char* text, const Vec2 &pos, const float fontSize, const Color color = TEXTCOL) {
        const Font *font_ptr;
        if (fontSize < 40)
            font_ptr = &FontSmall;
        else if (fontSize < 70)
            font_ptr = &FontMedium;
        else
            font_ptr = &FontLarge;

        const Vector2 textSize = MeasureTextEx(*font_ptr, text, fontSize, 0.0f);
        DrawTextEx(*font_ptr, text, (pos - Vec2(textSize)/2), fontSize, 0.0f, color);
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

        AddText("TODO LIST", {PanelWidth/2.0f, 60.0f * scale}, 50);
        AddText("FOCUS TIMER", {PanelWidth*1.5f, 60.0f * scale}, 50);

        /* === TIMER === */
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


        // Add timer controls
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

        // Add timer textbox
        TimerTBx.Draw(TimerCentre + Vec2{0.0f, -190.0f*scale}, Vec2{130.0f, 40.0f}*scale, 30.0f*scale, AddText);


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
                if ( (key >= '0' && key <= '9') && TimerTBx.lenText() < 4) {
                    TimerTBx.AppendChar(key);
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                TimerTBx.Backspace();
            }
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
                if (!Tmr.isRunning) {
                    Tmr.Reset();
                    int num = TimerTBx.GetNumFromTxt();
                    Tmr.SetTimer(num);
                    TimerTBx.Deselect();
                }
            }
        }

        EndDrawing();
    }

    UnloadFont(FontLarge);
    UnloadFont(FontMedium);
    CloseWindow();
}
