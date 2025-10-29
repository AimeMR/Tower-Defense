//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MONOFINESTRA amb OpenGL 4.6, interfície GLFW 3.4, ImGui i llibreries GLM
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2025)
// main.cpp : Definició de main
//    Versió 0.5:	- Interficie ImGui
//					- Per a dialeg de cerca de fitxers, s'utilitza la llibreria NativeFileDialog


// Entorn VGI.ImGui: Includes llibreria ImGui
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_glfw.h"
#include "ImGui\imgui_impl_opengl3.h"
#include "ImGui\nfd.h" // Native File Dialog

#include "stdafx.h"
#include "shader.h"
#include "visualitzacio.h"
#include "escena.h"
#include "main.h"
#include "material.h"


void InitGL()
{
// TODO: agregar aquí el código de construcción

//------ Entorn VGI: Inicialització de les variables globals de CEntornVGIView
	int i;

// Entorn VGI: Variable de control per a Status Bar (consola) 
	statusB = false;

// Entorn VGI: Variables de control per Menú Càmera: Esfèrica, Navega, Mòbil, Zoom, Satelit, Polars... 
	camera = CAM_ESFERICA;
	mobil = true;	zzoom = true;		zzoomO = false;		satelit = false;

// Entorn VGI: Variables de control de l'opció Càmera->Navega?
	n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
	opvN.x = 10.0;	opvN.y = 0.0;		opvN.z = 0.0;
	angleZ = 0.0;
	ViewMatrix = glm::mat4(1.0);		// Inicialitzar a identitat

// Entorn VGI: Variables de control de l'opció Càmera->Geode?
	OPV_G.R = 15.0;		OPV_G.alfa = 0.0;	OPV_G.beta = 0.0;	// Origen PV en esfèriques per a Vista_Geode

// Entorn VGI: Variables de control per Menú Vista: Pantalla Completa, Pan, dibuixar eixos i grids 
	fullscreen = true;
	pan = false;
	eixos = true;	eixos_programID = 0;  eixos_Id = 0;
	sw_grid = false;
	grid.x = false;	grid.y = false;		grid.z = false;		grid.w = false;
	hgrid.x = 0.0;	hgrid.y = 0.0;		hgrid.z = 0.0;		hgrid.w = 0.0;

// Entorn VGI: Variables opció Vista->Pan
	fact_pan = 1;
	tr_cpv.x = 0;	tr_cpv.y = 0;	tr_cpv.z = 0;		tr_cpvF.x = 0;	tr_cpvF.y = 0;	tr_cpvF.z = 0;

// Entorn VGI: Variables de control per les opcions de menú Projecció, Objecte
	projeccio = PERSPECT;
	ProjectionMatrix = glm::mat4(1.0);	// Inicialitzar a identitat
	objecte = CAP;		// objecte = TETERA;

// Entorn VGI: Variables de control Skybox Cube
	SkyBoxCube = true;		skC_programID = 0;
	skC_VAOID.vaoId = 0;	skC_VAOID.vboId = 0;	skC_VAOID.nVertexs = 36;
	cubemapTexture = 0;

// Entorn VGI: Variables de control del menú Transforma
	transf = false;		trasl = false;		rota = false;		escal = false;
	fact_Tras = 1;		fact_Rota = 90;
	TG.VTras.x = 0.0;	TG.VTras.y = 0.0;	TG.VTras.z = 0;	TGF.VTras.x = 0.0;	TGF.VTras.y = 0.0;	TGF.VTras.z = 0;
	TG.VRota.x = 0;		TG.VRota.y = 0;		TG.VRota.z = 45;	TGF.VRota.x = 0;	TGF.VRota.y = 0;	TGF.VRota.z = 45;
	TG.VScal.x = 1;		TG.VScal.y = 1;		TG.VScal.z = 1;	TGF.VScal.x = 1;	TGF.VScal.y = 1;	TGF.VScal.z = 1;

	transX = false;		transY = false;		transZ = false;
	GTMatrix= glm::mat4(1.0);		// Inicialitzar a identitat

// Entorn VGI: Variables de control per les opcions de menú Ocultacions
	front_faces = true;	test_vis = false;	oculta = true;		back_line = false;

// Entorn VGI: Variables de control del menú Iluminació		
	ilumina = SUAU;			ifixe = true;					ilum2sides = false;
// Reflexions actives: Ambient [1], Difusa [2] i Especular [3]. No actives: Emission [0]. 
	sw_material[0] = false;			sw_material[1] = true;			sw_material[2] = true;			sw_material[3] = true;	sw_material[4] = true;
	sw_material_old[0] = false;		sw_material_old[1] = true;		sw_material_old[2] = true;		sw_material_old[3] = true;	sw_material_old[4] = true;
	textura = false;				t_textura = CAP;				textura_map = true;
	for (i = 0; i < NUM_MAX_TEXTURES; i++) texturesID[i] = 0;
	tFlag_invert_Y = false;

// Entorn VGI: Variables de control del menú Llums
// Entorn VGI: Inicialització variables Llums
	llum_ambient = true;
	llum.posicio.x = 100.0;			llum.posicio.y = 100.0;			llum.posicio.z = 100.0;		llum.posicio.w = 1.0;
	llum.difusa.r = 1.0f;			llum.difusa.g = 1.0f;			llum.difusa.b = 1.0f;		llum.difusa.a = 1.0f;
	llum.especular.r = 1.0f;		llum.especular.g = 1.0f;		llum.especular.b = 1.0f;	llum.especular.a = 1.0f;
	llum.atenuacio.a = 0.0;		llum.atenuacio.b = 0.0;		llum.atenuacio.c = 1.0;
	llum.restringida = false;
	llum.spotdirection.x = 0.0; llum.spotdirection.y = 0.0;	llum.spotdirection.z = -1.0;
	llum.spotcoscutoff = cos(25.0 * PI / 180);		llum.spotexponent = 1.0;
	llum.encesa = true;

// Entorn VGI: Variables de control del menú Shaders
	shader = CAP_SHADER;	shader_programID = 0;	
	shaderLighting.releaseAllShaders();
	// Càrrega Shader de Gouraud
	shader_programID = 0;
	fprintf(stderr, "phong_shdrML: \n"); 
	if (!shader_programID) shader_programID = shaderLighting.loadFileShaders(".\\shaders\\phong_shdrML.vert", ".\\shaders\\phong_shdrML.frag");
	shader = PHONG_SHADER;

	customShader.releaseAllShaders();
	customShaderID = customShader.loadFileShaders(".\\shaders\\customShader.vert", ".\\shaders\\customShader.frag");

	shadowShader.releaseAllShaders();
	shadowShaderID = shadowShader.loadFileShaders(".\\shaders\\shadowMapping.vert", ".\\shaders\\shadowMapping.frag");

// Càrrega SHADERS
// Càrrega Shader Eixos
	fprintf(stderr, "Eixos: \n");
	if (!eixos_programID) eixos_programID = shaderEixos.loadFileShaders(".\\shaders\\eixos.VERT", ".\\shaders\\eixos.FRAG");

// Càrrega Shader Skybox
	fprintf(stderr, "SkyBox: \n");
	if (!skC_programID) skC_programID = shader_SkyBoxC.loadFileShaders(".\\shaders\\skybox.VERT", ".\\shaders\\skybox.FRAG");

// Càrrega VAO Skybox Cube
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

// Entorn VGI: Variables que controlen paràmetres visualització: Mides finestra Windows i PV
	w = 640;			h = 480;			// Mides de la finestra Windows (w-amplada,h-alçada)
	width_old = 640;	height_old = 480;	// Mides de la resolució actual de la pantalla (finestra Windows)
	w_old = 640;		h_old = 480;		// Mides de la finestra Windows (w-amplada,h-alçada) per restaurar Finestra des de fullscreen
	OPV.R = cam_Esferica[0];	OPV.alfa = cam_Esferica[1];		OPV.beta = cam_Esferica[2];		// Origen PV en esfèriques
	//OPV.R = 15.0;		OPV.alfa = 0.0;		OPV.beta = 0.0;										// Origen PV en esfèriques
	Vis_Polar = POLARZ;	oPolars = -1;

// Entorn VGI: Color de fons i de l'objecte
	fonsR = true;		fonsG = true;		fonsB = true;
	c_fons.r = clear_colorB.x;		c_fons.g = clear_colorB.y;		c_fons.b = clear_colorB.z;			c_fons.b = clear_colorB.w;
	sw_color = false;
	col_obj.r = clear_colorO.x;	col_obj.g = clear_colorO.y;	col_obj.b = clear_colorO.z;		col_obj.a = clear_colorO.w;

// Entorn VGI: Objecte OBJ
	ObOBJ = NULL;		vao_OBJ.vaoId = 0;		vao_OBJ.vboId = 0;		vao_OBJ.nVertexs = 0;

// Entorn VGI: OBJECTE --> Corba B-Spline i Bezier
	npts_T = 0;
	for (i = 0; i < MAX_PATCH_CORBA; i = i++)
	{	PC_t[i].x = 0.0;
		PC_t[i].y = 0.0;
		PC_t[i].z = 0.0;
	}

	pas_CS = PAS_BSPLINE;
	sw_Punts_Control = false;

// TRIEDRE DE FRENET / DARBOUX: VT: vector Tangent, VNP: Vector Normal Principal, VBN: vector BiNormal
	dibuixa_TriedreFrenet = false;		dibuixa_TriedreDarboux = false;
	VT = { 0.0, 0.0, 1.0 };		VNP = { 1.0, 0.0, 0.0 };	VBN = { 0.0, 1.0, 0.0 };

// Entorn VGI: Variables del Timer
	t = 0;			anima = false;

// Entorn VGI: Variables de l'objecte FRACTAL
	t_fractal = CAP;	soroll = 'C';
	pas = 64;			pas_ini = 64;
	sw_il = true;		palcolFractal = false;

// Entorn VGI: Altres variables
	mida = 1.0;			nom = "";		buffer = "";

	mapaModel = ::new COBJModel;
	mapaModel->netejaTextures_OBJ();
	mapaModel->netejaVAOList_OBJ();
	mapaModel->LoadModel(".\\modelos\\ejemplo.obj");

	mapa = GameObject(mapaModel);

	initVAOList();	// Inicialtzar llista de VAO'S.
}


void InitAPI()
{
// Vendor, Renderer, Version, Shading Laguage Version i Extensions suportades per la placa gràfica gravades en fitxer extensions.txt
	std::string nomf = "extensions.txt";
	char const* nomExt = "";
	const char* nomfitxer;
	nomfitxer = nomf.c_str();	// Conversió tipus string --> char *
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
				//fprintf(stderr, "\n");				// Displaiar <cr> per pantalla després extensions
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

void OnSize(GLFWwindow* window, int width, int height)
{
	w = width;	h = height;
}

void dibuixa_Escena() {

	//glUseProgram(shader_programID);

//	Dibuix SkyBox Cúbic.

	if (SkyBoxCube) dibuixa_Skybox(skC_programID, cubemapTexture, Vis_Polar, ProjectionMatrix, ViewMatrix);

	//	Dibuix Coordenades Món i Reixes.
	dibuixa_Eixos(eixos_programID, eixos, eixos_Id, grid, hgrid, ProjectionMatrix, ViewMatrix);

	// Escalat d'objectes, per adequar-los a les vistes ortogràfiques (Pràctica 2)
	//	GTMatrix = glm::scale();

	//	Dibuix geometria de l'escena amb comandes GL.




}

// OnPaint: Funció de dibuix i visualització en frame buffer del frame
void OnPaint(GLFWwindow* window)
{
	// TODO: Agregue aquí su código de controlador de mensajes
	GLdouble vpv[3] = { 0.0, 0.0, 1.0 };

	// Entorn VGI.ImGui: Menú ImGui condicionat al color de fons
	if ((c_fons.r < 0.5) || (c_fons.g < 0.5) || (c_fons.b < 0.5))
		ImGui::StyleColorsLight();
	else ImGui::StyleColorsDark();

	// Entorn VGI: PROJECCIÓ PERSPECTIVA
	glDisable(GL_SCISSOR_TEST);		// Desactivació del retall de pantalla

	// Entorn VGI: Activar shader Visualització Escena


	//glm::vec4 objectDiffuseColor(0.6f, 0.4f, 0.3f, 1.0f); // Marrón / Gris
	//glUniform3fv(glGetUniformLocation(customShaderID, "material.diffuse"), 1, &objectDiffuseColor[0]);

	CColor red;
	red.r = 0;
	red.b = 0;
	red.g = 0;
	red.a = 1;


	frameTimer += deltaTime;

	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(frameTimer * 60), glm::vec3(0, 0, 1));

	mapa.transform(glm::vec3(0, 0, 0), rot, glm::vec3(1, 1, 1));

	//mapa.dibuixarObjecte(customShaderID, red, sw_material, ViewMatrix);

	dibuixa_Escena();
}

// Skybox
void OnVistaSkyBox()
{
// Càrrega Shader Skybox
	if (!skC_programID) skC_programID = shader_SkyBoxC.loadFileShaders(".\\shaders\\skybox.VERT", ".\\shaders\\skybox.FRAG");

// Càrrega VAO Skybox Cube
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
			//Picking objects JAVI AQUÍ
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
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	double modul = 0;
	GLdouble vdir[3] = { 0, 0, 0 };
	CSize gir = { 0,0 }, girn = { 0,0 }, girT = { 0,0 }, zoomincr = { 0,0 };

	//ROTACIÓ
	if (m_ButoEAvall && mobil && projeccio != CAP)
	{
		gir.cx = m_PosEAvall.x - xpos;		gir.cy = m_PosEAvall.y - ypos;
		m_PosEAvall.x = xpos;				m_PosEAvall.y = ypos;
		OPV.beta = OPV.beta - gir.cx / 2.0;
		OPV.alfa = OPV.alfa + gir.cy / 2.0;

		// Entorn VGI: Control per evitar el creixement desmesurat dels angles.
		while (OPV.alfa >= 360)		OPV.alfa = OPV.alfa - 360.0;
		while (OPV.alfa < 0)		OPV.alfa = OPV.alfa + 360.0;
		while (OPV.beta >= 360)		OPV.beta = OPV.beta - 360.0;
		while (OPV.beta < 0)		OPV.beta = OPV.beta + 360.0;
	}

	
	//ZOOM
	if (m_ButoDAvall && zzoom && (projeccio != CAP))
	{
		zoomincr.cx = m_PosDAvall.x - xpos;		zoomincr.cy = m_PosDAvall.y - ypos;
		long int incr = zoomincr.cy / 1.0;

		OPV.R = OPV.R + incr;
		if (OPV.R < p_near) OPV.R = p_near;
		if (OPV.R > p_far) OPV.R = p_far;

		m_PosDAvall.x = xpos;				m_PosDAvall.y = ypos;
	}
}

void OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS)
		return;

	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard)
		return;

	switch (key) {
	case GLFW_KEY_F:
		OnFull_Screen(primary, window);
		break;
	default:
		break;
	}
}

// Entorn VGI. OnFull_Screen: Funció per a pantalla completa
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


//--------------------- A PARTIR D'AQUÍ EL NOSTRE CODI
void menu() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Render();
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

	// Llegir resolució actual de pantalla
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


	glEnable(GL_FRAMEBUFFER_SRGB); //NO WAY ESTO ARREGLA LOS COLORES WTF OSEA HOLY SHIT


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
		if (!shader_programID) shader_programID = shaderLighting.loadFileShaders(".\\shaders\\gouraud_shdrML.vert", ".\\shaders\\gouraud_shdrML.frag");
		if (!eixos_Id) eixos_Id = deixos(); // Funció que defineix els Eixos Coordenades Món com un VAO.
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


	glGenTextures(1, &depthMap); //textura depth
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	
	glGenFramebuffers(1, &depthMapFB);  //framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "ERROR: Depth framebuffer no completo\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {  
		now = glfwGetTime();
		delta = now - previous;
		previous = now;
		deltaTime = delta;


		// Poll for and process events
		glfwPollEvents();

		menu();





		////////////////////// VARIABLES ///////////////////

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Este primer glClear es un poco redundante

		glm::vec3 lightDir(1, 1, 1);
		glm::vec3 lightColor(1, 1, 1);
		float ambient = 0.1;

		float boxSize = 10;

		////////////////////// VARIABLES ///////////////////
		float near_plane = 1.0f, far_plane = 100.0f;
		glm::mat4 lightProjection = glm::ortho(-boxSize, boxSize, -boxSize, boxSize, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightDir * 10.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;


		////////////////////// RENDERIZAR ESCENA SOMBRAS ///////////////////
		glUseProgram(shadowShaderID);
		glUniformMatrix4fv(glGetUniformLocation(shadowShaderID, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);

		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFB);
		glClear(GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_FRONT); 


		mapa.dibuixarObjecte(shadowShaderID, ViewMatrix);

		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 


		////////////////////// DIBUJAR ESCENA NORMAL Y TAL ///////////////////
		glViewport(0, 0, w, h);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(customShaderID);

		glUniform1f(glGetUniformLocation(customShaderID, "ambientIntensity"), GLfloat(ambient));
		glUniform3fv(glGetUniformLocation(customShaderID, "lightDirection"), 1, &lightDir[0]);
		glUniform3fv(glGetUniformLocation(customShaderID, "lightColor"), 1, &lightColor[0]);

		ProjectionMatrix = Projeccio_Perspectiva(customShaderID, 0, 0, w, h, OPV.R);
		ViewMatrix = Vista_Esferica(customShaderID, OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
			front_faces, oculta, test_vis, back_line,
			'c', true, &llum, true, false,
			eixos, grid, hgrid);

		glUniformMatrix4fv(glGetUniformLocation(customShaderID, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(customShaderID, "shadowMap"), 0);

		mapa.dibuixarObjecte(customShaderID, ViewMatrix);


		//OnPaint(window);

		dibuixa_Skybox(skC_programID, cubemapTexture, Vis_Polar, ProjectionMatrix, ViewMatrix);

		//	Dibuix Coordenades Món i Reixes.
		dibuixa_Eixos(eixos_programID, eixos, eixos_Id, grid, hgrid, ProjectionMatrix, ViewMatrix);
		

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// Entorn VGI: Activa la finestra actual
		glfwMakeContextCurrent(window);

		// Entorn VGI: Transferència del buffer OpenGL a buffer de pantalla
		glfwSwapBuffers(window);
    }

	// Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

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