#pragma once

class Path {
public:
	Path(glm::vec2 pos = glm::vec2(0, 0), Path* np = nullptr, float sm = 1)
	{
		m_pos = pos;
		m_nextPath = np;
		m_speedMultiplier = sm;
	}
	Path* getNextPath() { return m_nextPath; }
	void setPreviousPath(Path* path) { m_prevPath = path; }
	void calculateBisector()
	{
		glm::vec2 dir1 = glm::normalize(m_pos - m_prevPath->m_pos);
		glm::vec2 dir2 = glm::normalize(m_nextPath->m_pos - m_pos);

		m_bisectorNormal = glm::normalize(dir1 - dir2);
	}
	glm::vec2 getBisector() { return m_bisectorNormal; }
	glm::vec2 getNextDir() { return glm::normalize(m_nextPath->m_pos - m_pos); }
	glm::vec2 getPos() { return m_pos; }
	float getSpeedMultiplier() { return m_speedMultiplier; }

private:
	float m_speedMultiplier = 1;
	glm::vec2 m_pos;

	Path* m_prevPath = nullptr;
	Path* m_nextPath = nullptr;

	glm::vec2 m_bisectorNormal = glm::vec2(0, 0);
};