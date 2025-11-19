#pragma once
#include "modelManager.h"

class GameObject
{
public:
	GameObject(COBJModel* objModel);
	void translate(glm::vec3 pos);
	glm::vec3 getPos() { return m_pos; }
	void rotate(glm::mat4 rot);
	glm::mat4 getRot() { return m_rot; }
	void scale(glm::vec3 scale) { m_scale = scale; }
	void setId(int id) { objectID = id; }
	void setParent(GameObject* parent) { m_parent = parent; }
	int getId() { return objectID; }
	glm::mat4 getModelMatrix();
	glm::mat4 getNormalMatrix();

	void dibuixarObjecte(GLuint shaderID);

protected:
	glm::mat4 m_parentMatrix;
	glm::vec3 m_pos;
	glm::mat4 m_rot;
	glm::vec3 m_scale;

	glm::vec4 m_colorBase;
	bool m_maskColor;
private:

	COBJModel* m_objModel;
	bool m_texture = false;
	int objectID;
	int pickingID;
	GameObject* m_parent = nullptr;


};