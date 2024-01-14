#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"
#include "rlgl.h"
#include "rcamera.h"

#include <string>
#include <vector>
#include <iostream>

bool Quit = false;

bool ImGuiDemoOpen = false;

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
		DrawCubeWires({ 0 }, scale.x, scale.y, scale.z, RED);
		rlPopMatrix();
	}

	bool Open = true;

	Vector3 position = {};
	Vector3 scale = { 1, 1, 0.1 };
	Vector3 rotation = { 0, 0, 0 };
};

struct Project
{
	Vector3 eyePosition = { 0,0,0 };
	std::vector<Display> displays;

	RenderTexture perspectiveTexture;
	RenderTexture topTexture;
	RenderTexture frontTexture;
	RenderTexture rightTexture;

	Camera camera;
	Camera topCamera;
	Camera frontCamera;
	Camera rightCamera;
};

void UpdateDrawFrame(void* userData)
{
	auto project = (Project*)userData;
	auto& eyePosition = project->eyePosition;
	auto& displays = project->displays;
	auto& perspectiveTexture = project->perspectiveTexture;
	auto& topTexture = project->topTexture;
	auto& frontTexture = project->frontTexture;
	auto& rightTexture = project->rightTexture;
	auto& camera = project->camera;
	auto& topCamera = project->topCamera;
	auto& frontCamera = project->frontCamera;
	auto& rightCamera = project->rightCamera;

	BeginDrawing();
	ClearBackground(DARKGRAY);

	rlImGuiBegin();
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{
			}
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
			}
			if (ImGui::MenuItem("Save As.."))
			{
			}
#ifndef PLATFORM_WEB
			ImGui::Separator();
			if (ImGui::MenuItem("Exit"))
				Quit = true;
#endif // PLATFORM_WEB
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	auto drawArrow = [](ImVec2 position, ImU32 color, float length, float angle)
		{
			auto drawList = ImGui::GetWindowDrawList();
			angle = angle * DEG2RAD;
			ImVec2 secondPoint = { position.x + length * sin(angle), position.y - length * cos(angle) };
			drawList->AddLine(position, secondPoint, color, 1.0f);

			// Draw arrow head with lines
			ImVec2 arrowHead1 = { secondPoint.x + 10 * cos(angle + PI / 4 + PI / 2), secondPoint.y + 10 * sin(angle + PI / 4 + PI / 2) };
			ImVec2 arrowHead2 = { secondPoint.x + 10 * cos(angle - PI / 4 + PI / 2), secondPoint.y + 10 * sin(angle - PI / 4 + PI / 2) };
			drawList->AddLine(secondPoint, arrowHead1, color, 1.0f);
			drawList->AddLine(secondPoint, arrowHead2, color, 1.0f);
		};


	if (ImGuiDemoOpen)
		ImGui::ShowDemoWindow(&ImGuiDemoOpen);

	auto drawScene = [&]() {
		rlPushMatrix();
		rlTranslatef(eyePosition.x, eyePosition.y, eyePosition.z);
		DrawSphere({ 0,0,0 }, 0.1f, GREEN);
		for (auto& display : displays)
			display.draw();
		rlPopMatrix();
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

	int hoveringImage = -1;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::BeginChild("View", ImVec2(halfHeight * 2 + 48, 0), true, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);
	ImGui::PopStyleVar();
	ImGui::BeginChild("Camera", ImVec2(halfHeight + 20, halfHeight + 35), true);
	ImGui::Text("Perspective");
	auto perspectivePos = ImGui::GetCursorScreenPos();
	auto p = perspectivePos;
	ImGui::Image((ImTextureID)&perspectiveTexture.texture, ImVec2(halfHeight, halfHeight), ImVec2(0, 1), ImVec2(1, 0));
	if (ImGui::IsItemHovered())
		hoveringImage = 0;
	p.y = p.y + halfHeight - 10;
	p.x += 10;


	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("TopChild", ImVec2(halfHeight + 20, halfHeight + 35), true);
	ImGui::Text("Top");
	p = ImGui::GetCursorScreenPos();
	auto topPos = p;
	ImGui::Image((ImTextureID)&topTexture.texture, ImVec2(halfHeight, halfHeight), ImVec2(0, 1), ImVec2(1, 0));
	if (ImGui::IsItemHovered())
		hoveringImage = 1;
	p.y = p.y + halfHeight - 10;
	p.x += 10;
	drawArrow(p, IM_COL32(255, 0, 0, 255), 40, 0);
	drawArrow(p, IM_COL32(0, 0, 255, 255), 40, 90);
	ImGui::EndChild();
	ImGui::BeginChild("FrontChild", ImVec2(halfHeight + 20, halfHeight + 35), true);
	ImGui::Text("Front");
	p = ImGui::GetCursorScreenPos();
	auto frontPos = p;
	ImGui::Image((ImTextureID)&frontTexture.texture, ImVec2(halfHeight, halfHeight), ImVec2(0, 1), ImVec2(1, 0));
	if (ImGui::IsItemHovered())
		hoveringImage = 2;
	p.y = p.y + halfHeight - 10;
	p.x += 10;
	drawArrow(p, IM_COL32(0, 255, 0, 255), 40, 0);
	drawArrow(p, IM_COL32(255, 0, 0, 255), 40, 90);
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("RightChild", ImVec2(halfHeight + 20, halfHeight + 35), true);
	ImGui::Text("Right");
	p = ImGui::GetCursorScreenPos();
	auto rightPos = p;
	ImGui::Image((ImTextureID)&rightTexture.texture, ImVec2(halfHeight, halfHeight), ImVec2(0, 1), ImVec2(1, 0));
	if (ImGui::IsItemHovered())
		hoveringImage = 3;
	p.y = p.y + halfHeight - 10;
	p.x += 10;
	drawArrow(p, IM_COL32(0, 255, 0, 255), 40, 0);
	drawArrow(p, IM_COL32(0, 0, 255, 255), 40, 90);

	ImGui::EndChild();
	ImGui::EndChild();

	float mouseWheel = ImGui::GetIO().MouseWheel;
	if (mouseWheel != 0 && hoveringImage != -1)
	{
		auto& camera = hoveringImage == 0 ? project->camera : hoveringImage == 1 ? project->topCamera : hoveringImage == 2 ? project->frontCamera : project->rightCamera;
		camera.fovy += mouseWheel * 2;
		if (camera.fovy < 1)
			camera.fovy = 1;
		if (camera.fovy > 179)
			camera.fovy = 179;
	}

	static int selectedDisplay = -1;

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoveringImage != -1)
	{
		auto& camera = hoveringImage == 0 ? project->camera : hoveringImage == 1 ? project->topCamera : hoveringImage == 2 ? project->frontCamera : project->rightCamera;

		auto mousePos = GetMousePosition();

		if (hoveringImage == 0)
		{
			mousePos.y -= perspectivePos.y;
			mousePos.x -= perspectivePos.x;
		}
		else if (hoveringImage == 1)
		{
			mousePos.y -= topPos.y;
			mousePos.x -= topPos.x;
		}
		else if (hoveringImage == 2)
		{
			mousePos.y -= frontPos.y;
			mousePos.x -= frontPos.x;
		}
		else if (hoveringImage == 3)
		{
			mousePos.y -= rightPos.y;
			mousePos.x -= rightPos.x;
		}

		Ray ray = GetMouseRay(mousePos, camera);

		int index = 0;
		for (auto& display : displays)
		{
			// Create bounding box for display
			Vector3 min = { display.position.x - display.scale.x / 2, display.position.y - display.scale.y / 2, display.position.z - display.scale.z / 2 };
			Vector3 max = { display.position.x + display.scale.x / 2, display.position.y + display.scale.y / 2, display.position.z + display.scale.z / 2 };
			BoundingBox cube = { min, max };
			// Check collision between ray and box
			RayCollision collision = GetRayCollisionBox(ray, cube);
			if (collision.hit)
			{
				selectedDisplay = index;
				break;
			}
			index++;
		}
	}

	ImGui::SameLine();
	ImGui::BeginChild("Options", ImVec2(400, 0), true);

	ImGui::DragFloat3("Eye Position", (float*)&eyePosition, 0.1f);

	if (ImGui::BeginListBox("Displays"))
	{
		for (int i = 0; i < displays.size(); i++)
		{
			std::string label = "Display " + std::to_string(i);
			if (ImGui::Selectable(label.c_str(), selectedDisplay == i))
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
		float size = 200;
		float aspectRatio = 1 / 2;
		ImGui::DragFloat("Size", &size, 0.1f, 0.1f, 100.0f);
		ImGui::DragFloat("Aspect Ratio", &aspectRatio, 0.01f, 0.01f, 10.0f);

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

	Project cameraCalibration;

	cameraCalibration.camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE };
	cameraCalibration.topCamera = { { 0.0f, 10.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, 10.0f, CAMERA_ORTHOGRAPHIC };
	cameraCalibration.frontCamera = { { 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 10.0f, CAMERA_ORTHOGRAPHIC };
	cameraCalibration.rightCamera = { { 10.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 10.0f, CAMERA_ORTHOGRAPHIC };

#ifdef PLATFORM_WEB
	emscripten_set_main_loop_arg(UpdateDrawFrame, &cameraCalibration, 0, 1);
#else
	int width = 512;
	int height = 512;
	cameraCalibration.perspectiveTexture = LoadRenderTexture(width, height);
	cameraCalibration.topTexture = LoadRenderTexture(width, height);
	cameraCalibration.frontTexture = LoadRenderTexture(width, height);
	cameraCalibration.rightTexture = LoadRenderTexture(width, height);

	// Main game loop
	while (!WindowShouldClose() && !Quit)    // Detect window close button or ESC key
	{
		UpdateDrawFrame(&cameraCalibration);
	}
#endif

	UnloadRenderTexture(cameraCalibration.perspectiveTexture);
	UnloadRenderTexture(cameraCalibration.frontTexture);
	UnloadRenderTexture(cameraCalibration.rightTexture);
	UnloadRenderTexture(cameraCalibration.topTexture);

	rlImGuiShutdown();

	CloseWindow();        // Close window and OpenGL context
	return 0;
}