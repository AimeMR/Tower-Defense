#pragma once

#include "stdafx.h"
#include "constants.h"

class Camara {
public:
	Camara();
	void UpdateWindow(int width, int height) { m_w = width; m_h = height; }

	void translate(glm::vec3 pos) { m_position = pos; }
	void rotate(glm::mat4 rot) { m_rotation = rot; }
	void rotate(glm::vec3 eulerRot);
	void target(glm::vec3 target);

	glm::mat4 getProjection();
	glm::mat4 getView();

	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;

	glm::vec3 m_position;
	glm::mat4 m_rotation;

private:
	
	double m_near, m_far;
	int m_w, m_h;
	double m_zoom;


};