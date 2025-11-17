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
	void setNextPath(Path* path) { m_nextPath = path; }
	void calculateBisector()
	{
		if (m_prevPath && m_nextPath)
		{
			glm::vec2 dir1 = glm::normalize(m_pos - m_prevPath->m_pos);
			glm::vec2 dir2 = glm::normalize(m_nextPath->m_pos - m_pos);

			glm::vec2 bis = glm::normalize(dir1 - dir2);

			if (glm::dot(bis, dir1) < 0.0f)
				bis = -bis;

			m_bisectorNormal = bis;
		}
		else
		{
			m_bisectorNormal = glm::vec2(1, 0);
		}
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