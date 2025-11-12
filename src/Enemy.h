#pragma once
#include "gameObject.h"
#include "Path.h"
#include <vector>

class Enemy : public GameObject {
public:
	Enemy(std::vector<COBJModel*> objModels, int enemyType) : GameObject(objModels[0])
	{
		setUpEnemyStats(1.0f); // Leer dificultad de Player.h y actualizar

		for (auto it = ++objModels.begin(); it != objModels.end(); ++it){
			GameObject* newBodyPart = new GameObject(objModels[0]);
			m_bodyParts.push_back(newBodyPart);
		}
	}
	~Enemy() 
	{
		for (GameObject* obj : m_bodyParts)
			delete obj;
	}

	void setUpEnemyStats(float difficulty);
	void animate();
	void move(float deltaTime);
	void reachPathEnd();
	void takeDamage(float damage) { m_health -= m_damage; if (m_health <= 0) die(); }
	void die();

private:
	int m_damage, m_reward, m_type, m_weight;
	float m_health, m_defSpeed, m_speed, baseHealth = 10.0f, baseSpeed = 5.0f, m_offset = 0.0f, m_maxOffset = 3.0f;
	glm::vec2 m_dir, m_bisector, m_targetPos;
	
	std::vector<GameObject*> m_bodyParts;

	Path* m_target;
};
