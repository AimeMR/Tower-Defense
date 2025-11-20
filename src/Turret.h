#pragma once
#include "Enemy.h"
#include <vector>

#define METRALLADORA 0
#define CONGELADORA 1
#define LASER 2
#define VERI 3
#define FRANCOTIRADORA 4

#define NTURRETS 9

class Turret : public GameObject {
public:
	Turret() : GameObject(nullptr) {
		m_type = -1;
	}

	~Turret() {
		for (GameObject* obj : m_turretParts)
			delete obj;
	}

	void draw(GLuint shader);
	void loadTurret(int type);
	void mainUpdate(float deltaTime);
	int getType() { return m_type; }

private:
	void updateLaser(float deltaTime);
	void updateCannon(float deltaTime);
	void updateIce(float deltaTime);

	Enemy* selectTarget();
	std::vector<Enemy*> selectAllTargetsInRange();

	int m_type = -1;
	float m_damage = 0, m_range = 0, m_defCD = 0, m_cd = 0;

	std::vector<GameObject*> m_turretParts;
	std::vector<Enemy*>* enemies;
};