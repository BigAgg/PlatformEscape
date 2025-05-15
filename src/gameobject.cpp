#include "gameobject.h"
#include "logging.h"
#include <algorithm>
#include <raylib.h>
#include <raymath.h>

static bool debug = false;
void DebugGameObjects(bool d) { debug = d; }

void GameObject::Update(float deltatime) {
  for (auto &comp : components) {
    comp->Update(deltatime);
  }
}

void GameObject::Render() {
  for (auto &comp : components) {
    comp->Render();
  }
}

// Now start with definitions of Components
TransformComponent::TransformComponent(Vector2 position)
    : m_position(position) {}

void TransformComponent::Update(float deltatime) {}
void TransformComponent::Render() {
  if (debug) {
    DrawCircleV(m_position, 2.0f, RED);
  }
}

Vector2 TransformComponent::GetPosition() { return m_position; }
void TransformComponent::SetPosition(const Vector2 position) {
  m_position = position;
}

VelocityComponent::VelocityComponent(Vector2 velocity, float gravity,
                                     float speed)
    : velocity(velocity), gravity(gravity), speed(speed) {}

void VelocityComponent::Update(float deltatime) {
  if (grounded)
    return;
  velocity.y += gravity * deltatime;
}

CollisionComponent::CollisionComponent(const Rectangle hitbox,
                                       GameObject *parentObject)
    : m_hitbox(hitbox), m_gameObject(parentObject) {}

void CollisionComponent::Update(float deltatime) {
  auto &&c = m_gameObject->GetComponent<TransformComponent>();
  if (c) {
    const Vector2 pos = c->GetPosition();
    m_hitbox.x = pos.x;
    m_hitbox.y = pos.y;
  }
}
void CollisionComponent::Render() {
  if (debug) {
    DrawRectangleRec(m_hitbox, ORANGE);
  }
}

void CollisionComponent::AddLayer(int layer) {
  if (layer > 29)
    return;
  m_layers[layer] = true;
}

void CollisionComponent::RemoveLayer(int layer) {
  if (layer > 29)
    return;
  m_layers[layer] = false;
}

bool CollisionComponent::HasLayer(int layer) {
  if (layer > 29)
    return false;
  return m_layers[layer];
}

TextureComponent::TextureComponent(Texture texture, GameObject *parentObject)
    : m_texture(texture), m_gameObject(parentObject) {}

void TextureComponent::Update(float deltatime) {
  auto &&c = m_gameObject->GetComponent<TransformComponent>();
  if (c) {
    m_position = c->GetPosition();
  }
}
void TextureComponent::Render() { DrawTextureV(m_texture, m_position, WHITE); }

Texture TextureComponent::GetTexture() { return m_texture; }
unsigned int TextureComponent::GetTextureID() { return m_texture.id; }

AnimationComponent::AnimationComponent(GameObject *parentObject)
    : m_gameObject(parentObject) {};

void AnimationComponent::Update(float deltatime) {
  size_t size = m_frames.size();
  if (!m_playing || m_paused || size <= 0)
    return;
  m_timePassed += deltatime;
  if (m_timePassed >= m_frametime) {
    m_timePassed = 0.0f;
    m_currentFrame++;
    if (m_currentFrame > size) {
      if (!m_playRepeat) {
        m_currentFrame = m_baseFrame;
        m_playing = false;
      } else {
        m_currentFrame = 1;
      }
    }
  }
}

void AnimationComponent::Render() {
  if (m_frames.size() <= 0)
    return;
  DrawTextureV(m_frames[m_currentFrame - 1],
               m_gameObject->GetComponent<TransformComponent>()->GetPosition(),
               WHITE);
}

void AnimationComponent::AddFrame(Texture frame) {
  if (!IsTextureValid(frame))
    return;
  m_frames.resize(m_frames.size() + 1);
  m_frames[m_frames.size() - 1] = frame;
}
void AnimationComponent::AddFrames(std::vector<Texture> frames) {
  for (Texture &t : frames) {
    AddFrame(t);
  }
}
void AnimationComponent::SetFrametime(float frametime) {
  m_frametime = frametime;
}
void AnimationComponent::SetCurrentFrame(unsigned int frame) {
  if (frame > m_frames.size())
    return;
  m_currentFrame = frame;
}
void AnimationComponent::SetBaseFrame(unsigned int baseFrame) {
  if (baseFrame > m_frames.size())
    return;
  m_baseFrame = baseFrame;
}
unsigned int AnimationComponent::GetCurrentFrame() { return m_currentFrame; }

void AnimationComponent::Play() { m_playing = true; }
void AnimationComponent::PlayRepeat() {
  m_playing = true;
  m_playRepeat = true;
}
void AnimationComponent::Pause() { m_paused = true; }
void AnimationComponent::Stop() {
  m_playing = false;
  m_playRepeat = false;
  m_timePassed = 0.0f;
  m_currentFrame = m_baseFrame;
}

bool AnimationComponent::IsPlaying() { return m_playing; }
bool AnimationComponent::IsPaused() { return m_paused; }

void AnimatorComponent::Update(float deltatime) {
  if (m_animations.size() <= 0)
    return;
  m_animations[m_currentAnimation].second.Update(deltatime);
}
void AnimatorComponent::Render() {
  if (m_animations.size() <= 0)
    return;
  m_animations[m_currentAnimation].second.Render();
}

void AnimatorComponent::AddAnimation(const std::string &name,
                                     const AnimationComponent &animation) {
  for (auto &&a : m_animations) {
    if (a.first == name)
      return;
  }
  m_animations.emplace_back(name, animation);
}

void AnimatorComponent::PlayAnimation(const std::string &name) {
  int x = 0;
  for (auto &&a : m_animations) {
    x++;
    if (a.first != name)
      continue;
    if (x - 1 != m_currentAnimation)
      m_animations[m_currentAnimation].second.Stop();
    a.second.Play();
    m_currentAnimation = x - 1;
    return;
  }
}
void AnimatorComponent::PlayAnimationRepeat(const std::string &name) {
  int x = 0;
  for (auto &&a : m_animations) {
    x++;
    if (a.first != name)
      continue;
    a.second.PlayRepeat();
    m_currentAnimation = x - 1;
    return;
  }
}
void AnimatorComponent::PauseCurrentAnimation() {
  m_animations[m_currentAnimation].second.Pause();
}
void AnimatorComponent::StopCurrentAnimation() {
  m_animations[m_currentAnimation].second.Stop();
}

HealthComponent::HealthComponent(int health, int maxhealth, int minhealth)
    : m_health(health), m_maxHealth(maxhealth), m_minHealth(minhealth) {}

void HealthComponent::SetHealth(int health) {
  if (health < m_minHealth) {
    m_health = m_minHealth;
  } else if (health > m_maxHealth)
    m_health = m_maxHealth;
  else
    m_health = health;
  if (m_health <= 0)
    m_dead = true;
}

int HealthComponent::GetHealth() { return m_health; }
int HealthComponent::GetMaxHealth() { return m_maxHealth; }
int HealthComponent::GetMinHealth() { return m_minHealth; }
void HealthComponent::SetInvurnable(bool invurnable) {
  m_invurnable = invurnable;
}
bool HealthComponent::Invurnable() { return m_invurnable; }
void HealthComponent::Heal(int amount) {
  m_health += amount;
  if (m_health > m_maxHealth)
    m_health = m_maxHealth;
}
void HealthComponent::Damage(int amount) {
  m_health -= amount;
  if (m_health <= m_minHealth)
    m_health = m_minHealth;
  if (m_health <= 0)
    m_dead = true;
}
bool HealthComponent::Dead() { return m_dead; }
