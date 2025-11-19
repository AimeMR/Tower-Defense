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
}

void Enemy::move(float deltaTime, float timer)
{
	if (!alive || !m_target) return;
		
	m_pos += glm::vec3(m_dir.x, m_dir.y, 0) * m_speed * deltaTime;
	
	//Rotar
	float targetAngle = atan2(m_dir.y, m_dir.x) - glm::half_pi<float>();
	m_rotation = m_rotation + (fmod(targetAngle - m_rotation + PI, 2.0f * PI) - PI) * 5.0f * deltaTime;
	m_rot = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0, 0, 1));

	animate(timer, deltaTime);

	//Sistema de detección de giros temporal
	glm::vec2 diff = m_targetPos - m_prevTargetPos;
	glm::vec2 relPos = glm::vec2(m_pos) - m_prevTargetPos;
	float t = glm::dot(relPos, diff) / glm::dot(diff, diff);

	if (t >= 1.0f)
		reachPathEnd();
}

void Enemy::startMoving() 
{
	if (!m_target) return;

	m_dir = m_target->getNextDir();
	m_speed = m_defSpeed * m_target->getSpeedMultiplier();
	m_bisector = m_target->getBisector();
	m_targetPos = m_target->getPos();

	m_rotation = atan2(m_dir.y, m_dir.x) - glm::half_pi<float>();
	m_rot = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0, 0, 1));
}

void Enemy::reachPathEnd() 
{
	m_prevTargetPos = m_targetPos;
	m_target = m_target->getNextPath();

	if (m_target == nullptr) 
	{
		//Restar vida al jugador
		//Explota
		//Eliminar enemigo
		alive = false;
		return;
	}

	m_dir = m_target->getNextDir();
	m_speed = m_defSpeed * m_target->getSpeedMultiplier();
	m_bisector = m_target->getBisector();
	m_targetPos = m_target->getPos();
	int newPType = m_target->getTipus();
	if (newPType != m_pathType) 
	{
		m_pathType = newPType;
		//Esto se ejecuta solo en las transiciones entre diferentes tipos de caminos
		switch (m_type) {
		case Basic:
			translate(glm::vec3(m_pos.x, m_pos.y, 0.3));
			break;
		case Rapid:
			break;
		case Tanc:
			break;
		case Volador:
			break;
		case Accelerador:
			translate(glm::vec3(m_pos.x, m_pos.y, 0.5));
			break;
		case Divisible: case DivisibleDIV:
			break;

		default:
			break;
		}
	}


}

void Enemy::animate(float timer, float deltaTime)
{
	//De moment farem una animació per cada enemic, però podem crear variants pels líquids o terres trencats
	glm::mat4 matriz_rotacion, matriz_rotacion_180, matriz_rotacion_rueda;

	switch (m_type) {
	case Basic:
		translate(glm::vec3(m_pos.x, m_pos.y, 0.3));

		matriz_rotacion_180 = glm::rotate(glm::mat4(1.0f), radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		matriz_rotacion_rueda = glm::rotate(glm::mat4(1.0f), -0.5f * timer * m_speed * deltaTime / m_defSpeed, glm::vec3(1.0f, 0.0f, 0.0f));

		// --- GRUPO IZQUIERDO (Piezas 0, 1, 2)
		// No necesitan la rotación de 180 grados (están mirando hacia adelante por defecto)
		m_bodyParts[0]->translate(glm::vec3(-0.18, -0.077, -0.25));
		m_bodyParts[1]->translate(glm::vec3(-0.18, 0.077, -0.25));
		m_bodyParts[2]->translate(glm::vec3(-0.18, 0.004, -0.119));

		// Aplicar la rotación continua (rueda)
		m_bodyParts[0]->rotate(matriz_rotacion_rueda);
		m_bodyParts[1]->rotate(matriz_rotacion_rueda);
		m_bodyParts[2]->rotate(matriz_rotacion_rueda);

		// --- GRUPO DERECHO (Piezas 3, 4, 5)
		// Necesitan la rotación de 180 grados (para que "miren" a la izquierda, si el modelo está orientado al revés)

		// 1. Posicionar
		m_bodyParts[3]->translate(glm::vec3(0.18, -0.07, -0.25));
		m_bodyParts[4]->translate(glm::vec3(0.18, 0.076, -0.25));
		m_bodyParts[5]->translate(glm::vec3(0.18, 0.004, -0.12));

		// 3. Aplicar la rotación continua (rueda)
		m_bodyParts[3]->rotate(matriz_rotacion_rueda);
		m_bodyParts[4]->rotate(matriz_rotacion_rueda);
		m_bodyParts[5]->rotate(matriz_rotacion_rueda);

		if (m_pathType == Aceite)
		{
			translate(glm::vec3(m_pos.x + sin(timer * 10) * 0.0015f, m_pos.y, m_pos.z));
			glm::mat4 matriz_rotacionAceite1 = glm::rotate(glm::mat4(1.0f), sin(timer * 10.0f) * glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			rotate(matriz_rotacionAceite1); // Aplicar la rotación
		}
		else {
			if (m_pathType == Baches)
			{
				translate(glm::vec3(m_pos.x, m_pos.y, 0.3 + abs(sin(timer * 10) * 0.1f)));
			}
		}
		


		break;
	case Rapid:
		break;
	case Tanc:
		break;
	case Volador:
		break;
	case Accelerador: case AcceleradorACT:

		// Crear y aplicar la matriz de rotación
		matriz_rotacion = glm::rotate(glm::mat4(1.0f), -0.5f * timer * m_speed * deltaTime / m_defSpeed, glm::vec3(1.0f, 0.0f, 0.0f));
		m_bodyParts[0]->rotate(matriz_rotacion); // Aplicar la rotación

		if (m_pathType == Aceite) 
		{
			translate(glm::vec3(m_pos.x + sin(timer * 10) * 0.0025f, m_pos.y, m_pos.z));
			glm::mat4 matriz_rotacionAceite1 = glm::rotate(glm::mat4(1.0f),sin(timer * 10.0f) * glm::radians(5.0f),glm::vec3(0.0f, 1.0f, 0.0f) );
			rotate(matriz_rotacionAceite1); // Aplicar la rotación
		}
		else {
			if (m_pathType == Baches)
			{
				translate(glm::vec3(m_pos.x, m_pos.y, 0.5 + abs(sin(timer * 10) * 0.2f))); 
			}
		}
		break;

	case Divisible: case DivisibleDIV:   
		break;

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

void Enemy::setStartPoint(glm::vec2 startPoint) 
{ 
	m_prevTargetPos = startPoint;
	m_offset = (((float)rand() / (float)RAND_MAX) * 2.0 - 1.0) * m_maxOffset;
	m_pos = glm::vec3(startPoint.x, startPoint.y + m_offset, 0.5f); 
}
