#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"

#include <game.h>
#include <gamedrawer.h>

bool Quit = false;

bool ImGuiDemoOpen = false;

class DocumentWindow
{
public:
    bool Open = false;

    RenderTexture ViewTexture;

    virtual void Setup() = 0;
    virtual void Shutdown() = 0;
    virtual void Show() = 0;
    virtual void Update() = 0;

    bool Focused = false;
};

class MazeViewerWindow : public DocumentWindow
{
public:
    MazeViewerWindow()
        : game(800, 800){}
    virtual void Setup() override
    {
        ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

        game.init();
        game.updateCanvasSize(GetScreenWidth(), GetScreenHeight());
    }

    virtual void Shutdown() override
    {
    }

    virtual void Show() override
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

        if (ImGui::Begin("Maze View", &Open, ImGuiWindowFlags_NoScrollbar))
        {
            Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);

            ImVec2 size = ImGui::GetContentRegionAvail();

            if (ViewTexture.texture.width != size.x || ViewTexture.texture.height != size.y)
            {
                UnloadRenderTexture(ViewTexture);
                ViewTexture = LoadRenderTexture(size.x, size.y);
                game.updateCanvasSize(size.x, size.y);
            }

            Rectangle viewRect = { 0 };
            viewRect.x = ViewTexture.texture.width / 2 - size.x / 2;
            viewRect.y = ViewTexture.texture.height / 2 - size.y / 2;
            viewRect.width = size.x;
            viewRect.height = -size.y;

            // draw the view
            rlImGuiImageRect(&ViewTexture.texture, (int)size.x, (int)size.y, viewRect);
        }
        ImGui::End();

        ImGui::PopStyleVar();
    }

    virtual void Update() override
    {
        game.update(0);
        BeginTextureMode(ViewTexture);
        ClearBackground(SKYBLUE);
        GameDrawer drawer(&game);
        drawer.draw(&game.board, game.showColor, &game.colorer);
        drawer.draw(&game.boardGenerator);
        EndTextureMode();
    }

    maze::Game game;
};

void DoMainMenu(MazeViewerWindow& mazeView)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
                Quit = true;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            ImGui::MenuItem("ImGui Demo", nullptr, &ImGuiDemoOpen);
            ImGui::MenuItem("Maze View", nullptr, &mazeView.Open);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void drawInfo(maze::Game& game)
{
    ImGui::Checkbox("Show Color", &game.showColor);
    ImGui::Checkbox("Loop", &game.loop);
    ImGui::DragInt("Speed", &game.speed, 1.0f, 0, 100);
    ImGui::DragInt("Cell Size", &game.cellSize, 1.0f, 0, 100);
    if (ImGui::Button("Finish Generating"))
    {
        if (game.boardGenerator.isFinished())
        {
            game.reset();
        }

        game.boardGenerator.finish(&game.board);
    }
    if (ImGui::Button("Reset"))
    {
        game.reset();
        game.speed = 1;
    }
}

void UpdateDrawFrame(void* userData)
{
    auto mazeView = (MazeViewerWindow*)userData;
    mazeView->Update();

    BeginDrawing();
    ClearBackground(DARKGRAY);

    rlImGuiBegin();
    DoMainMenu(*mazeView);

    if (ImGuiDemoOpen)
        ImGui::ShowDemoWindow(&ImGuiDemoOpen);

    if (mazeView->Open)
    {
        drawInfo(mazeView->game);
        mazeView->Show();
    }

    rlImGuiEnd();

    EndDrawing();
}

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#pragma comment(lib, "winmm.lib")
#endif

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1900;
    int screenHeight = 900;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "raylib-Extras [ImGui] example - ImGui Demo");
    SetTargetFPS(144);
    rlImGuiSetup(true);
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

    MazeViewerWindow mazeView;
    mazeView.Setup();
    mazeView.Open = true;

#ifdef PLATFORM_WEB
    emscripten_set_main_loop_arg(UpdateDrawFrame, &mazeView, 0, 1);
#else
    // Main game loop
    while (!WindowShouldClose() && !Quit)    // Detect window close button or ESC key
    {
        mazeView.Update();

        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();
        DoMainMenu(mazeView);

        if (ImGuiDemoOpen)
            ImGui::ShowDemoWindow(&ImGuiDemoOpen);

        if (mazeView.Open)
        {
            drawInfo(mazeView.game);
            mazeView.Show();
        }

        rlImGuiEnd();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
#endif


    rlImGuiShutdown();

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}