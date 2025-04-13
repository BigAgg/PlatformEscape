#include "Player.h"
#include <raylib.h>
#include <raymath.h>

static const int s_playerWidth = 16;
static const int s_playerHeight = 16;
static const float s_speed = 200.0f;

Player::Player(int health, Vector2 position) : m_health(health), m_position(position) {
	m_alive = true;
	m_grounded = false;
	m_direction = { 0.0f, 0.0f };
}

void Player::Input() {
	m_direction = { 0.0f, 0.0f };
	if (IsKeyDown(KEY_W)) {
		m_direction.y -= 1;
	}
	if (IsKeyDown(KEY_S)) {
		m_direction.y += 1;
	}
	if (IsKeyDown(KEY_A)) {
		m_direction.x -= 1;
	}
	if (IsKeyDown(KEY_D)) {
		m_direction.x += 1;
	}
}

void Player::Update(float deltatime){
	m_direction *= m_directionOffset;
	m_direction = Vector2Normalize(m_direction);
	m_position += (m_direction * s_speed * deltatime);
}

void Player::Render(){
	DrawRectangleV({ m_position.x - s_playerWidth / 2, m_position.y - s_playerHeight / 2 }, { s_playerWidth, s_playerHeight }, RED);
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
	m_position = position;
}

Vector2 Player::GetPosition(){
	return m_position;
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
