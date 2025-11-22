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
	{
		if (m != nullptr)
		{
			delete m;
			m = nullptr;
		}
	}
}

void modelManager::initialSetup()
{
	//Setup Mapa
	m_modelMapa = loadModel("Mapas\\MAPAFINAL.obj");

	//Setup Enemics, modelos en intérvalos [x, y] incluídos
	
	/////////////////////////////////////////////////////////////////// Basic [0,6] ///////////////////////////////////////////////////////////////////

	m_modelsEnemics.push_back(loadModel("Enemigos\\Basico.obj")); //Cuerpo
	COBJModel* ruedaDer = loadModel("Enemigos\\RuedaDER.obj");
	COBJModel* ruedaIzq = loadModel("Enemigos\\RuedaIZQ.obj");

	for (int i = 0; i < 3;i++) {
		m_modelsEnemics.push_back(ruedaDer); //Rueda
	}
	for (int i = 0; i < 3; i++) {
		m_modelsEnemics.push_back(ruedaIzq); //Rueda
	}
	/////////////////////////////////////////////////////////////////// Rapid [7,9] ///////////////////////////////////////////////////////////////////
	COBJModel* ruedaTipo2 = loadModel("Enemigos\\Rueda.obj");

	m_modelsEnemics.push_back(loadModel("Enemigos\\Rapido.obj")); //Cuerpo
	for (int i = 0; i < 2; i++) {
		m_modelsEnemics.push_back(ruedaTipo2); //Rueda
	}

	/////////////////////////////////////////////////////////////////// Tanc [10,14] ///////////////////////////////////////////////////////////////////
	m_modelsEnemics.push_back(loadModel("Enemigos\\Tanque.obj")); //Cuerpo
	for (int i = 0; i < 2; i++) {
		m_modelsEnemics.push_back(ruedaDer); //Rueda
	}
	for (int i = 0; i < 2; i++) {
		m_modelsEnemics.push_back(ruedaIzq); //Rueda
	}

	/////////////////////////////////////////////////////////////////// Volador [15,19]  ///////////////////////////////////////////////////////////////////
	COBJModel* helize = loadModel("Enemigos\\Helice.obj");

	m_modelsEnemics.push_back(loadModel("Enemigos\\Volador.obj")); //Cuerpo
	for (int i = 0; i < 4; i++) {
		m_modelsEnemics.push_back(helize); //Rueda
	}

	/////////////////////////////////////////////////////////////////// Accelerador/corredor [20,21] ///////////////////////////////////////////////////////////////////
	COBJModel* accelerador = loadModel("Enemigos\\Accelerador.obj");
	m_modelsEnemics.push_back(accelerador); //Cuerpo
	m_modelsEnemics.push_back(loadModel("Enemigos\\AcceleradorM1.obj")); //Rueda

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
	/////////////////////////////////////////////////////////////////// AcceleradorACT [20,21] ///////////////////////////////////////////////////////////////////
	m_modelsEnemics.push_back(accelerador); //Cuerpo
	m_modelsEnemics.push_back(loadModel("Enemigos\\AcceleradorM2.obj")); //Rueda

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
		return getModelRange(m_modelsEnemics, 0, 6);  // Carga las piezas 0... hasta la 6
	case Rapid:
		return getModelRange(m_modelsEnemics, 7, 9);  
	case Tanc:
		return getModelRange(m_modelsEnemics, 10, 14);  
	case Volador:
		return getModelRange(m_modelsEnemics, 15, 19);  
	case Accelerador:
		return getModelRange(m_modelsEnemics, 20,21);  
	case Divisible:
		return getModelRange(m_modelsEnemics, 22, 26);  
	case DivisibleDIV:
		return getModelRange(m_modelsEnemics, 27, 29);  
	case AcceleradorACT:
		return getModelRange(m_modelsEnemics, 30, 31);  
	default:
		break;
	}
}