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
		: game(800, 800) {}
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

		if (ImGui::Begin("Maze View", 0, ImGuiWindowFlags_NoScrollbar))
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
			viewRect.x = 0;
			viewRect.y = 0;
			viewRect.width = size.x;
			viewRect.height = -size.y;

			// draw the view
			ImGui::Image((ImTextureID)&ViewTexture.texture, ImVec2(float(size.x), float(size.y)));
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
		drawer.draw(&game.board, game.showColor);
		drawer.draw(&game.boardGenerator);
		EndTextureMode();
	}

	maze::Game game;
};

void DoMainMenu()
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

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void drawInfo(maze::Game& game)
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));
	if (ImGui::Begin("Control"))
	{
		ImGui::BeginTabBar("Control");
		if (ImGui::BeginTabItem("Settings"))
		{
			ImGui::Checkbox("Show Color", &game.showColor);
			ImGui::Checkbox("Loop", &game.loop);
			ImGui::DragInt("Speed", &game.speed, 1.0f, 0, 100);
			ImGui::DragInt("Cell Size", &game.cellSize, 1.0f, 0, 100);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Generate"))
		{
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
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Solve"))
		{
			if (ImGui::Button("Start Solve"))
			{
			}
			if (ImGui::Button("Finish Solve"))
			{
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void UpdateDrawFrame(void* userData)
{
	auto mazeView = (MazeViewerWindow*)userData;
	mazeView->Update();

	BeginDrawing();
	ClearBackground(DARKGRAY);

	rlImGuiBegin();
	DoMainMenu();

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

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "raylib-Extras [ImGui] example - ImGui Demo");
	SetTargetFPS(144);
	rlImGuiSetup(true);
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

	Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE };
	Camera topCamera = { { 0.0f, 10.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, 10.0f, CAMERA_ORTHOGRAPHIC };
	Camera frontCamera = { { 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 10.0f, CAMERA_ORTHOGRAPHIC };
	Camera rightCamera = { { 10.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 10.0f, CAMERA_ORTHOGRAPHIC };

#ifdef PLATFORM_WEB
	emscripten_set_main_loop_arg(UpdateDrawFrame, &mazeView, 0, 1);
#else
	int width = 512;
	int height = 512;
	RenderTexture perspectiveTexture = LoadRenderTexture(width, height);
	RenderTexture topTexture = LoadRenderTexture(width, height);
	RenderTexture frontTexture = LoadRenderTexture(width, height);
	RenderTexture rightTexture = LoadRenderTexture(width, height);

	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
	Vector3 cylinderPosition = { 4.0f, 0.0f, -2.0f };

	// Main game loop
	while (!WindowShouldClose() && !Quit)    // Detect window close button or ESC key
	{
		BeginDrawing();
		ClearBackground(DARKGRAY);

		rlImGuiBegin();
		DoMainMenu();

		if (ImGuiDemoOpen)
			ImGui::ShowDemoWindow(&ImGuiDemoOpen);

		auto drawScene = [&]() {
			DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, RED);
			DrawCylinderWires(cylinderPosition, 1.0f, 2.0f, 1.0f, 4, DARKBLUE);
			DrawGrid(10, 1.0f);
			};


		BeginTextureMode(perspectiveTexture);
		ClearBackground(DARKGRAY);
		BeginMode3D(camera);
		drawScene();
		EndMode3D();
		EndTextureMode();

		BeginTextureMode(topTexture);
		ClearBackground(DARKGRAY);
		BeginMode3D(topCamera);
		drawScene();
		EndMode3D();
		EndTextureMode();

		BeginTextureMode(frontTexture);
		ClearBackground(DARKGRAY);
		BeginMode3D(frontCamera);
		drawScene();
		EndMode3D();
		EndTextureMode();

		BeginTextureMode(rightTexture);
		ClearBackground(DARKGRAY);
		BeginMode3D(rightCamera);
		drawScene();
		EndMode3D();
		EndTextureMode();

		ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
		auto windowSize = ImGui::GetMainViewport()->Size;
		windowSize.y -= ImGui::GetFrameHeight();
		ImGui::SetNextWindowSize(windowSize);
		ImGui::Begin("Camera", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration);
		auto maxHeight = ImGui::GetContentRegionMax();
		auto halfHeight = maxHeight.y / 2 - 40;

		ImGui::BeginChild("Camera", ImVec2(halfHeight + 20, halfHeight + 35), true);
		ImGui::Text("Perspective");
		ImGui::Image((ImTextureID)&perspectiveTexture.texture, ImVec2(halfHeight, halfHeight), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("TopChild", ImVec2(halfHeight + 20, halfHeight + 35), true);
		ImGui::Text("Top");
		ImGui::Image((ImTextureID)&topTexture.texture, ImVec2(halfHeight, halfHeight), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
		ImGui::BeginChild("FrontChild", ImVec2(halfHeight + 20, halfHeight + 35), true);
		ImGui::Text("Front");
		ImGui::Image((ImTextureID)&frontTexture.texture, ImVec2(halfHeight, halfHeight), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("RightChild", ImVec2(halfHeight + 20, halfHeight + 35), true);
		ImGui::Text("Right");
		ImGui::Image((ImTextureID)&rightTexture.texture, ImVec2(halfHeight, halfHeight), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild("Options");
		ImGui::DragFloat3("Cube Position", (float*)&cubePosition, 0.1f);
		ImGui::DragFloat3("Cylinder Position", (float*)&cylinderPosition, 0.1f);
		ImGui::EndChild();

		ImGui::End();

		rlImGuiEnd();

		EndDrawing();

		if (perspectiveTexture.texture.width != (int)halfHeight || perspectiveTexture.texture.height != (int)halfHeight)
		{
			UnloadRenderTexture(perspectiveTexture);
			UnloadRenderTexture(topTexture);
			UnloadRenderTexture(frontTexture);
			UnloadRenderTexture(rightTexture);
			perspectiveTexture = LoadRenderTexture(halfHeight, halfHeight);
			topTexture = LoadRenderTexture(halfHeight, halfHeight);
			frontTexture = LoadRenderTexture(halfHeight, halfHeight);
			rightTexture = LoadRenderTexture(halfHeight, halfHeight);
		}
		//----------------------------------------------------------------------------------
	}
#endif
	UnloadRenderTexture(perspectiveTexture);
	UnloadRenderTexture(frontTexture);
	UnloadRenderTexture(rightTexture);

	rlImGuiShutdown();

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}