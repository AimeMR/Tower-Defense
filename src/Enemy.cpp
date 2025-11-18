#include "Enemy.h"

void Enemy::setUpEnemyStats(float difficulty)
{
	switch (m_type) {
	case Basic:
		m_health = baseHealth * 0.6f;
		m_defSpeed = baseSpeed * 0.6f;
		m_damage = 1;
		m_weight = 1;
		break;
	case Rapid:
		m_health = baseHealth * 0.3f;
		m_defSpeed = baseSpeed * 1.0f;
		m_damage = 1;
		m_weight = 2;
		break;
	case Tanc:
		m_health = baseHealth * 1.0f;
		m_defSpeed = baseSpeed * 0.4f;
		m_damage = 2;
		m_weight = 3;
		break;
	case Volador:
		m_health = baseHealth * 0.4f;
		m_defSpeed = baseSpeed * 0.5f;
		m_damage = 2;
		m_weight = 3;
		break;
	case Accelerador:
		m_health = baseHealth * 0.3f;
		m_defSpeed = baseSpeed * 0.4f;
		m_damage = 2;
		m_weight = 3;
		break;
	case Divisible:
		m_health = baseHealth * 0.5f;
		m_defSpeed = baseSpeed * 0.4f;
		m_damage = 2;
		m_weight = 4;
		break;
	//Per donar l'efecte de transició en el l'enemic 5, l'eliminem i creem un nou. L'espai 6 està ocupat per identificar enemics 4 accelerats.
	case DivisibleDIV: //Enemic 5 dividit
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
	if (!alive || !m_target) return;

	animate(timer);
		
	m_pos += glm::vec3(m_dir.x, m_dir.y, 0) * m_speed * deltaTime;
	
	//Rotar

	glm::vec2 relPos = glm::vec2(m_pos) - m_targetPos;

	if (glm::dot(relPos, m_bisector) >= 0.0f)
		reachPathEnd();
}

void Enemy::startMoving() 
{
	glm::vec2 delta = m_target->getPos() - glm::vec2(m_pos);

	if (glm::length(delta) < 0.001f)  // si estás EXACTAMENTE en el nodo
	{
		// avanzar al siguiente nodo automáticamente
		m_target = m_target->getNextPath();
		if (!m_target) return; // fin del camino

		delta = m_target->getPos() - glm::vec2(m_pos);
	}

	m_dir = glm::normalize(delta);
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
		//Eliminar enemigo
		alive = false;
		return;
	}

	startMoving();
}

void Enemy::animate(float timer) 
{
	//De moment farem una animació per cada enemic, però podem crear variants pels líquids o terres trencats
	switch (m_type) {
	case 1:
		m_bodyParts[0]->setParent(m_parentMatrix);
		m_bodyParts[0]->translate(glm::vec3(m_pos.x, m_pos.y, m_pos.z));

		// 3. Crear y aplicar la matriz de rotación
		glm::mat4 matriz_rotacion = glm::rotate(
			glm::mat4(1.0f),            // Matriz inicial (identidad)
			5.0f*timer,                     // El ángulo de rotación (velocidad * timer)
			glm::vec3(1.0f, 0.0f, 0.0f) // El eje de rotación (Eje Z)
		);

		m_bodyParts[0]->rotate(matriz_rotacion); // Aplicar la rotación

	default:
		break;
	}
}

void Enemy::die() 
{
	switch (m_type) {
	case Basic: case Rapid: case Tanc: case Volador: case AcceleradorACT: case DivisibleDIV:
		//Pagar al jugador reward
		//Explota
		delete this;
		break;
	case Accelerador:
		//No paguem
		m_health = baseHealth * 0.3f;
		m_defSpeed = baseSpeed * 0.9f;
		m_damage = 1;
		m_type = 6;
		break;
	case Divisible:
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
