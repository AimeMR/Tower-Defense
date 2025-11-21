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
}

void Turret::loadTurret(int type, COBJModel* base, COBJModel* head)
{
	m_type = type;

	if (m_baseObj != nullptr) delete m_baseObj;
	if (m_headObj != nullptr) delete m_headObj;

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

	m_baseObj = new GameObject(base);
	m_headObj = new GameObject(head);

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
		zHead = 1.25f;
		break;
	case CONGELADORA:
		m_damage = 1.0f;
		m_defCD = 3.0f;
		m_range = 5;
		break;
	case LASER:
		m_damage = 0.65f;
		m_defCD = 0.0f;
		m_range = 5;
		zBase = 0.0f;
		zHead = 1.25f;
		break;
	case VERI:
		m_damage = 0.2f;
		m_defCD = 1.0f;
		m_range = 6;
		zHead = 1.25f;
		break;
	case FRANCOTIRADORA:
		m_damage = 2.5f;
		m_defCD = 2.5f;
		m_range = 12;
		zHead = 1.25f;
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
			//Cast bullet and shoot animation

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
	glm::vec3 toTarget = ePos - m_headObj->getPos();

	glm::vec3 yawDir = toTarget;
	yawDir.z = 0.0f;
	float yaw = atan2(yawDir.y, yawDir.x);
	glm::quat yawRot = glm::angleAxis(yaw, glm::vec3(0, 0, 1));

	glm::mat4 yawMat = glm::mat4_cast(yawRot);
	glm::vec3 localTarget = glm::inverse(yawMat) * glm::vec4(toTarget, 1.0f);
	float pitch = atan2(localTarget.z, localTarget.x);
	glm::quat pitchRot = glm::angleAxis(pitch, glm::vec3(1, 0, 0));

	glm::quat targetRot = yawRot * pitchRot;
	glm::quat currentRot = glm::quat_cast(m_headObj->getRot());
	glm::quat smoothRot = glm::slerp(currentRot, targetRot, 0.1f);

	m_headObj->setRotation(glm::mat4_cast(smoothRot));
}
