#pragma once
#include "stdafx.h"
#include "objLoader.h"
#include "material.h"

class GameObject
{
public:
	GameObject() {};
	GameObject(COBJModel* model);
	void transform(glm::vec3 pos, glm::mat4 rot, glm::vec3 scale);
	glm::mat4 getModelMatrix();
	glm::mat4 getNormalMatrix();

	void dibuixarObjecte(GLuint shaderID, CColor colorObj, bool* sw_mat, glm::mat4 view);

	int ID;
	bool m_texture;
	COBJModel* m_objModel;


	glm::mat4 m_parentMatrix;
	glm::vec3 m_pos;
	glm::mat4 m_rot;
	glm::vec3 m_scale;
}; 
