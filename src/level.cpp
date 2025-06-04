#include "level.h"
#include "logging.h"
#include "utils.h"
#include <filesystem>
#include <fstream>
#include <memory>

void Level::AddGameObject(std::unique_ptr<GameObject> object) {
  m_gameobjects.emplace_back(std::move(object));
}

void Level::DeleteGameobject(std::unique_ptr<GameObject> object) {
  auto it = std::find(m_gameobjects.begin(), m_gameobjects.end(), object);
  if (it != m_gameobjects.end())
    m_gameobjects.erase(it);
}

void Level::LoadLevelFromFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    logging::logwarning(
        "level.cpp::Level::LoadLevelFromFile Unable to load file:\n%s",
        filename.c_str());
    return;
  }
  m_levelname = utils::files::LoadStringFromFile(file);
}

void Level::SaveLevelToFile(const std::string &filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    logging::logwarning(
        "level.cpp::Level::SaveLevelToFile Unable to save file:\n%s",
        filename.c_str());
    return;
  }
  utils::files::SaveStringToFile(file, m_levelname);
}

void Level::SetLevelName(const std::string &levelname) {
  m_levelname = levelname;
}

std::string Level::GetLevelName() { return m_levelname; }

void Level::Update(float deltatime) {
  for (auto &object : m_gameobjects) {
    object->Update(deltatime);
  }
}

void Level::Render() {
  for (auto &object : m_gameobjects) {
    object->Render();
  }
}

void Level::Input() {
  // Dunno if this function is being used later on
}

void Level::SetLoaded() { m_loaded = true; }

bool Level::IsReady() { return m_loaded; }
