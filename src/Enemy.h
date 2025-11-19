#pragma once
#include "gameObject.h"
#include "Path.h"
#include <vector>

#define Basic 0
#define Rapid 1
#define Tanc 2
#define Volador 3
#define Accelerador 4
#define Divisible 5
#define AcceleradorACT 6
#define DivisibleDIV 7


class Enemy : public GameObject {
public:
	Enemy(std::vector<COBJModel*> objModels, int enemyType) : GameObject(objModels[0])
	{
		m_type = enemyType;
		setUpEnemyStats(1.0f); // Leer dificultad de Player.h y actualizar

		for (auto it = ++objModels.begin(); it != objModels.end(); ++it) {
			GameObject* newBodyPart = new GameObject(*it);
			newBodyPart->setParent(this);
			m_bodyParts.push_back(newBodyPart);
		}
	}

	~Enemy()
	{
		for (GameObject* obj : m_bodyParts)
			delete obj;
	}

	void setUpEnemyStats(float difficulty);
	void setTarget(Path* path) { m_target = path; }
	void setStartPoint(glm::vec2 startPoint);
	void animate(float timer);
	void move(float deltaTime, float timer);
	void reachPathEnd();
	void takeDamage(float damage) { m_health -= m_damage; if (m_health <= 0) die(); }
	void die();
	void draw(GLuint shader);
	void startMoving();

private:
	int m_damage, m_reward, m_type, m_weight;
	float m_health, m_defSpeed, m_speed, baseHealth = 10.0f, baseSpeed = 5.0f, m_offset = 0.0f, m_maxOffset = 0.5f, m_rotation = 0.0f;
	glm::vec2 m_dir, m_bisector, m_targetPos, m_prevTargetPos;
	bool alive = true;

	std::vector<GameObject*> m_bodyParts;

	Path* m_target;
};
