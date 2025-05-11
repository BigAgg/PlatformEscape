#include "Player.h"
#include <raylib.h>
#include <raymath.h>

static const int s_playerWidth = 16;
static const int s_playerHeight = 16;
static const int s_gravity = 800.0f;
static const float s_speed = 200.0f;

Player::Player(int health, Vector2 position) : m_health(health), m_position(position) {
	m_alive = true;
	m_grounded = false;
	m_direction = { 0.0f, 0.0f };
	m_hitbox.x = position.x;
	m_hitbox.y = position.y;
	m_hitbox.width = s_playerWidth;
	m_hitbox.height = s_playerHeight;
}

void Player::Input() {
	m_direction.x = 0.0f;	// Reset the x movement
	// Checking for player inputs
	if (IsKeyPressed(KEY_SPACE) && !IsGrounded()) {
		m_direction.y = -400.0f;
		Ground(false);
	}
	if (IsKeyDown(KEY_A)) {
		m_direction.x -= 1;
	}
	if (IsKeyDown(KEY_D)) {
		m_direction.x += 1;
	}
}

void Player::Update(float deltatime){
	// Updateing the direction
	m_direction.y += deltatime * s_gravity;
	if (m_direction.y > 800)
		m_direction.y = 800;

	Vector2 computedDirection = m_direction;
	computedDirection *= m_directionOffset;

	m_hitbox.x += computedDirection.x * s_speed * deltatime;
	if (!IsGrounded())
		m_hitbox.y += computedDirection.y * deltatime;

}

void Player::Render(){
	DrawRectangleRec(m_hitbox, RED);
}

void Player::Ground(bool grounded){
	m_grounded = grounded;
}

bool Player::IsGrounded()
{
	return false;
}

void Player::Kill(){
	m_alive = false;
	m_health = 0;
}

bool Player::Alive(){
	return m_alive;
}

void Player::Damage(int amount){
	m_health -= amount;
}

int Player::Health(){
	return m_health;
}

void Player::SetPosition(Vector2 position){
	m_hitbox.x = position.x;
	m_hitbox.y = position.y;
}

Vector2 Player::GetPosition(){
	return Vector2(m_hitbox.x, m_hitbox.y);
}

void Player::SetDirection(Vector2 direction){
	m_direction = direction;
}

Vector2 Player::GetDirection(){
	return m_direction;
}

void Player::SetDirectionOffset(Vector2 offset) {
	m_directionOffset = offset;
}
