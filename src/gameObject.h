#pragma once
#include "stdafx.h"
#include "objLoader.h"
#include "material.h"

class GameObject
{
public:
	GameObject() {};
	GameObject(COBJModel* model);
	void translate(glm::vec3 pos) { m_pos = pos; }
	void rotate(glm::mat4 rot) { m_rot = rot; }
	void scale(glm::vec3 scale) { m_scale = scale; }
	void setId(int id) { objectID = id; }
	glm::mat4 getModelMatrix();
	glm::mat4 getNormalMatrix();

	void dibuixarObjecte(GLuint shaderID, CColor colorObj, bool* sw_mat, glm::mat4 view);

	int objectID;
	int pickingID;
	bool m_texture;
	COBJModel* m_objModel;


	glm::mat4 m_parentMatrix;
	glm::vec3 m_pos;
	glm::mat4 m_rot;
	glm::vec3 m_scale;
}; 
