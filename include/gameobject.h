#pragma once

#include <memory>
#include <raylib.h>
#include <string>
#include <vector>

class Component;
class GameObject;

void DebugGameObjects(bool debug);

class Component {
public:
  virtual void Update(float deltatime) = 0;
  virtual void Render() = 0;
};

class GameObject {
public:
  void Update(float deltatime);
  void Render();

  template <typename T, typename... Args> void AddComponent(Args &&...args) {
    static_assert(
        std::is_base_of<Component, T>::value,
        "In: GameObject::AddComponent -> T must inherit from Component");
    components.push_back(std::make_unique<T>(std::forward<Args>(args)...));
  }

  template <typename T> bool HasComponent() const {
    static_assert(
        std::is_base_of<Component, T>::value,
        "In: GameObject::HasComponent -> T must inherit from Component");
    for (const auto &comp : components) {
      if (dynamic_cast<T *>(comp.get()) != nullptr) {
        return true;
      }
    }
    return false;
  }

  template <typename T> T *GetComponent() const {
    static_assert(
        std::is_base_of<Component, T>::value,
        "In: GameObject::GetComponent -> T must inherit from Component");
    for (const auto &comp : components) {
      if (T *casted = dynamic_cast<T *>(comp.get())) {
        return casted;
      }
    }
    return nullptr;
  }

  template <typename T> std::vector<T *> GetComponents() const {
    static_assert(
        std::is_base_of<Component, T>::value,
        "In: GameObject::GetComponents -> T must inherit from Component");
    std::vector<T *> matches;
    for (const auto &comp : components) {
      if (T *casted = dynamic_cast<T *>(comp.get()))
        matches.push_back(casted);
    }
    return matches;
  }

private:
  std::vector<std::unique_ptr<Component>> components;
};

// Components
class TransformComponent : public Component {
public:
  TransformComponent(const Vector2 position);

  void Update(float deltatime) override;
  void Render() override;

  Vector2 GetPosition();
  void SetPosition(const Vector2 position);

private:
  Vector2 m_position;
};

class VelocityComponent : public Component {
public:
  VelocityComponent(Vector2 velocity, float gravity, float speed);

  void Update(float deltatime) override;
  void Render() override {};

  Vector2 velocity;
  float gravity, speed;
  bool grounded = false;
};

class CollisionComponent : public Component {
public:
  CollisionComponent(const Rectangle hitbox, GameObject *parentObject);

  void Update(float deltatime) override;
  void Render() override;

  void AddLayer(int layer);
  void RemoveLayer(int layer);
  bool HasLayer(int layer);
  std::vector<int> GetLayers;

private:
  bool m_layers[30] = {false};
  Rectangle m_hitbox;
  GameObject *m_gameObject;
};

class TextureComponent : public Component {
public:
  TextureComponent(Texture texture, GameObject *parentObject);

  void Update(float deltatime) override;
  void Render() override;

  Texture GetTexture();
  unsigned int GetTextureID();

private:
  Texture m_texture;
  Vector2 m_position = {0.0f, 0.0f};
  GameObject *m_gameObject;
};

class AnimationComponent : public Component {
public:
  AnimationComponent(GameObject *parentObject);

  void Update(float deltatime) override;
  void Render() override;

  void AddFrame(Texture frame);
  void AddFrames(std::vector<Texture> frames);
  void SetFrametime(float frametime);
  void SetCurrentFrame(unsigned int frame);
  void SetBaseFrame(unsigned int baseFrame);
  unsigned int GetCurrentFrame();

  void Play();
  void PlayRepeat();
  void Pause();
  void Stop();

  bool IsPlaying();
  bool IsPaused();

private:
  bool m_playRepeat = false;
  bool m_playing = false;
  bool m_paused = false;
  GameObject *m_gameObject;
  std::vector<Texture> m_frames;
  unsigned int m_currentFrame = 1;
  unsigned int m_baseFrame = 1;
  float m_frametime = 0.2f;
  float m_timePassed = 0.0f;
};

class AnimatorComponent : public Component {
public:
  void Update(float deltatime) override;
  void Render() override;

  void AddAnimation(const std::string &name,
                    const AnimationComponent &animation);
  void PlayAnimation(const std::string &name);
  void PlayAnimationRepeat(const std::string &name);
  void PauseCurrentAnimation();
  void StopCurrentAnimation();

private:
  std::vector<std::pair<std::string, AnimationComponent>> m_animations;
  unsigned int m_currentAnimation;
};

class HealthComponent : public Component {
public:
  HealthComponent(int health, int maxhealth, int minhealth);

  void Update(float deltatime) override {};
  void Render() override {};

  void SetHealth(int health);
  int GetHealth();
  int GetMaxHealth();
  int GetMinHealth();
  void SetInvurnable(bool invurnable);
  bool Invurnable();
  void Heal(int amount);
  void Damage(int amount);
  bool Dead();

private:
  bool m_dead = false;
  bool m_invurnable = false;
  int m_health;
  int m_minHealth;
  int m_maxHealth;
};
