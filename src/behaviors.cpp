#include "behaviors.h"
#include "gameobject.h"
#include "logging.h"
#include <raylib.h>
#include <raymath.h>

PlayerBehaviorComponent::PlayerBehaviorComponent(GameObject *parentObj) {
  m_transformComponent = parentObj->GetComponent<TransformComponent>();
  if (!m_transformComponent)
    logging::logerror(
        "[PlayerBehaviorComponent] Parent Object has no TransformComponent");
  m_velocityComponent = parentObj->GetComponent<VelocityComponent>();
  if (!m_velocityComponent)
    logging::logerror(
        "[PlayerBehaviorComponent] Parent Object has no VelocityComponent");
  m_animatorComponent = parentObj->GetComponent<AnimatorComponent>();
  if (!m_animatorComponent)
    logging::logerror(
        "[PlayerBehaviorComponent] Parent Object has no AnimatorComponent");
  m_healthComponent = parentObj->GetComponent<HealthComponent>();
  if (!m_healthComponent)
    logging::logerror(
        "[PlayerBehaviorComponent] Parent Object has no HealthComponent");
  m_collisionComponent = parentObj->GetComponent<CollisionComponent>();
  if (!m_collisionComponent)
    logging::logerror(
        "[PlayerBehaviorComponent] Parent Object has no CollisionComponent");
}

void PlayerBehaviorComponent::Update(float deltatime) {
  ProcessInput();
  HandleAnimationState();
  if (m_velocityComponent->velocity.y > 800)
    m_velocityComponent->velocity.y = 800;
  Vector2 pos = m_transformComponent->GetPosition();
  Vector2 computedVelocity = m_velocityComponent->velocity;
  computedVelocity *= m_offsetVelocity;

  pos.x += computedVelocity.x * m_velocityComponent->speed * deltatime;
  if (!m_velocityComponent->grounded) {
    pos.y += computedVelocity.y * deltatime;
  } else {
    m_velocityComponent->velocity.y = 0;
  }
  m_transformComponent->SetPosition(pos);
}

void PlayerBehaviorComponent::Render() {}

void PlayerBehaviorComponent::ProcessInput() {
  m_velocityComponent->velocity.x = 0.0f;
  // Handle movement
  if (IsKeyDown(KEY_A)) {
    m_velocityComponent->velocity.x -= 1;
  }
  if (IsKeyDown(KEY_D)) {
    m_velocityComponent->velocity.x += 1;
  }
  if (IsKeyPressed(KEY_SPACE)) {
    m_velocityComponent->velocity.y = -400.0f;
    m_velocityComponent->grounded = false;
  }
}

void PlayerBehaviorComponent::HandleAnimationState() {
  if (m_velocityComponent->velocity.x > 0)
    m_animatorComponent->PlayAnimation("walk_right");
  else if (m_velocityComponent->velocity.x < 0)
    m_animatorComponent->PlayAnimation("walk_left");
  else
    m_animatorComponent->PlayAnimation("stand");
}

void PlayerBehaviorComponent::SetVelocityOffset(Vector2 offset) {
  m_offsetVelocity = offset;
}

TestBehaviorComponent::TestBehaviorComponent(GameObject *parentObject)
    : m_parentObject(parentObject) {
  if (!m_parentObject->HasComponent<TransformComponent>())
    logging::logerror(
        "[TestBehaviorComponent] Parent Object has no Transform component");
  if (!m_parentObject->HasComponent<TextureComponent>())
    logging::logerror(
        "[TestBehaviorComponent] Parent Object has no Texture component");
  m_startpos =
      m_parentObject->GetComponent<TransformComponent>()->GetPosition();
}

void TestBehaviorComponent::Update(float deltatime) {
  Vector2 maxRight = m_startpos + Vector2(100.0f, 0.0f);
  Vector2 pos =
      m_parentObject->GetComponent<TransformComponent>()->GetPosition();

  if (pos.x > maxRight.x)
    m_reverse = true;
  else if (pos.x < m_startpos.x)
    m_reverse = false;
  if (m_reverse)
    pos.x -= deltatime * 200.0f;
  else
    pos.x += deltatime * 200.0f;
  m_parentObject->GetComponent<TransformComponent>()->SetPosition(pos);
}

void TestBehaviorComponent::Render() {}
