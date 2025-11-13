#include "gameObject.h"

GameObject::GameObject(COBJModel* objModel)
{
	m_parentMatrix = glm::mat4(1.0f);
	m_objModel = objModel;
	m_pos = glm::vec3(0, 0, 0);
	m_rot = glm::mat4(1.0);
	m_scale = glm::vec3(1, 1, 1);
	objectID = -1;
	pickingID = -1;
}

void GameObject::dibuixarObjecte(GLuint shaderID)
{
	CColor white;
	white.r = 1;
	white.g = 1;
	white.b = 1;
	white.a = 1;


	int id = pickingID;

	int r = (id & 0x000000FF) >> 0;
	int g = (id & 0x0000FF00) >> 8;
	int b = (id & 0x00FF0000) >> 16;
	glUniform4f(glGetUniformLocation(shaderID, "PickingColor"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

	glm::mat4 model(1.0f), normal(1.0f);


	model = getModelMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "modelMatrix"), 1, GL_FALSE, &model[0][0]);
	
	//normal = transpose(inverse(view * model));
	normal = transpose(inverse(model));
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "normalMatrix"), 1, GL_FALSE, &normal[0][0]);
	glUniform1i(glGetUniformLocation(shaderID, "textur"), false);
	m_objModel->draw_TriVAO_OBJ(shaderID);
}


	
glm::mat4 GameObject::getModelMatrix()
{
	glm::mat4 TransMatrix;

	TransMatrix = glm::scale(m_parentMatrix, m_scale);

	TransMatrix = m_rot * TransMatrix;

	TransMatrix = glm::translate(glm::mat4(1.0f), m_pos) * TransMatrix;


	return TransMatrix;
}

glm::mat4 GameObject::getNormalMatrix()
{
	glm::mat4 modelM = getModelMatrix();
	return transpose(inverse(modelM));
}
