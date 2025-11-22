#include "modelManager.h"
#include "Enemy.h"
#define Basic 0
#define Rapid 1
#define Tanc 2
#define Volador 3
#define Accelerador 4
#define Divisible 5
#define AcceleradorACT 6
#define DivisibleDIV 7

#define METRALLADORA 0
#define CONGELADORA 1
#define LASER 2
#define VERI 3
#define FRANCOTIRADORA 4



modelManager::modelManager()
{

}

modelManager::~modelManager()
{
	delete m_modelMapa;
	for (COBJModel* m : m_modelsEnemics)
		delete m;
	for (COBJModel* m : m_modelsTorres)
		delete m;
}

void modelManager::initialSetup()
{
	//Setup Mapa
	m_modelMapa = loadModel("Mapas\\MAPAFINAL.obj");

	//Setup Enemics, modelos en intérvalos [x, y] incluídos
	//Enemic1 [0,6]
	m_modelsEnemics.push_back(loadModel("Enemigos\\BasicoINMOVIL.obj")); //Cuerpo
	for (int i = 0; i < 3;i++) {
		m_modelsEnemics.push_back(loadModel("Enemigos\\BasicoMOVIL.obj")); //Rueda
	}
	for (int i = 0; i < 3; i++) {
		m_modelsEnemics.push_back(loadModel("Enemigos\\BasicoMOVIL2.obj")); //Rueda
	}
	

	//Enemic2, [7,8]
	m_modelsEnemics.push_back(loadModel("Enemigos\\CorredorINMOVIL.obj")); //Cuerpo
	m_modelsEnemics.push_back(loadModel("Enemigos\\CorredorMOVIL.obj")); //Rueda

	//Setup Torretas [pares base, impares cabeza]
	//Metralleta
	m_modelsTorres.push_back(loadModel("Torretas\\turret3_base.obj"));
	m_modelsTorres.push_back(loadModel("Torretas\\turret3_head.obj"));

	//Hielo
	m_modelsTorres.push_back(loadModel("Torretas\\tesla2.obj"));
	m_modelsTorres.push_back(loadModel("Torretas\\empty.obj"));

	//Laser
	m_modelsTorres.push_back(loadModel("Torretas\\laser2_base.obj"));
	m_modelsTorres.push_back(loadModel("Torretas\\laser2_head.obj"));

	//Toxica
	m_modelsTorres.push_back(loadModel("Torretas\\toxic2_base.obj"));
	m_modelsTorres.push_back(loadModel("Torretas\\toxic2_head.obj"));

	//Francotirador
	m_modelsTorres.push_back(loadModel("Torretas\\sniper3_base.obj"));
	m_modelsTorres.push_back(loadModel("Torretas\\sniper3_head.obj"));

	//A partir de aquí se añaden todos modelos auxiliares extras como balas, efectos, etc...
	m_modelsTorres.push_back(loadModel("Torretas\\bala.obj")); //Ejemplo de bala
	m_modelsTorres.push_back(loadModel("Torretas\\tesla_bala.obj")); //Ejemplo de bala

}

COBJModel* modelManager::loadModel(const std::string& filename)
{
	std::string fullPath = ".\\modelos\\" + filename;

	COBJModel* newModel = new COBJModel();
	newModel->LoadModel(fullPath.c_str());
	return newModel;
}

std::vector<COBJModel*> modelManager::getModelRange(const std::vector<COBJModel*>& src, int start, int end)
{
	//Control OOB
	if (src.empty() || start < 0 || end < start || start >= src.size())
		return {};

	//Limitar end
	if (end >= src.size())
		end = src.size() - 1;

	return std::vector<COBJModel*>(src.begin() + start, src.begin() + end + 1);
}

std::vector<COBJModel*> modelManager::getEnemy(int type)
{
	switch (type) {
	case Basic:
		return getModelRange(m_modelsEnemics, 0, 6);  // Carga las piezas 0... hasta la 1
	case Rapid:
		return getModelRange(m_modelsEnemics, 0, 1);  // Carga las piezas 0... hasta la 1
	case Tanc:
		break;
	case Volador:
		break;
	case Accelerador:
		return getModelRange(m_modelsEnemics, 7,8);  // Carga las piezas 0... hasta la 1
	case Divisible:
		break;
	case AcceleradorACT:
		break;
	case DivisibleDIV:
		break;
	default:
		break;
	}
}

std::vector<COBJModel*> modelManager::getTurret(int type)
{
	std::vector<COBJModel*> models;
	if (type < 0 || type > 4 ) return models;
	
	models = getModelRange(m_modelsTorres, type * 2, type * 2 + 1);
	
	//Espai per afegir a models els models auxiliars com bales o efectes
	switch (type) {
	case METRALLADORA:
		models.push_back(m_modelsTorres[10]); //Carreguem la bala com a auxiliar de la metralladora
		break;
	case CONGELADORA:
		models.push_back(m_modelsTorres[11]); //Carreguem la bala com a auxiliar de la metralladora
		break;

	case LASER:

		break;
	case VERI:

		break;
	case FRANCOTIRADORA:
		models.push_back(m_modelsTorres[10]); //Carreguem la bala com a auxiliar de la metralladora
		break;
	default:
		return models;
	}

	return models;
}

