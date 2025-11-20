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
		if(m!=nullptr) 
			delete m;

}

void modelManager::initialSetup()
{
	//Setup Mapa
	m_modelMapa = loadModel("Mapas\\MAPAFINAL.obj");

	//Setup Enemics, modelos en intérvalos [x, y] incluídos
	
	/////////////////////////////////////////////////////////////////// Basic [0,6] ///////////////////////////////////////////////////////////////////

	m_modelsEnemics.push_back(loadModel("Enemigos\\BasicoINMOVIL.obj")); //Cuerpo
	COBJModel* ruedaDer = loadModel("Enemigos\\BasicoMOVIL.obj");
	COBJModel* ruedaIzq = loadModel("Enemigos\\BasicoMOVIL2.obj");

	for (int i = 0; i < 3;i++) {
		m_modelsEnemics.push_back(ruedaDer); //Rueda
	}
	for (int i = 0; i < 3; i++) {
		m_modelsEnemics.push_back(ruedaIzq); //Rueda
	}
	/////////////////////////////////////////////////////////////////// Rapid [7,9] ///////////////////////////////////////////////////////////////////
	COBJModel* ruedaTipo2 = loadModel("Enemigos\\RuedaTIPO2.obj");

	m_modelsEnemics.push_back(loadModel("Enemigos\\RapidoINMOVIL.obj")); //Cuerpo
	for (int i = 0; i < 2; i++) {
		m_modelsEnemics.push_back(ruedaTipo2); //Rueda
	}

	/////////////////////////////////////////////////////////////////// Tanc [10,14] ///////////////////////////////////////////////////////////////////
	m_modelsEnemics.push_back(loadModel("Enemigos\\TanqueINMOVIL.obj")); //Cuerpo
	for (int i = 0; i < 2; i++) {
		m_modelsEnemics.push_back(ruedaDer); //Rueda
	}
	for (int i = 0; i < 2; i++) {
		m_modelsEnemics.push_back(ruedaIzq); //Rueda
	}

	/////////////////////////////////////////////////////////////////// Volador [15,19]  ///////////////////////////////////////////////////////////////////
	COBJModel* helize = loadModel("Enemigos\\VoladorINMOVIL.obj");
	m_modelsEnemics.push_back(helize); //Cuerpo
	for (int i = 0; i < 4; i++) {
		m_modelsEnemics.push_back(helize); //Rueda
	}

	/////////////////////////////////////////////////////////////////// Accelerador/corredor [20,21] ///////////////////////////////////////////////////////////////////
	m_modelsEnemics.push_back(loadModel("Enemigos\\CorredorINMOVIL.obj")); //Cuerpo
	m_modelsEnemics.push_back(loadModel("Enemigos\\CorredorMOVIL.obj")); //Rueda

	/////////////////////////////////////////////////////////////////// Divisible [22,26] ///////////////////////////////////////////////////////////////////
	m_modelsEnemics.push_back(loadModel("Enemigos\\Divisible.obj")); //Cuerpo
	for (int i = 0; i < 4; i++) {
		m_modelsEnemics.push_back(ruedaTipo2); //Rueda
	}
	/////////////////////////////////////////////////////////////////// Dividid [27,29] ///////////////////////////////////////////////////////////////////
	m_modelsEnemics.push_back(loadModel("Enemigos\\Dividid.obj")); //Cuerpo
	for (int i = 0; i < 2; i++) {
		m_modelsEnemics.push_back(ruedaTipo2); //Rueda
	}
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
		return getModelRange(m_modelsEnemics, 7, 9);  // Carga las piezas 0... hasta la 1
	case Tanc:
		return getModelRange(m_modelsEnemics, 10, 14);  // Carga las piezas 0... hasta la 1
	case Volador:
		return getModelRange(m_modelsEnemics, 15, 19);  // Carga las piezas 0... hasta la 1
	case Accelerador:
		return getModelRange(m_modelsEnemics, 20,21);  // Carga las piezas 0... hasta la 1
	case Divisible:
		return getModelRange(m_modelsEnemics, 22, 26);  // Carga las piezas 0... hasta la 1
	case DivisibleDIV:
		return getModelRange(m_modelsEnemics, 27, 29);  // Carga las piezas 0... hasta la 1
	default:
		break;
	}
}