#pragma once

#include <raylib.h>

class Player {
public:
	Player(int health, Vector2 position);
	
	void Input();
	void Update(float deltatime);
	void Render();

	void Ground(bool grounded);
	bool IsGrounded();

	void Kill();
	bool Alive();

	void Damage(int amount);
	int Health();

	void SetPosition(Vector2 position);
	Vector2 GetPosition();

	void SetDirection(Vector2 direction);
	void SetDirectionOffset(Vector2 offset);
	Vector2 GetDirection();

private:
	bool m_alive;
	bool m_grounded;
	int m_health;
	Vector2 m_direction;
	Vector2 m_directionOffset = { 1.0f, 1.0f };
	Vector2 m_position;
	Rectangle m_hitbox;
};
