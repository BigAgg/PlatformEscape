#pragma once

#include "gameobject.h"

class PlayerBehaviorComponent : public Component {
public:
  PlayerBehaviorComponent(GameObject *parentObject);

  void Update(float deltatime) override;
  void Render() override;

  void ProcessInput();
  void HandleAnimationState();
  void SetVelocityOffset(Vector2 offset);

private:
  HealthComponent *m_healthComponent;
  TransformComponent *m_transformComponent;
  AnimatorComponent *m_animatorComponent;
  CollisionComponent *m_collisionComponent;
  VelocityComponent *m_velocityComponent;
  Vector2 m_offsetVelocity = {1.0f, 1.0f};
};

class TestBehaviorComponent : public Component {
public:
  TestBehaviorComponent(GameObject *parentObject);

  void Update(float deltatime) override;
  void Render() override;

private:
  GameObject *m_parentObject;
  Vector2 m_startpos;
  bool m_reverse;
};
