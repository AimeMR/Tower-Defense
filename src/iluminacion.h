#pragma once

#include "stdafx.h"
#include "visualitzacio.h"
#include "Turret.h"
#include "shader.h"
#include "camara.h"

class Iluminacion {
public:

	Iluminacion() { m_turretsLoaded = false; }

	void InitIluminacion(int width, int height);
	void UpdateWindow(int width, int height);

	void RenderShadows(glm::vec3 lightDir);
	void RenderGame(GLuint shaderID);

	void TurretsWereLoaded() { m_turretsLoaded = true; }

	Camara* m_cam;
	std::vector<GameObject*>* objetos;
	std::vector<Enemy*>* enemigos;
	Turret** turrets;

private:
	// camara y ventana
	int w, h;

	//sombras
	GLuint m_shadowShaderID;
	Shader m_shadowShader;

	GLuint m_depthMapFB;
	GLuint m_depthMap;
	int m_shadowWidth = 1024;
	int m_shadowHeight = 1024;

	//luz
	glm::vec3 m_lightDirection;
	glm::mat4 m_lightSpaceMatrix;
	float m_ambientIntensity;
	glm::vec3 m_lightColor;

	//control torretas
	bool m_turretsLoaded = false;

	//objetos
};

