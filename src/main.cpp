//******** PRACTICA VISUALITZACI� GR�FICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn b�sic VS2022 MONOFINESTRA amb OpenGL 4.6, interf�cie GLFW 3.4, ImGui i llibreries GLM
//******** Ferran Poveda, Marc Vivet, Carme Juli�, D�bora Gil, Enric Mart� (Setembre 2025)
// main.cpp : Definici� de main
//    Versi� 0.5:	- Interficie ImGui
//					- Per a dialeg de cerca de fitxers, s'utilitza la llibreria NativeFileDialog


// Entorn VGI.ImGui: Includes llibreria ImGui
//#define STB_IMAGE_IMPLEMENTATION  // <--- ESTO ES VITAL
//#include "stb_image.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_glfw.h"
#include "ImGui\imgui_impl_opengl3.h"
#include "ImGui\nfd.h" // Native File Dialog

#include "stdafx.h"
#include "shader.h"
#include "visualitzacio.h"
#include "escena.h"
#include "main.h"
#include "menu.h"

#define Basic 0
#define Rapid 1
#define Tanc 2
#define Volador 3
#define Accelerador 4
#define Divisible 5
#define AcceleradorACT 6
#define DivisibleDIV 7

#define Normal 0
#define Aceite 1
#define Nuclear 2
#define Baches 3
#define Esquina 4



void InitGL()
{
// TODO: agregar aqu� el c�digo de construcci�n

//------ Entorn VGI: Inicialitzaci� de les variables globals de CEntornVGIView
	int i;

// Entorn VGI: Variable de control per a Status Bar (consola) 
	statusB = false;

// Entorn VGI: Variables de control per Men� Vista: Pantalla Completa, Pan, dibuixar eixos i grids 
	fullscreen = true;
	eixos = true;

// Entorn VGI: Variables de control Skybox Cube
	SkyBoxCube = true;		skC_programID = 0;
	skC_VAOID.vaoId = 0;	skC_VAOID.vboId = 0;	skC_VAOID.nVertexs = 36;
	cubemapTexture = 0;


// Entorn VGI: Variables de control del men� Shaders
	shader = CAP_SHADER;	shader_programID = 0;	
	shaderLighting.releaseAllShaders();
	// C�rrega Shader de Gouraud
	shader_programID = 0;
	fprintf(stderr, "phong_shdrML: \n"); 
	shader = PHONG_SHADER;

	fprintf(stderr, "Custom shader: \n");
	customShader.releaseAllShaders();
	customShaderID = customShader.loadFileShaders(".\\shaders\\customShader.vert", ".\\shaders\\customShader.frag");


	fprintf(stderr, "Picking objects shader: \n");
	poShader.releaseAllShaders();
	poShaderID = poShader.loadFileShaders(".\\shaders\\pickingObject.vert", ".\\shaders\\pickingObject.frag");


// C�rrega SHADERS
// C�rrega Shader Eixos
	fprintf(stderr, "Eixos: \n");
	if (!eixos_programID) eixos_programID = shaderEixos.loadFileShaders(".\\shaders\\eixos.VERT", ".\\shaders\\eixos.FRAG");

// C�rrega Shader Skybox
	fprintf(stderr, "SkyBox: \n");
	if (!skC_programID) skC_programID = shader_SkyBoxC.loadFileShaders(".\\shaders\\skybox.VERT", ".\\shaders\\skybox.FRAG");

// C�rrega VAO Skybox Cube
	if (skC_VAOID.vaoId == 0) skC_VAOID = loadCubeSkybox_VAO();
	Set_VAOList(CUBE_SKYBOX, skC_VAOID);

	if (!cubemapTexture)
	{	// load Skybox textures
		// -------------
		std::vector<std::string> faces =
		{ ".\\textures\\skybox\\right.jpg",
			".\\textures\\skybox\\left.jpg",
			".\\textures\\skybox\\top.jpg",
			".\\textures\\skybox\\bottom.jpg",
			".\\textures\\skybox\\front.jpg",
			".\\textures\\skybox\\back.jpg"
		};
		cubemapTexture = loadCubemap(faces);
	}


// Entorn VGI: Variables de control dels botons de mouse
	m_PosEAvall.x = 0;			m_PosEAvall.y = 0;			m_PosDAvall.x = 0;			m_PosDAvall.y = 0;
	m_ButoEAvall = false;		m_ButoDAvall = false;
	m_EsfeEAvall.R = 0.0;		m_EsfeEAvall.alfa = 0.0;	m_EsfeEAvall.beta = 0.0;
	m_EsfeIncEAvall.R = 0.0;	m_EsfeIncEAvall.alfa = 0.0;	m_EsfeIncEAvall.beta = 0.0;

// Entorn VGI: Variables que controlen par�metres visualitzaci�: Mides finestra Windows i PV
	w = 640;			h = 480;			// Mides de la finestra Windows (w-amplada,h-al�ada)
	width_old = 640;	height_old = 480;	// Mides de la resoluci� actual de la pantalla (finestra Windows)
	w_old = 640;		h_old = 480;		// Mides de la finestra Windows (w-amplada,h-al�ada) per restaurar Finestra des de fullscreen
	OPV.R = cam_Esferica[0];	OPV.alfa = cam_Esferica[1];		OPV.beta = cam_Esferica[2];		// Origen PV en esf�riques
	//OPV.R = 15.0;		OPV.alfa = 0.0;		OPV.beta = 0.0;										// Origen PV en esf�riques
	Vis_Polar = POLARZ;	oPolars = -1;

// Entorn VGI: Altres variables
	mida = 1.0;			nom = "";		buffer = "";
	
	//Carga modelos
	mm = modelManager();
	mm.initialSetup();
	createObject(mm.getMapa());

	//Carga player
	player = &Player::GetInstance();

	nCamaras = 6;
	camaras = std::vector<Camara>(nCamaras, Camara(w,h));

	camaraActual = 0;
	mainCamara = &camaras[CAM_ORBIT];

	camaras[CAM_TOP].translate(glm::vec3(-2, 1.51f, 30));
	camaras[CAM_TOP].target(glm::vec3(-2, 1.5f, 0));

	camaras[CAM_FRONT].translate(glm::vec3(-22, 0, 6));
	camaras[CAM_FRONT].target(glm::vec3(0, 0, 0));

	camaras[CAM_RIGHT].translate(glm::vec3(0, -20, 20));
	camaras[CAM_RIGHT].target(glm::vec3(0, 0, 0));

	freeCameraPos = glm::vec3(0, 0, 0);

	cameraSpeed = 10;
	distancia = 25;
	yawCamera = -135;
	pitchCamera = 45; 
	sensibilidad = 0.5;

	luz.InitIluminacion(w, h);
	direccionSol = glm::vec3(-0.9f, -3.0f, 2.0f);
	ambientIntensity = 0.3;
	lightColor = glm::vec3(1, 0.86f, 0.77f);

	boxSize = 25;
	near_plane = 1.0f;
	far_plane = 100.0f;

	luz.m_cam = &mainCamara;
	luz.objetos = &objects;
	luz.enemigos = &enemies;
	luz.turrets = turrets;

	po = PickingObjects3D(w, h, poShaderID);
	po.m_cam = &mainCamara;
	po.m_turrets = turrets;
	po.m_objectes = &objects;
	luz.m_po = &po;
	

	initVAOList();	// Inicialtzar llista de VAO'S.
}

void InitAPI()
{
// Vendor, Renderer, Version, Shading Laguage Version i Extensions suportades per la placa gr�fica gravades en fitxer extensions.txt
	std::string nomf = "extensions.txt";
	char const* nomExt = "";
	const char* nomfitxer;
	nomfitxer = nomf.c_str();	// Conversi� tipus string --> char *
	int num_Ext;

	char* str = (char*)glGetString(GL_VENDOR);
	FILE* f = fopen(nomfitxer, "w");
	if(f)	{	fprintf(f,"VENDOR: %s\n",str);
				fprintf(stderr, "VENDOR: %s\n", str);
				str = (char*)glGetString(GL_RENDERER);
				fprintf(f, "RENDERER: %s\n", str);
				fprintf(stderr, "RENDERER: %s\n", str);
				str = (char*)glGetString(GL_VERSION);
				fprintf(f, "VERSION: %s\n", str);
				fprintf(stderr, "VERSION: %s\n", str);
				str = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
				fprintf(f, "SHADING_LANGUAGE_VERSION: %s\n", str);
				fprintf(stderr, "SHADING_LANGUAGE_VERSION: %s\n", str);
				glGetIntegerv(GL_NUM_EXTENSIONS, &num_Ext);
				fprintf(f, "EXTENSIONS: \n");
				fprintf(stderr, "EXTENSIONS: \n");
				for (int i = 0; i < num_Ext; i++)
				{	nomExt = (char const*)glGetStringi(GL_EXTENSIONS, i);
					fprintf(f, "%s \n", nomExt);
					//fprintf(stderr, "%s", nomExt);	// Displaiar extensions per pantalla
				}
				//fprintf(stderr, "\n");				// Displaiar <cr> per pantalla despr�s extensions
//				str = (char*)glGetString(GL_EXTENSIONS);
//				fprintf(f, "EXTENSIONS: %s\n", str);
				//fprintf(stderr, "EXTENSIONS: %s\n", str);
				fclose(f);
			}

// Program
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
	glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
	glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");
	glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)wglGetProcAddress("glVertexAttrib1fv");
	glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)wglGetProcAddress("glVertexAttrib2fv");
	glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)wglGetProcAddress("glVertexAttrib3fv");
	glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)wglGetProcAddress("glVertexAttrib4fv");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");

// Shader
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");

// VBO
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
}

Enemy* spawnEnemy(int type)
{
	Enemy* newEnemy = new Enemy(mm.getEnemy(type), type);
	newEnemy->setId(enemies.size());
	enemies.push_back(newEnemy);

	Path* start = path.front();
	newEnemy->setStartPoint(start->getPos());

	// target = siguiente nodo si existe, sino mismo
	if (type == Volador) 	newEnemy->setTarget(path[path.size() - 2]);
	else
	{
		start = start->getNextPath() ? start->getNextPath() : nullptr;
		newEnemy->setTarget(start);
	}

	newEnemy->startMoving();

	return newEnemy;
}

void destroyEnemies(Enemy* en)
{
	auto it = std::find(enemies.begin(), enemies.end(), en);
	if (it == enemies.end()) return;
	if (en->mustDestroy()) return;

	enemies.erase(it);
	delete en;
}

void divideEnemy(Enemy* en) 
{
	if (en->mustDestroy()) return;
	if (en->getEnemyType() == Divisible && en->getTarget() != nullptr)
	{
		Path* nt = en->getTarget();
		Enemy* div = spawnEnemy(DivisibleDIV);
		div->copyMovementData(nt);
		div->translate(en->getPos() + glm::vec3(0.25f, 0.25f, 0.0f));

		div = spawnEnemy(DivisibleDIV);
		div->copyMovementData(nt);
		div->translate(en->getPos() + glm::vec3(-0.25f, -0.25f, 0.0f));
	}

}

GameObject* createObject(COBJModel* model)
{
	GameObject* newObject = new GameObject(model);
	newObject->setId(objects.size());
	

	objects.push_back(newObject);
	return newObject;
}

Path* createPath(glm::vec2 pos, float speedMultiplier = 1.0f, int tipo = 0)
{
	Path* newPath = new Path(pos, speedMultiplier, tipo);

	if (!path.empty())
	{
		Path* prev = path.back();
		prev->setNextPath(newPath);
		newPath->setPreviousPath(prev);

		prev->calculateBisector();
	}

	newPath->calculateBisector();

	path.push_back(newPath);

	return newPath;
}


void setUpPath() 
{

	createPath(glm::vec2(10, 0.25));

	createPath(glm::vec2(7, 0.25));
	createPath(glm::vec2(5.5, 0.25), 0.8, Esquina);

	createPath(glm::vec2(5.5, -3));
	createPath(glm::vec2(5.5, -4.15), 0.8, Esquina);

	createPath(glm::vec2(2, -4.15));
	createPath(glm::vec2(0.9, -4.15), 0.8, Esquina);


	createPath(glm::vec2(0.9, -3.5));
	createPath(glm::vec2(0.9, 4), 1.25, Aceite); // CAMINO ACEITE
	createPath(glm::vec2(0.9, 4.75), 0.8, Esquina);

	createPath(glm::vec2(7.25, 4.75));
	createPath(glm::vec2(8.25, 4.75), 0.8, Esquina);

	createPath(glm::vec2(8.25, 7.75));
	createPath(glm::vec2(8.25, 8.75), 0.8, Esquina);

	createPath(glm::vec2(8, 8.75));
	createPath(glm::vec2(-2.75, 8.75), 0.7, Nuclear);  // CAMINO NUCLEAR
	createPath(glm::vec2(-3.25, 8.75), 0.8, Esquina);

	//////////////////////
	createPath(glm::vec2(-3.25, 4.75));
	createPath(glm::vec2(-3.25, -3.0), 0.7, Baches);  // CAMINO BACHES
	createPath(glm::vec2(-3.25, -5.5));
	createPath(glm::vec2(-3.25, -6.7), 0.8, Esquina);
	//////////////////////

	createPath(glm::vec2(-13.75, -6.7));
	createPath(glm::vec2(-14.85, -6.7), 0.8, Esquina);

	createPath(glm::vec2(-14.85, -4));
	createPath(glm::vec2(-14.85, -2.5), 0.8, Esquina);

	createPath(glm::vec2(-14, -2.5)); 
	createPath(glm::vec2(-9.5, -2.5), 1.25, Aceite); // CAMINO ACEITE
	createPath(glm::vec2(-8, -2.5), 0.8, Esquina);

	createPath(glm::vec2(-8, 0.5));
	createPath(glm::vec2(-8, 1.75), 0.8, Esquina);

	createPath(glm::vec2(-20, 1.75));
	createPath(glm::vec2(-20.5, 1.75));
	

}

void destroyObject(GameObject* obj)
{
	int id = obj->getId();
	int vectorSize = objects.size();

	if (id < vectorSize)
	{
		GameObject* lastObject = objects.back();
		if (id != vectorSize - 1)
			lastObject->setId(id);

		std::swap(objects[id], objects.back());
		objects.pop_back();
		delete obj;
	}
}

void modifyTurret(int id, int type)
{
	if (type == -1) 
	{
		std::vector<COBJModel*> emptyModels;
		turrets[id]->loadTurret(type, emptyModels);
	}
	else 
	{
		turrets[id]->loadTurret(type, mm.getTurret(type));
	}
}

void setUpTurrets() 
{
	glm::vec2 pos[NTURRETS];

	//Posicions torres

	pos[0] = glm::vec2(3.25f, -2.1f);
	pos[1] = glm::vec2(-1.2f, 2.35f);
	pos[2] = glm::vec2(6.15f, 6.75f);
	pos[3] = glm::vec2(1.85f, 11.125f);
	pos[4] = glm::vec2(-5.45f, 6.45f);
	pos[5] = glm::vec2(-6.2f, -9.1f);
	pos[6] = glm::vec2(-12.75f, -4.75f);
	pos[7] = glm::vec2(-9.9f, -0.6f);
	pos[8] = glm::vec2(-15.5f, 3.9f);


	for (int i = 0; i < NTURRETS; i++)
	{
		turrets[i] = new Turret(i+1);
		turrets[i]->setPos(pos[i]);
		turrets[i]->setEnemiesVector(&enemies);
	}
}

void OnSize(GLFWwindow* window, int width, int height)
{
	w = width;	h = height;
	luz.UpdateWindow(width, height);
	for (Camara& c : camaras)
	{
		c.UpdateWindow(w, h);
	}
	mainCamara->UpdateWindow(width, height);
	po.updatePickingObjectSize(width, height);
}

// Skybox
void OnVistaSkyBox()
{
// C�rrega Shader Skybox
	if (!skC_programID) skC_programID = shader_SkyBoxC.loadFileShaders(".\\shaders\\skybox.VERT", ".\\shaders\\skybox.FRAG");

// C�rrega VAO Skybox Cube
	if (skC_VAOID.vaoId == 0) skC_VAOID = loadCubeSkybox_VAO();
	Set_VAOList(CUBE_SKYBOX, skC_VAOID);

	if (!cubemapTexture)
	{	// load Skybox textures
		// -------------
		std::vector<std::string> faces =
		{ ".\\textures\\skybox\\right.jpg",
			".\\textures\\skybox\\left.jpg",
			".\\textures\\skybox\\top.jpg",
			".\\textures\\skybox\\bottom.jpg",
			".\\textures\\skybox\\front.jpg",
			".\\textures\\skybox\\back.jpg"
		};
		cubemapTexture = loadCubemap(faces);
	}
}


/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL RATOLI                              */
/* ------------------------------------------------------------------------- */

void OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(button, action);

	if (!io.WantCaptureMouse) {
		// OnLButtonDown
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			m_ButoEAvall = true;
			m_PosEAvall.x = xpos;	m_PosEAvall.y = ypos;
			m_EsfeEAvall = OPV;
			//Picking objects JAVI AQU�
		}
		// OnLButtonUp
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
			m_ButoEAvall = false;
		// OnRButtonDown
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			m_ButoDAvall = true;
			m_PosDAvall.x = xpos;	m_PosDAvall.y = ypos;
		}
		// OnRButtonUp
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
			m_ButoDAvall = false;
	}
}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{

	if (m_ButoEAvall)
	{
		double deltaX = xpos - m_PosEAvall.x;
		double deltaY = ypos - m_PosEAvall.y;

		
		yawCamera += (float)deltaX * sensibilidad * ((camaraActual == CAM_FREE) ? -1 : 1);
		pitchCamera -= (float)deltaY * sensibilidad * ((camaraActual == CAM_FREE) ? -1 : 1);

		m_PosEAvall.x = xpos;
		m_PosEAvall.y = ypos;

		if (camaraActual != CAM_FREE)
		{
			if (pitchCamera > 89.0f)  pitchCamera = 89.0f;
			if (pitchCamera < 15) pitchCamera = 15;
		}
		else
		{
			if (pitchCamera > 89.0f)  pitchCamera = 89.0f;
			if (pitchCamera < -89.0f) pitchCamera = -89.0f;
		}

		while (yawCamera >= 360.0f) yawCamera -= 360.0f;
		while (yawCamera < 0.0f)    yawCamera += 360.0f;


	}

	float yawRadiants = glm::radians(yawCamera);
	float pitchRadiants = glm::radians(pitchCamera);

	glm::vec3 forward;

	forward.x = glm::cos(yawRadiants) * glm::cos(pitchRadiants);
	forward.y = glm::sin(yawRadiants) * glm::cos(pitchRadiants);
	forward.z = glm::sin(pitchRadiants);

	forward = glm::normalize(forward);
	cameraDir = forward;
	cameraRight = glm::normalize(glm::cross(forward, glm::vec3(0, 0, 1)));
	
	if (camaraActual != CAM_FREE)
	{
		freeCameraPos = forward * distancia;
	}

	camaras[CAM_ORBIT].translate(forward * distancia);
	camaras[CAM_ORBIT].target(glm::vec3(0, 0, 0));



	//ZOOM

	if (m_ButoDAvall)
	{
		distancia += m_PosDAvall.y - ypos;
		if (distancia < 2) { distancia = 2; }
		if (distancia > 50) { distancia = 50; }
		m_PosDAvall.x = xpos;				m_PosDAvall.y = ypos;
	}
	

}


void OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods) {

	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard)
		return;

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_F:
			OnFull_Screen(primary, window);
			break;
		case GLFW_KEY_T:
			for (Enemy* e : enemies)
				e->takeDamage(2.5f);
			break;
		case GLFW_KEY_1:
			spawnEnemy(Basic);
			break;
		case GLFW_KEY_2:
			spawnEnemy(Rapid);
			break;
		case GLFW_KEY_3:
			spawnEnemy(Tanc);
			break;
		case GLFW_KEY_4:
			spawnEnemy(Volador);
			break;
		case GLFW_KEY_5:
			spawnEnemy(Accelerador);
			break;
		case GLFW_KEY_6:
			spawnEnemy(Divisible);
			break;
		case GLFW_KEY_X:
			camaraActual++;
			break;
		case GLFW_KEY_Z:
			camaraActual--;
			break;
		default:
			break;
		}

		camaraActual = (camaraActual % nCamaras + nCamaras) % nCamaras;

		mainCamara = &camaras[camaraActual];


		if (key == GLFW_KEY_W) keyW = true;
		if (key == GLFW_KEY_A) keyA = true;
		if (key == GLFW_KEY_S) keyS = true;
		if (key == GLFW_KEY_D) keyD = true;
	}


	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_W) keyW = false;
		if (key == GLFW_KEY_A) keyA = false;
		if (key == GLFW_KEY_S) keyS = false;
		if (key == GLFW_KEY_D) keyD = false;
	}

	inputVector = glm::vec3(0);
	if (keyW) inputVector += glm::vec3(0, 1, 0);
	if (keyS) inputVector += glm::vec3(0, -1, 0);
	if (keyA) inputVector += glm::vec3(-1, 0, 0);
	if (keyD) inputVector += glm::vec3(1, 0, 0);
}

// Entorn VGI. OnFull_Screen: Funci� per a pantalla completa
void OnFull_Screen(GLFWmonitor* monitor, GLFWwindow *window)
{
	fullscreen = !fullscreen;
	if (fullscreen) 
	{	// backup window position and window size
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		w = mode->width;	h = mode->height;
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else // Restore last window size and position
		glfwSetWindowMonitor(window, nullptr, 216, 239, 640, 480, mode->refreshRate);
}

// -------------------- TRACTAMENT ERRORS NO TOCAR
// error_callback: Displaia error que es pugui produir
void error_callback(int code, const char* description)
{
    //const char* descripcio;
    //int codi = glfwGetError(&descripcio);

 //   display_error_message(code, description);
	fprintf(stderr, "Codi Error: %i", code);
	fprintf(stderr, "Descripcio: %s\n",description);
}

GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		fprintf(stderr, "ERROR %s | File: %s | Line ( %3i ) \n", error.c_str(), file, line);
		//fprintf(stderr, "ERROR %s | ", error.c_str());
		//fprintf(stderr, "File: %s | ", file);
		//fprintf(stderr, "Line ( %3i ) \n", line);
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

	fprintf(stderr, "---------------\n");
	fprintf(stderr, "Debug message ( %3i %s \n", id, message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             fprintf(stderr, "Source: API \n"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   fprintf(stderr, "Source: Window System \n"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: fprintf(stderr, "Source: Shader Compiler \n"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     fprintf(stderr, "Source: Third Party \n"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     fprintf(stderr, "Source: Application \n"); break;
	case GL_DEBUG_SOURCE_OTHER:           fprintf(stderr, "Source: Other \n"); break;
	} //std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               fprintf(stderr, "Type: Error\n"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: fprintf(stderr, "Type: Deprecated Behaviour\n"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  fprintf(stderr, "Type: Undefined Behaviour\n"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         fprintf(stderr, "Type: Portability\n"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         fprintf(stderr, "Type: Performance\n"); break;
	case GL_DEBUG_TYPE_MARKER:              fprintf(stderr, "Type: Marker\n"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          fprintf(stderr, "Type: Push Group\n"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           fprintf(stderr, "Type: Pop Group\n"); break;
	case GL_DEBUG_TYPE_OTHER:               fprintf(stderr, "Type: Other\n"); break;
	} //std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         fprintf(stderr, "Severity: high\n"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       fprintf(stderr, "Severity: medium\n"); break;
	case GL_DEBUG_SEVERITY_LOW:          fprintf(stderr, "Severity: low\n"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: fprintf(stderr, "Severity: notification\n"); break;
	} //std::cout << std::endl;
	//std::cout << std::endl;
	fprintf(stderr, "\n");
}

void CamerasUpdate() 
{
	Enemy* target = nullptr;
	float maxProg = 0;
	for (Enemy* e : enemies) 
	{
		float prog = e->getProgress();
		if (prog > maxProg)
		{
			maxProg = prog;
			target = e;
		}
	}

	if (target != nullptr)
	{
		camaras[CAM_FOLLOW].translate(cameraDir * distancia + target->getPos());
		camaras[CAM_FOLLOW].target(target->getPos());
	}
	else
	{
		camaras[CAM_FOLLOW] = camaras[CAM_ORBIT];
	}


	if (inputVector.y == 1)	freeCameraPos -= cameraDir * cameraSpeed * deltaTime;
	if (inputVector.x == -1) freeCameraPos += cameraRight * cameraSpeed * deltaTime;
	if (inputVector.y == -1) freeCameraPos += cameraDir * cameraSpeed * deltaTime;
	if (inputVector.x == 1) freeCameraPos -= cameraRight * cameraSpeed * deltaTime;

	camaras[CAM_FREE].translate(freeCameraPos);
	camaras[CAM_FREE].target(freeCameraPos - cameraDir);
}

//-----------------Variables globales

void Update(float timer, float deltaTime) 
{
	for (Enemy* e : enemies) 
	{
		e->move(deltaTime, timer);
		divideEnemy(e);
		destroyEnemies(e);
	}

	for (int i = 0; i < NTURRETS; i++) 
	{
		turrets[i]->mainUpdate(deltaTime);
	}
}


int main(void)
{
	//    GLFWwindow* window;
	// Entorn VGI. Timer: Variables
	float time = elapsedTime;
	float now;
	float delta;

	// glfw: initialize and configure
	// ------------------------------
	if (!glfwInit())
	{	fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	// Retrieving main monitor
    primary = glfwGetPrimaryMonitor();

	// To get current video mode of a monitor
    mode = glfwGetVideoMode(primary);

    int countVM;
    const GLFWvidmode* modes = glfwGetVideoModes(primary, &countVM);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Entorn Grafic VS2022 amb GLFW i OpenGL 4.6 (Visualitzacio Grafica Interactiva - Grau en Enginyeria Informatica - Escola Enginyeria - UAB)", NULL, NULL);
    if (!window)
    {	fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 4.6 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
        return -1;
    }

	// Make the window's context current
    glfwMakeContextCurrent(window);

	// Llegir resoluci� actual de pantalla
	glfwGetWindowSize(window, &width_old, &height_old);

	// Initialize GLEW
	if (GLEW_VERSION_3_3) glewExperimental = GL_TRUE; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		glGetError();	// Esborrar codi error generat per bug a llibreria GLEW
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Initialize Application control varibles
	InitGL();


	glEnable(GL_FRAMEBUFFER_SRGB);


	//cargarModelos();

	// ------------- Entorn VGI: Callbacks
	// Set GLFW event callbacks. I removed glfwSetWindowSizeCallback for conciseness
	glfwSetWindowSizeCallback(window, OnSize);										// - Windows Size in screen Coordinates
	glfwSetFramebufferSizeCallback(window, OnSize);									// - Windows Size in Pixel Coordinates
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)OnMouseButton);			// - Directly redirect GLFW mouse button events
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)OnMouseMove);				// - Directly redirect GLFW mouse position events
	glfwSetKeyCallback(window, OnKeyDown);										// - Directly redirect GLFW key events
	//glfwSetCharCallback(window, OnTextDown);										// - Directly redirect GLFW char events
	glfwSetErrorCallback(error_callback);											// Error callback

	// Entorn VGI; Timer: Lectura temps
	float previous = glfwGetTime();

	// Entorn VGI.ImGui: Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;


	// Entorn VGI.ImGui: Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

	// Entorn VGI.ImGui: Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	// Entorn VGI.ImGui: End Setup Dear ImGui context

	//Dibuixa els eixos
	if (eixos) 
	{
		if (!eixos_programID) eixos_programID = shaderEixos.loadFileShaders(".\\shaders\\eixos.VERT", ".\\shaders\\eixos.FRAG");
		if (!eixos_Id) eixos_Id = deixos(); // Funci� que defineix els Eixos Coordenades M�n com un VAO.
	}
	if (SkyBoxCube) OnVistaSkyBox();
	if (fullscreen)
	{
		const GLFWvidmode* currentMode = glfwGetVideoMode(primary);
		if (currentMode) {
			// Store original window size before going fullscreen
			w_old = w;
			h_old = h;
			glfwSetWindowMonitor(window, primary, 0, 0, currentMode->width, currentMode->height, currentMode->refreshRate);
			w = currentMode->width;
			h = currentMode->height;
			glViewport(0, 0, w, h);
		}
		else {
			fprintf(stderr, "Failed to get video mode for primary monitor.\n");
			fullscreen = false;
		}
	}

	setUpPath();
	setUpTurrets();
	luz.TurretsWereLoaded();
	modifyTurret(0, -1);
	modifyTurret(1, CONGELADORA);
	modifyTurret(2, LASER);
	modifyTurret(3, METRALLADORA);
	modifyTurret(4, VERI);
	modifyTurret(5, FRANCOTIRADORA);
	modifyTurret(6, CONGELADORA);
	modifyTurret(7, LASER);
	modifyTurret(8, METRALLADORA);

	glEnable(GL_DEPTH_TEST);
	bool salir = false;



    while (!glfwWindowShouldClose(window) and !salir)
    {  
		now = glfwGetTime();
		delta = now - previous;
		previous = now;
		deltaTime = delta;

		// --- ERROR EN TU CÓDIGO ORIGINAL ---
		// frameTimer += deltaTime;  <-- BORRA ESTO DE AQUÍ (si lo dejas, el tiempo avanza aunque estés en pausa)

		// Poll for and process events
		glfwPollEvents();

		// Draws the UI
		menu(salir);

		// LÓGICA DE RESET (Modo Pruebas)
		if (debug_resetear_todo)
		{
			// Reiniciar Timer
			frameTimer = 0.0f;

			// Matar Enemigos (Limpiar vector)
			for (Enemy* en : enemies) 
			{
				en->die();// Liberar memoria
			}
			//enemies.clear(); // Vaciar lista

			// Apagar la bandera para que no se ejecute infinitamente
			debug_resetear_todo = false;
		}


		// LÓGICA DE SPAWN MANUAL (Modo Pruebas)
		if (debug_solicitar_spawn && debug_id_enemigo_spawn >= 0 && debug_id_enemigo_spawn <= 5)
		{

			for (int i = 0; i < debug_num_enemigo_spawn; i++)
				// Llamamos a tu funcion existente spawnEnemy con el ID del hueco
				spawnEnemy(debug_id_enemigo_spawn);

			// Apagamos la bandera inmediatamente para que no spawnee infinitos
			debug_solicitar_spawn = false;
		}

		// CONTROL DE ACTUALIZACIÓN (UPDATE)
		// Se actualiza si:
		// 1. NO está en pausa global (ESC)
		// 2. Y ADEMÁS: (Estamos jugando) O (Estamos en pruebas Y el timer no está detenido manualmente)
		bool debeActualizar = !juego_pausado && (show_jugar || (show_menu_pruebas && !debug_detener_tiempo));

		if (debeActualizar)
		{
			Update(frameTimer, deltaTime * debug_speedMult);
			frameTimer += deltaTime * debug_speedMult;
		}
		CamerasUpdate();

		po.renderPicking();
		
		// CONSTRUIMOS EL VECTOR DE LUZ DESDE LOS SLIDERS
		glm::vec3 lightDir(debug_lightDir[0], debug_lightDir[1], debug_lightDir[2]);

		// LLAMADA MODIFICADA RENDER SHADOWS
		luz.RenderShadows(lightDir, debug_boxSize, debug_nearPlane, debug_farPlane);

		glm::vec3 lightColor(debug_lightColor[0], debug_lightColor[1], debug_lightColor[2]);

		// LLAMADA MODIFICADA RENDER GAME
		luz.RenderGame(customShaderID, debug_ambientIntensity, lightColor, debug_renderMode);



		dibuixa_Skybox(skC_programID, cubemapTexture, Vis_Polar, mainCamara->getProjection(), mainCamara->getView());
		//dibuixa_Eixos(eixos_programID, eixos, eixos_Id, grid, hgrid, mainCamara.getProjection(), mainCamara.getView());

		

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// Entorn VGI: Activa la finestra actual
		glfwMakeContextCurrent(window);

		// Entorn VGI: Transfer�ncia del buffer OpenGL a buffer de pantalla
		glfwSwapBuffers(window);
    }

	// Check if the ESC key was pressed or the window was closed o boton salir presionado
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0 && !salir);

	// Delete all the objects in the scene
	for (GameObject* obj : objects)
		DeleteObject(obj);

	for (Enemy* en : enemies) 
		destroyEnemies(en);

	for (Turret* tu : turrets)
		delete tu;			

	// Entorn VGI.ImGui: Cleanup ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);


	// Terminating GLFW: Destroy the windows, monitors and cursor objects
    glfwTerminate();

	if (shaderLighting.getProgramID() != -1) shaderLighting.DeleteProgram();
	if (shaderSkyBox.getProgramID() != -1) shaderSkyBox.DeleteProgram();
    return 0;
}