#include "modelManager.h"

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
	m_modelMapa = loadModel("Mapas\\MAPAV2.obj");

	//Setup Enemics, modelos en intérvalos [x, y] incluídos
	//Enemic1

	//Enemic2, [0,1]
	m_modelsEnemics.push_back(loadModel("Enemigos\\CorredorINMOVIL.obj")); //Cuerpo
	m_modelsEnemics.push_back(loadModel("Enemigos\\CorredorMOVIL.obj")); //Rueda
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
	case 0:
		break;
	case 1:
		return getModelRange(m_modelsEnemics, 0, 1);
	default:
		break;
	}
}