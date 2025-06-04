#pragma once

#include <memory>
#include <string>
#include <vector>

#include "gameobject.h"

class Level {
public:
  void AddGameObject(std::unique_ptr<GameObject> gameobject);
  void DeleteGameobject(std::unique_ptr<GameObject> gameobject);
  void LoadLevelFromFile(const std::string &filename);
  void SaveLevelToFile(const std::string &filename);

  void SetLevelName(const std::string &levelname);
  std::string GetLevelName();

  void Update(float deltatime);
  void Render();
  void Input();

  void SetLoaded();
  bool IsReady();

private:
  std::vector<std::unique_ptr<GameObject>> m_gameobjects;
  std::string m_levelname;
  bool m_loaded = false;
};
