#include "Enemy.h"

void Enemy::setUpEnemyStats(float difficulty)
{
	switch (m_type) {
	case 0:
		m_health = baseHealth * 0.6f;
		m_defSpeed = baseSpeed * 0.6f;
		m_damage = 1;
		m_weight = 1;
		break;
	case 1:
		m_health = baseHealth * 0.3f;
		m_defSpeed = baseSpeed * 1.0f;
		m_damage = 1;
		m_weight = 2;
		break;
	case 2:
		m_health = baseHealth * 1.0f;
		m_defSpeed = baseSpeed * 0.4f;
		m_damage = 2;
		m_weight = 3;
		break;
	case 3:
		m_health = baseHealth * 0.4f;
		m_defSpeed = baseSpeed * 0.5f;
		m_damage = 2;
		m_weight = 3;
		break;
	case 4:
		m_health = baseHealth * 0.3f;
		m_defSpeed = baseSpeed * 0.4f;
		m_damage = 2;
		m_weight = 3;
		break;
	case 5:
		m_health = baseHealth * 0.5f;
		m_defSpeed = baseSpeed * 0.4f;
		m_damage = 2;
		m_weight = 4;
		break;
	//Per donar l'efecte de transició en el l'enemic 5, l'eliminem i creem un nou. L'espai 6 està ocupat per identificar enemics 4 accelerats.
	case 7: //Enemic 5 dividit
		m_health = baseHealth * 0.3f;
		m_defSpeed = baseSpeed * 0.4f;
		m_damage = 1;
		m_weight = 2;
		break;
	default:
		die();
	}

	m_reward = (int)((float)m_weight * difficulty * 100.0f);
	m_offset = (((float)rand() / (float)RAND_MAX) * 2.0 - 1.0) * m_maxOffset;
}

void Enemy::move(float deltaTime, float timer)
{
	animate(timer);
	
	if (!m_target) return;
	
	m_pos += glm::vec3(m_dir.x, m_dir.y, 0) * m_speed * deltaTime;
	
	//Rotar

	glm::vec2 relPos = glm::vec2(m_pos) - m_targetPos;

	if (glm::dot(relPos, m_bisector) >= 0.0f)
		reachPathEnd();
}

void Enemy::startMoving() 
{
	m_dir = m_target->getNextDir();
	m_speed = m_defSpeed * m_target->getSpeedMultiplier();
	m_bisector = m_target->getBisector();
	m_targetPos = m_target->getPos();
}

void Enemy::reachPathEnd() 
{
	m_target = m_target->getNextPath();

	if (m_target == nullptr) 
	{
		//Restar vida al jugador
		//Explota
		delete this;
	}

	startMoving();
}

void Enemy::animate(float timer) 
{
	//De moment farem una animació per cada enemic, però podem crear variants pels líquids o terres trencats
	switch (m_type) {
	case 1:
		m_bodyParts[0]->setParent(m_parentMatrix);
		m_bodyParts[0]->translate(glm::vec3(m_pos.x + sin(timer * 5) * 0.3f, m_pos.y, m_pos.z));

	default:
		break;
	}
}

void Enemy::die() 
{
	switch (m_type) {
	case 0: case 1: case 2: case 3: case 6: case 7:
		//Pagar al jugador reward
		//Explota
		delete this;
		break;
	case 4:
		//No paguem
		m_health = baseHealth * 0.3f;
		m_defSpeed = baseSpeed * 0.9f;
		m_damage = 1;
		m_type = 6;
		break;
	case 5:
		//No paguem
		//Explota
		//Creem 2 enemics 7 a la seva posició actual amb la seva direcció actual
		delete this;
		break;
	default:
		delete this;
		break;
	}
}

void Enemy::draw(GLuint shader)
{
	this->dibuixarObjecte(shader);
	for (GameObject* g : m_bodyParts)
		g->dibuixarObjecte(shader);
}
