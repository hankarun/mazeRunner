#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"
#include "rlgl.h"

#include <string>
#include <vector>

bool Quit = false;

bool ImGuiDemoOpen = false;

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

void UpdateDrawFrame(void* userData)
{
	//BeginDrawing();
	//ClearBackground(DARKGRAY);

	//rlImGuiBegin();
	//DoMainMenu();

	//if (ImGuiDemoOpen)
	//	ImGui::ShowDemoWindow(&ImGuiDemoOpen);

	//if (mazeView->Open)
	//{
	//	drawInfo(mazeView->game);
	//	mazeView->Show();
	//}

	//rlImGuiEnd();

	//EndDrawing();
}

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#pragma comment(lib, "winmm.lib")
#endif

class Display
{
public:
	Display(Vector3 position, Vector3 scale)
		: position(position), scale(scale)
	{
	}

	void draw()
	{		
		rlPushMatrix();
		rlTranslatef(position.x, position.y, position.z);
		rlRotatef(rotation.x, 1, 0, 0);
		rlRotatef(rotation.y, 0, 1, 0);
		rlRotatef(rotation.z, 0, 0, 1);
		DrawCubeWires({0}, scale.x, scale.y, scale.z, RED);
		rlPopMatrix();
	}

	bool Open = true;

	Vector3 position = {};
	Vector3 scale = { 1, 1, 0.1 };
	Vector3 rotation = {0, 0, 0};
};

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

	std::vector<Display> displays;

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
			for (auto& display : displays)
				display.draw();
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
		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing());
		ImGui::BeginChild("Options", ImVec2(400, 0), true);
		static int selectedDisplay = -1;
		if (ImGui::BeginListBox("Displays"))
		{
			for (int i = 0; i < displays.size(); i++)
			{
				if (ImGui::Selectable(std::to_string(i).c_str(), selectedDisplay == i))
					selectedDisplay = i;
			}
			ImGui::EndListBox();
		}
		if (ImGui::Button("Add"))
		{
			displays.emplace_back(Vector3{ 0,0,0 }, Vector3{ 1,1,0.1f });
		}
		ImGui::SameLine();
		if (ImGui::Button("Remove"))
		{
			if (selectedDisplay >= 0 && selectedDisplay < displays.size())
				displays.erase(displays.begin() + selectedDisplay);
		}

		if (selectedDisplay >= 0 && selectedDisplay < displays.size())
		{
			ImGui::DragFloat3("Position", (float*)&displays[selectedDisplay].position, 0.1f);
			ImGui::DragFloat3("Scale", (float*)&displays[selectedDisplay].scale, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&displays[selectedDisplay].rotation, 0.1f);
		}

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