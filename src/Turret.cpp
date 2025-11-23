#include "Turret.h"

Enemy* Turret::selectTarget() 
{
	Enemy* target = nullptr;
	float maxProg = 0;

	for (Enemy* e : *enemies) 
	{
		glm::vec3 epos = e->getPos();
		float len = glm::distance(glm::vec2(epos.x, epos.y), m_pos);
		
		if (len > m_range || !e->isAlive()) continue;
		
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

		if (len <= m_range && e->isAlive())
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
		m_headZ = 0;
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

	switch (type) {
	case METRALLADORA:
		//Ajustar stats i z
		m_damage = 0.5f;
		m_defCD = 0.5f;
		m_range = 8;
		zBase = -0.05f;
		m_headZ = 0.8f;
		break;
	case CONGELADORA:
		m_damage = 1.0f;
		m_defCD = 3.0f;
		m_range = 5;
		zBase = -0.05f;
		m_headZ = 0.0f;
		break;
	case LASER:
		m_damage = 0.65f;
		m_defCD = 0.0f;
		m_range = 5;
		zBase = -0.05f;
		m_headZ = 0.87f;
		break;
	case VERI:
		m_damage = 0.2f;
		m_defCD = 1.0f;
		m_range = 6;
		zBase = -0.05f;
		m_headZ = 0.8f;
		break;
	case FRANCOTIRADORA:
		m_damage = 2.5f;
		m_defCD = 2.5f;
		m_range = 12;
		zBase = -0.15f;
		m_headZ = 1.2f;
		break;
	default:
		return;
	}
	
	m_cd = m_defCD;
	m_baseObj->translate(glm::vec3(m_pos, zBase));
	m_headObj->translate(glm::vec3(m_pos, m_headZ));
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
		if (auxBool) 
		{
			auxVec3 = target->getPos();
			float laserDMG = max(target->getHealth() * 0.5f, m_damage * 2);
			target->takeDamage(laserDMG * deltaTime * m_damage);
			if (!m_auxObject) 
			{
				spawnAuxObj(0);
				glm::vec3 worldFW = glm::vec3(m_headObj->getRot() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
				glm::quat rot = glm::rotation(glm::vec3(0, 0, 1), worldFW);
				m_auxObject->setRotation(glm::mat4_cast(rot));

				glm::vec3 cannonTip = glm::vec3(m_pos, m_headZ + 0.6f) + worldFW;
				glm::vec3 dir = auxVec3 - cannonTip;
				m_auxObject->translate(cannonTip + dir / 2.0f);

				m_auxObject->scale(glm::vec3(0.25f, 0.25f, glm::length(dir) / 2.0f));
			}
		}
		else
		{
			if (m_auxObject) deleteAuxObj();
		}
		
		return;
	}
	else if (m_auxObject) deleteAuxObj();
}

void Turret::updateCannon(float deltaTime) 
{
	Enemy* target = selectTarget();
	if (!target && m_cd < m_defCD) 
	{
		m_cd += deltaTime;
	}
	else if(target)
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
			shootAnimation();
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
	glm::quat smoothRot = glm::slerp(currentRot, targetRot, 0.2f);

	if (m_type == LASER) auxBool = abs(glm::dot(smoothRot, targetRot)) > 0.999f;

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

void Turret::shootAnimation() 
{
	glm::vec3 headPos;
	glm::vec3 worldBackward;
	glm::vec3 recoilOffset;
	switch (m_type) {
	case METRALLADORA:
		auxBool = true;
		headPos = glm::vec3(m_pos, m_headZ);
		worldBackward = glm::vec3(m_headObj->getRot() * glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
		recoilOffset = worldBackward * 0.25f;

		m_headObj->translate(headPos + recoilOffset);

		spawnAuxObj(0);
		m_auxObject->translate(glm::vec3(m_pos, 1.0f) - worldBackward * 1.45f);
		m_auxObject->scale(glm::vec3(0.1f, 0.1f, 0.1f));

		break;
	case CONGELADORA:
		spawnAuxObj(0);
		m_auxObject->translate(glm::vec3(m_pos, 0));
		m_auxObject->scale(glm::vec3(0.1f, 0.1f, 1));
		break;
	case VERI:
		auxBool = true;
		headPos = glm::vec3(m_pos, m_headZ);
		worldBackward = glm::vec3(m_headObj->getRot() * glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
		recoilOffset = worldBackward * 0.2f;

		m_headObj->translate(headPos + recoilOffset);

		spawnAuxObj(0);
		m_auxObject->translate(glm::vec3(m_pos, 1.0f) - worldBackward * 1.45f);
		m_auxObject->scale(glm::vec3(0.25f, 0.25f, 0.25f));
		break;
	case FRANCOTIRADORA:
		headPos = glm::vec3(m_pos, m_headZ);
		worldBackward = glm::vec3(m_headObj->getRot() * glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));

		spawnAuxObj(0);
		m_auxObject->translate(glm::vec3(m_pos, 1.2f) - worldBackward * 1.45f);
		m_auxObject->scale(glm::vec3(0.1f, 0.1f, 0.1f));

		break;
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
			m_headObj->translate(glm::lerp(m_headObj->getPos(), glm::vec3(m_pos, m_headZ), 3.0f * deltaTime));
			if (glm::length(m_headObj->getPos() - glm::vec3(m_pos, m_headZ)) < 0.001f)
			{
				m_headObj->translate(glm::vec3(m_pos, m_headZ));
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
				
		if (m_auxObject == nullptr) return;
		m_auxObject->scale(glm::lerp(m_auxObject->getScale(), glm::vec3(m_range, m_range, 1), 7.5f * deltaTime));

		if (m_auxObject)
		{
			if (m_auxObject->getScale().x > m_range - 0.25f)
				deleteAuxObj();
		}

		return;
	case LASER:
		if (!m_auxObject) return;
		
		glm::vec3 worldFW = glm::vec3(m_headObj->getRot() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		glm::quat rot = glm::rotation(glm::vec3(0, 0, 1), worldFW);
		m_auxObject->setRotation(glm::mat4_cast(rot));

		glm::vec3 cannonTip = glm::vec3(m_pos, m_headZ + 0.6f) + worldFW;
		glm::vec3 dir = auxVec3 - cannonTip;
		m_auxObject->translate(cannonTip + dir / 2.0f);

		m_auxObject->scale(glm::vec3(0.25f, 0.25f, glm::length(dir) / 2.0f));

		return;
	case VERI:
		if (auxBool)
		{
			m_headObj->translate(glm::lerp(m_headObj->getPos(), glm::vec3(m_pos, m_headZ), 3.0f * deltaTime));
			if (glm::length(m_headObj->getPos() - glm::vec3(m_pos, m_headZ)) < 0.001f)
			{
				m_headObj->translate(glm::vec3(m_pos, m_headZ));
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
	case FRANCOTIRADORA:

		if (m_auxObject)
		{
			m_auxObject->translate(glm::lerp(m_auxObject->getPos(), auxVec3, 35.0f * deltaTime));
			if (glm::length(m_auxObject->getPos() - auxVec3) < 0.1f)
				deleteAuxObj();
		}

		return;
	default:
		return;
	}
}
