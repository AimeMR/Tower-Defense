#pragma once
#include "material.h"
#include "objLoader.h"
#include "player.h"
#include <vector>

class modelManager
{
public:
	modelManager();
	~modelManager();
	void initialSetup();
	COBJModel* loadModel(const std::string& filename);
	std::vector<COBJModel*> getModelRange(const std::vector<COBJModel*>& src, int start, int end);
	COBJModel* getMapa() { return m_modelMapa; }
	std::vector<COBJModel*> getEnemy(int type);
	std::vector<COBJModel*> getTurret(int type);
	COBJModel* getFloor() { return m_turretFloor; }
	COBJModel* getRadius() { return m_turretRadius; }

private:
	COBJModel* m_modelMapa;
	COBJModel* m_turretFloor;
	COBJModel* m_turretRadius;
	std::vector<COBJModel*> m_modelsEnemics;
	std::vector<COBJModel*> m_modelsTorres;
};

