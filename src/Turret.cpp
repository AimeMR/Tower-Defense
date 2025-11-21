#include "Turret.h"

Enemy* Turret::selectTarget() 
{
	Enemy* target = nullptr;
	float maxProg = 0;

	for (Enemy* e : *enemies) 
	{
		glm::vec3 epos = e->getPos();
		float len = glm::distance(glm::vec2(epos.x, epos.y), m_pos);
		
		if (len > m_range) continue;
		
		float prog = e->getProgress();
		if (prog > maxProg) 
		{
			maxProg = prog;
			target = e;
		}
	}

	return target;
}

std::vector<Enemy*> Turret::selectAllTargetsInRange()
{
	std::vector<Enemy*> targets;

	for (Enemy* e : *enemies)
	{
		glm::vec3 epos = e->getPos();
		float len = glm::distance(glm::vec2(epos.x, epos.y), m_pos);

		if (len <= m_range)
			targets.push_back(e);
	}

	return targets;
}

void Turret::draw(GLuint shader)
{
	if (m_type == -1) return;
	m_baseObj->dibuixarObjecte(shader);
	m_headObj->dibuixarObjecte(shader);
	if (m_auxObject) m_auxObject->dibuixarObjecte(shader);
}

void Turret::loadTurret(int type, std::vector<COBJModel*> models)
{
	m_type = type;

	if (m_baseObj != nullptr) delete m_baseObj;
	if (m_headObj != nullptr) delete m_headObj;
	m_auxModels.clear();

	if (type == -1) 
	{
		m_damage = 0; 
		m_range = 0; 
		m_defCD = 0; 
		m_cd = 0;
		m_baseObj = nullptr;
		m_headObj = nullptr;
		return;
	}

	m_baseObj = new GameObject(models[0]);
	m_headObj = new GameObject(models[1]);

	for (int i = 2; i < models.size(); i++)
		m_auxModels.push_back(models[i]);

	m_baseObj->setPOID(m_id);
	m_headObj->setPOID(m_id);

	float zBase = 0.0f;
	float zHead = 0.0f;

	switch (type) {
	case METRALLADORA:
		//Ajustar stats i z
		m_damage = 0.5f;
		m_defCD = 0.5f;
		m_range = 8;
		zBase = -0.05f;
		zHead = 0.8f;
		break;
	case CONGELADORA:
		m_damage = 1.0f;
		m_defCD = 3.0f;
		m_range = 5;
		zBase = -0.05f;
		break;
	case LASER:
		m_damage = 0.65f;
		m_defCD = 0.0f;
		m_range = 5;
		zBase = -0.05f;
		zHead = 0.87f;
		break;
	case VERI:
		m_damage = 0.2f;
		m_defCD = 1.0f;
		m_range = 6;
		zBase = -0.05f;
		zHead = 0.75f;
		break;
	case FRANCOTIRADORA:
		m_damage = 2.5f;
		m_defCD = 2.5f;
		m_range = 12;
		zHead = 2.42f;
		break;
	default:
		return;
	}
	
	m_cd = m_defCD;
	m_baseObj->translate(glm::vec3(m_pos, zBase));
	m_headObj->translate(glm::vec3(m_pos, zHead));
}

void Turret::mainUpdate(float deltaTime)
{
	if (m_type < 0 || m_type > 4) return;

	animate(deltaTime);

	if (m_type == LASER) updateLaser(deltaTime);
	else if (m_type == CONGELADORA) updateIce(deltaTime);
	else updateCannon(deltaTime);
}

void Turret::updateLaser(float deltaTime)
{
	Enemy* target = selectTarget();
	if (target)
	{
		TurnHead(deltaTime, target->getPos());
		float laserDMG = max(target->getHealth() * 0.5f, m_damage * 2);
		target->takeDamage(laserDMG * deltaTime * m_damage);
		return;
	}
}

void Turret::updateCannon(float deltaTime) 
{
	Enemy* target = selectTarget();
	if (!target) 
	{
		m_cd = m_defCD;
		return;
	}
	else 
	{
		TurnHead(deltaTime, target->getPos());
		m_cd -= deltaTime;
		if (m_cd <= 0) 
		{
			shootAnimation();
			auxVec3 = target->getPos();

			if(m_type == VERI) target->setPoison(1.5f + m_damage * 2.0f);

			target->takeDamage(m_damage);
			m_cd = m_defCD;
		}
	}
}

void Turret::updateIce(float deltaTime)
{
	m_cd -= deltaTime;
	if (m_cd <= 0) 
	{
		m_cd = m_defCD;
		std::vector<Enemy*> affectedEnemies = selectAllTargetsInRange();
		
		for (Enemy* e : affectedEnemies) 
		{
			//Attack effect
			//Slow enemies
			e->takeDamage(m_damage);
			e->setSlow(2.0f + m_damage);
		}
	}
}

void Turret::TurnHead(float deltaTime, glm::vec3 ePos)
{
	glm::vec3 headPos = m_headObj->getPos();
	glm::mat4 view = glm::lookAt(headPos, ePos, glm::vec3(0, 0, 1));
	
	glm::mat4 modelRot = glm::inverse(view);

	glm::quat targetRot = glm::quat_cast(modelRot);

	glm::quat fixXForward = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0));
	glm::quat fixSideOffset = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1));

	targetRot = targetRot * fixSideOffset * fixXForward;

	glm::quat currentRot = glm::quat_cast(m_headObj->getRot());
	glm::quat smoothRot = glm::slerp(currentRot, targetRot, 0.15f);

	m_headObj->setRotation(glm::mat4_cast(smoothRot));
}

void Turret::spawnAuxObj(int id) 
{
	if (m_auxObject) return;

	m_auxObject = new GameObject(m_auxModels[id]);
}

void Turret::deleteAuxObj() 
{
	if (m_auxObject == nullptr) return;

	delete m_auxObject;
	m_auxObject = nullptr;
}

//mm = modelManager.cpp
//El concepto de las animaciones es el siguiente: Al disparar se ejecuta shootAnimation() y animate se va ejecutando todo el rato.
//Por ejemplo, para disparar una bala la tienes que haber cargado en el mm, tanto el modelo al final de la carga como añadida al switch del getTurret que quieras.
//En shootAnimation() llamas a spawnAuxObj(el num de modelo auxiliar) para crear la bala, y pones su m_pos en la punta del cañón con translate(), su tamaño con scale()... 

//Desde animate() modificas su posición/rotación/escala lo que quieras mientras no se cumpla cierta condición. Allí animas también la própia torreta.
//Cuando pase la condición límite llamas a deleteAuxObj(objeto) o lo que sea.

//Usa los espacios de variables auxiliares de torre para guardar variables, y crea los que necesites.

void Turret::shootAnimation() 
{
	glm::vec3 headPos = m_headObj->getPos();
	glm::vec3 worldBackward = glm::vec3(m_headObj->getRot() * glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
	glm::vec3 recoilOffset = worldBackward * 0.25f;
	switch (m_type) {
	case METRALLADORA:
		auxBool = true; // Control de retroceso

		m_headObj->translate(headPos + recoilOffset);

		spawnAuxObj(0);
		m_auxObject->translate(glm::vec3(m_pos, 1.0f) - worldBackward * 1.45f);
		m_auxObject->scale(glm::vec3(0.1f, 0.1f, 0.1f));

		return;
	case CONGELADORA:
		//Podem fer servir una circumferència blava al terra semblant a la que farem servir per veure el radi. Que el model tingui radi 1.
		// Llavors la iniciem en escala (0.1, 0.1, 1) a dins la torre i fem que vagi creixent fins a ser del tamany m_range i que llavors desaparegui.
		//Et pots inspirar en l'animació de mort que va fer el Joan a una altra branca per fer lo de que creixi fins a X punt.

		//Et deixo un esquelet del codi entre aquesta funció i la de animate: 
		// 
		spawnAuxObj(0);
		m_auxObject->translate(glm::vec3(m_pos, 0));
		m_auxObject->scale(glm::vec3(0.1f, 0.1f, 1));
		return;
	case LASER:
		return;
	case VERI:
		return;
	case FRANCOTIRADORA:
		auxBool = true; // Control de retroceso

		m_headObj->translate(headPos + recoilOffset);

		spawnAuxObj(0);
		m_auxObject->translate(glm::vec3(m_pos, 1.0f) - worldBackward * 10.0f);
		m_auxObject->scale(glm::vec3(0.1f, 0.1f, 0.1f));

		return;

	default: 
		return;
	}
}

void Turret::animate(float deltaTime)
{
	switch (m_type) {
	case METRALLADORA:
		if (auxBool) 
		{
			m_headObj->translate(glm::lerp(m_headObj->getPos(), glm::vec3(m_pos, 0.8f), 3.0f * deltaTime));
			if (glm::length(m_headObj->getPos() - glm::vec3(m_pos, 0.8f)) < 0.001f)
			{
				m_headObj->translate(glm::vec3(m_pos, 0.8f));
				auxBool = false;
			}
		}

		if (m_auxObject) 
		{
			m_auxObject->translate(glm::lerp(m_auxObject->getPos(), auxVec3, 25.0f * deltaTime));
			if (glm::length(m_auxObject->getPos() - auxVec3) < 0.1f)
				deleteAuxObj();
		}

		return;
	case CONGELADORA:
		
		// Esto combinado con lo del shootAnimation debería funcionar
		
		if (m_auxObject == nullptr) return;
		
		m_auxObject->scale(glm::lerp(m_auxObject->getScale(), glm::vec3(m_range, m_range, 1), 2.5f * deltaTime));

		if (m_auxObject->getScale().x > m_range - 0.25f)
			deleteAuxObj();


		return;
	case LASER:
		//Hem de veure com fer un laser que vagi d'un punt a un altre, no ho tinc molt clar, investiga com fer-ho. Deixa-ho pel final.
		return;
	case VERI:

		return;
	case FRANCOTIRADORA:
		if (auxBool)
		{
			m_headObj->translate(glm::lerp(m_headObj->getPos(), glm::vec3(m_pos, 2.42f), deltaTime));
			if (glm::length(m_headObj->getPos() - glm::vec3(m_pos, 2.42f)) < 0.001f)
			{
				m_headObj->translate(glm::vec3(m_pos, 2.42f));
				auxBool = false;
			}
		}

		if (m_auxObject)
		{
			m_auxObject->translate(glm::lerp(m_auxObject->getPos(), auxVec3, 25.0f * deltaTime));
			if (glm::length(m_auxObject->getPos() - auxVec3) < 0.1f)
				deleteAuxObj();
		}

		return;
	default:
		return;
	}
}
