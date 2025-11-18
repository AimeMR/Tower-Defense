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



modelManager::modelManager()
{

}

modelManager::~modelManager()
{
	delete m_modelMapa;
	for (COBJModel* m : m_modelsEnemics)
		delete m;
}

void modelManager::initialSetup()
{
	//Setup Mapa
	m_modelMapa = loadModel("Mapas\\MAPAFINAL.obj");

	//Setup Enemics, modelos en intérvalos [x, y] incluídos
	//Enemic1

	//Enemic2, [0,1]
	m_modelsEnemics.push_back(loadModel("Enemigos\\CorredorINMOVIL4.obj")); //Cuerpo
	m_modelsEnemics.push_back(loadModel("Enemigos\\CorredorMOVIL4.obj")); //Rueda
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
		return getModelRange(m_modelsEnemics, 0, 1);  // Carga las piezas 0... hasta la 1
	case Rapid:
		return getModelRange(m_modelsEnemics, 0, 1);  // Carga las piezas 0... hasta la 1
	case Tanc:
		break;
	case Volador:
		break;
	case Accelerador:
		return getModelRange(m_modelsEnemics, 0, 1);  // Carga las piezas 0... hasta la 1
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