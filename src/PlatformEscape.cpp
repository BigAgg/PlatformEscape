#include "PlatformEscape.h"
#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>
#include <raymath.h>
#include <fstream>
#include "logging.h"
#include "Player.h"
#include "gameobject.h"

using namespace logging;

// Settings
static struct {
	int width, height, fps;
	int posx, posy, device;
	bool vsync, maximized;
} s_window;
// Game related
static struct {
	bool running, paused;
	Player *player;
	Camera2D gameCam;
} s_game;
// ImGui related
static struct {
	bool mainMenuBar = false;
	bool objectViewer = false;
} s_menus;

GameObject player;

#define WINDOW_BIN "window.bin"

// Predefined Functions
static void Input(float deltatime);
static void Update(float deltatime);
static void Render();
static void LoadDefaultWindowSettings();
static void HandleWindowSettingChange();
static void UpdateWindowSettings();

void Init() {
	// Initializing s_game for testing running is true
	s_game = { true, false, new Player(20, {100.0f, 400.0f}), Camera2D({0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f, 3.0f) };
	// Initializing raylib
	loginfo("Starting up PlatformEscape...");
	InitWindow(640, 480, "PlatformEscape");
	if (!IsWindowReady()) {
		logerror("Window Creation went wrong! Raylib could not be initialized!");
	}
	else {
		loginfo("Started PlatformEscape!");
	}
	// JUST TESTING!!! REMOVE IN FINAL!
	DebugGameObjects(true);
	player.AddComponent<TransformComponent>(Vector2(20.0f, 20.0f));
	player.AddComponent<HealthComponent>(100, 0, 100);
	player.AddComponent<CollisionComponent>(Rectangle(0, 0, 16, 16), &player);
	player.AddComponent<AnimatorComponent>();
	player.AddComponent<VelocityComponent>(Vector2(0.0f, 0.0f), 800.0f, 200.0f);
	player.GetComponent<VelocityComponent>()->grounded = true;
	player.AddComponent<PlayerBehaviorComponent>(&player);
	auto&& ac = player.GetComponent<AnimatorComponent>();
	Image img = LoadImage("character.png");
	std::vector<Texture> anim(4);
	for (int x = 0; x < 4; x++) {
		Image tmp = ImageFromImage(img, Rectangle(0 + x * 16, 38, 16, 22));
		anim[x] = LoadTextureFromImage(tmp);
	}
	AnimationComponent animc(&player);
	animc.AddFrames(anim);
	ac->AddAnimation("walk_right", animc);
	for (int x = 0; x < 4; x++) {
		Image tmp = ImageFromImage(img, Rectangle(0 + x * 16, 102, 16, 22));
		anim[x] = LoadTextureFromImage(tmp);
	}
	AnimationComponent walk_left(&player);
	walk_left.AddFrames(anim);
	ac->AddAnimation("walk_left", walk_left);
	anim.resize(3);
	for (int x = 0; x < 3; x++) {
		Image tmp = ImageFromImage(img, Rectangle(80 + x * 16, 6, 16, 22));
		anim[x] = LoadTextureFromImage(tmp);
	}
	AnimationComponent stand(&player);
	stand.AddFrames(anim);
	ac->AddAnimation("stand", stand);
	// END TESTING
	SetExitKey(0);

	// Make Window Resizable
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	
	// Loading settings
	LoadSettings();
	HandleWindowSettingChange();
	// Init ImGui
	rlImGuiSetup(true);
}

void LoadSettings() {
	std::ifstream file(WINDOW_BIN);
	if (!file.is_open() || !file.good()) {
		LoadDefaultWindowSettings();
		return;
	}
	loginfo("Loading Window Settings from file...");
	file.read((char*)&s_window, sizeof(s_window));
	file.close();
}

void SaveSettings() {
	std::ofstream file(WINDOW_BIN);
	if (!file.is_open() || !file.good()) {
		logwarning("Could not save Window Settings to file: %s", WINDOW_BIN);
		return;
	}
	loginfo("Saving Window Settings...");
	file.write((char*)&s_window, sizeof(s_window));
	file.close();
}

void Run() {
	loginfo("Running Game!");
	while (!WindowShouldClose()) {
		if (IsWindowResized()) {
			UpdateWindowSettings();
			HandleWindowSettingChange();
		}
		float deltatime = GetFrameTime();
		Input(deltatime);
		Update(deltatime);
		Render();
	}
}

void Shutdown() {
	loginfo("Shutting down Game...");
	UpdateWindowSettings();
	SaveSettings();
	rlImGuiShutdown();
	CloseWindow();
}

static void Input(float deltatime) {
	// Standard game Inputs
	if (IsKeyPressed(KEY_ESCAPE)) {
		s_game.paused = !s_game.paused;
	}
	// Other inputs
	if (s_game.running && !s_game.paused) {
		s_game.player->Input();
	}
}

static void Update(float deltatime) {
	// Update the Player
	if (s_game.running && !s_game.paused) {
		s_game.player->Update(deltatime);
		player.Update(deltatime);
	}
}

static void RenderImGui() {
	// Variables to handle certain windows
	if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_M))
		s_menus.mainMenuBar = !s_menus.mainMenuBar;
	
	rlImGuiBegin();
	// Drawing MainMenuBar
	if (s_menus.mainMenuBar && ImGui::BeginMainMenuBar()) {
		if (ImGui::MenuItem("Setup", "", nullptr)) {
			loginfo("Setup pressed");
		}
		ImGui::EndMainMenuBar();
	}
	// Drawing GameObject Viewer
	if (ImGui::Begin("Test Window")) {
		ImGui::TextUnformatted(ICON_FA_JEDI);
	}
	ImGui::End();
	rlImGuiEnd();
}

static void Render() {
	BeginDrawing();
	ClearBackground(BLACK);
	DrawFPS(0, 0);
	BeginMode2D(s_game.gameCam);
	if (s_game.running) {
		s_game.player->Render();
		player.Render();
	}
	EndMode2D();
	RenderImGui();
	EndDrawing();
}

static void LoadDefaultWindowSettings() {
	loginfo("Loading default Window Settings...");
	int device = GetCurrentMonitor();
	int width = GetMonitorWidth(device);
	int height = GetMonitorHeight(device);

	s_window = { 1280, 720, 60, width / 2 - 640, height / 2 - 360, device, false, true };
}

static void UpdateWindowSettings() {
	s_window.device = GetCurrentMonitor();
	s_window.maximized = IsWindowMaximized();
	s_window.width = GetScreenWidth();
	s_window.height = GetScreenHeight();
	const Vector2 windowPos = GetWindowPosition();
	s_window.posx = static_cast<int>(windowPos.x);
	s_window.posy = static_cast<int>(windowPos.y);
	s_window.vsync = IsWindowState(FLAG_VSYNC_HINT);
}

static void HandleWindowSettingChange() {
	SetWindowSize(s_window.width, s_window.height);
	SetWindowPosition(s_window.posx, s_window.posy);
	SetTargetFPS(s_window.fps);
	if (s_window.vsync) {
		SetWindowState(FLAG_VSYNC_HINT);
	}
	else {
		ClearWindowState(FLAG_VSYNC_HINT);
	}

	if (s_window.maximized && !IsWindowMaximized()) {
		MaximizeWindow();
		UpdateWindowSettings();
	}
	else if (!s_window.maximized && IsWindowMaximized()) {
		RestoreWindow();
		UpdateWindowSettings();
	}
}