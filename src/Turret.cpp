#include "Turret.h"

Enemy* Turret::selectTarget() 
{
	Enemy* target = nullptr;
	glm::vec2 tpos = glm::vec2(m_pos.x, m_pos.y);
	float maxProg = 0;

	for (Enemy* e : *enemies) 
	{
		glm::vec3 epos = e->getPos();
		float len = glm::distance(glm::vec2(epos.x, epos.y), tpos);
		
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
	glm::vec2 tpos = glm::vec2(m_pos.x, m_pos.y);

	for (Enemy* e : *enemies)
	{
		glm::vec3 epos = e->getPos();
		float len = glm::distance(glm::vec2(epos.x, epos.y), tpos);

		if (len <= m_range)
			targets.push_back(e);
	}

	return targets;
}

void Turret::draw(GLuint shader)
{
	this->dibuixarObjecte(shader);
	for (GameObject* g : m_turretParts)
		g->dibuixarObjecte(shader);
}

void Turret::loadTurret(int type) 
{
	//Cargar model
	
	//Switch para cargar stats
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
		m_damage = max(target->getHealth() * 0.3f, 1.0f);
		target->takeDamage(m_damage * deltaTime);
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
		m_cd -= deltaTime;
		if (m_cd <= 0) 
		{
			//Cast bullet and shoot animation

			if(m_type == VERI) //Enverina target

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
		std::vector<Enemy*> affectedEnemies = selectAllTargetsInRange();
		for (Enemy* e : affectedEnemies) 
		{
			//Attack effect
			//Slow enemies
			e->takeDamage(m_damage);
		}
		m_cd = m_defCD;
	}
}
