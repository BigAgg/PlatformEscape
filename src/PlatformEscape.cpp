#include "PlatformEscape.h"
#include "gameobject.h"
#include "logging.h"
#include "objectloader.h"
#include <fstream>
#include <imgui.h>
#include <raylib.h>
#include <raymath.h>
#include <rlImGui.h>

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
  Camera2D gameCam;
} s_game;
// ImGui related
static struct {
  bool mainMenuBar = false;
  bool objectViewer = false;
} s_menus;

GameObject player;
GameObject testobject;

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
  s_game = {true, false, Camera2D({0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f, 3.0f)};
  // Initializing raylib
  loginfo("Starting up PlatformEscape...");
  InitWindow(640, 480, "PlatformEscape");
  if (!IsWindowReady()) {
    logerror("Window Creation went wrong! Raylib could not be initialized!");
  } else {
    loginfo("Started PlatformEscape!");
  }
  // JUST TESTING!!! REMOVE IN FINAL!
  GeneratePlayer(&player);
  GenerateTestobject(&testobject);
  // DebugGameObjects(true);
  //  END TESTING
  SetExitKey(KEY_BACKSPACE);

  // Make Window Resizable
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  // Loading settings
  LoadSettings();
  HandleWindowSettingChange();
  // Init ImGui
  rlImGuiSetup(true);
  ImGuiIO& io = ImGui::GetIO();
  io.IniFilename = nullptr;
}

void LoadSettings() {
  std::ifstream file(WINDOW_BIN);
  if (!file.is_open() || !file.good()) {
    LoadDefaultWindowSettings();
    return;
  }
  loginfo("Loading Window Settings from file...");
  file.read((char *)&s_window, sizeof(s_window));
  file.close();
}

void SaveSettings() {
  std::ofstream file(WINDOW_BIN);
  if (!file.is_open() || !file.good()) {
    logwarning("Could not save Window Settings to file: %s", WINDOW_BIN);
    return;
  }
  loginfo("Saving Window Settings...");
  file.write((char *)&s_window, sizeof(s_window));
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
}

static void Update(float deltatime) {
  // Update the Player
  if (s_game.running && !s_game.paused) {
    player.Update(deltatime);
    testobject.Update(deltatime);
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
    player.Render();
    testobject.Render();
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

  s_window = {1280,   720,   60,  width / 2 - 640, height / 2 - 360,
              device, false, true};
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
  } else {
    ClearWindowState(FLAG_VSYNC_HINT);
  }

  if (s_window.maximized && !IsWindowMaximized()) {
    MaximizeWindow();
    UpdateWindowSettings();
  } else if (!s_window.maximized && IsWindowMaximized()) {
    RestoreWindow();
    UpdateWindowSettings();
  }
}
