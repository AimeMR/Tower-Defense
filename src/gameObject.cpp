#include "gameObject.h"

GameObject::GameObject(COBJModel* objModel)
{
	m_parentMatrix = glm::mat4(1.0f);
	m_objModel = objModel;
	m_pos = glm::vec3(0, 0, 0);
	m_rot = glm::mat4(1.0);
	m_scale = glm::vec3(1, 1, 1);
}

void GameObject::dibuixarObjecte(GLuint shaderID)
{

	//SeleccionaMaterial(shaderID, 0, sw_mat);

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
