#pragma once
#include "Enemy.h"
#include <vector>

#define METRALLADORA 0
#define CONGELADORA 1
#define LASER 2
#define VERI 3
#define FRANCOTIRADORA 4

#define NTURRETS 9

class Turret {
public:
	Turret(int id){
		m_id = id;
		m_type = -1;
	}

	~Turret() {
		delete m_baseObj;
		delete m_headObj;
		deleteAuxObj();
	}

	void draw(GLuint shader);
	void loadTurret(int type, std::vector<COBJModel*> models);
	void mainUpdate(float deltaTime);
	int getType() { return m_type; }
	void setEnemiesVector(std::vector<Enemy*>* e) { enemies = e; }
	void setPos(glm::vec2 pos) { m_pos = pos; }

private:
	void updateLaser(float deltaTime);
	void updateCannon(float deltaTime);
	void updateIce(float deltaTime);
	void TurnHead(float deltaTime, glm::vec3 ePos);
	void shootAnimation();
	void animate(float deltatime);
	void spawnAuxObj(int id);
	void deleteAuxObj();

	Enemy* selectTarget();
	std::vector<Enemy*> selectAllTargetsInRange();

	int m_type = -1, m_id = 0;
	float m_damage = 0, m_range = 0, m_defCD = 0, m_cd = 0, m_headZ = 0;
	glm::vec2 m_pos;

	//Variables auxiliars per animacions
	glm::vec3 auxVec3 = glm::vec3(0.0f, 0.0f, 0.0f);
	bool auxBool = false;


	GameObject* m_baseObj = nullptr;
	GameObject* m_headObj = nullptr;
	GameObject* m_auxObject = nullptr;
	std::vector<COBJModel*> m_auxModels;
	std::vector<Enemy*>* enemies;
};