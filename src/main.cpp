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
	fullscreen = false;
	pan = false;
	eixos = true;	eixos_programID = 0;  eixos_Id = 0;
	sw_grid = false;
	grid.x = false;	grid.y = false;		grid.z = false;		grid.w = false;
	hgrid.x = 0.0;	hgrid.y = 0.0;		hgrid.z = 0.0;		hgrid.w = 0.0;

// Entorn VGI: Variables opció Vista->Pan
	fact_pan = 1;
	tr_cpv.x = 0;	tr_cpv.y = 0;	tr_cpv.z = 0;		tr_cpvF.x = 0;	tr_cpvF.y = 0;	tr_cpvF.z = 0;

// Entorn VGI: Variables de control per les opcions de menú Projecció, Objecte
	projeccio = PERSPECT;	// projeccio = PERSPECT;
	ProjectionMatrix = glm::mat4(1.0);	// Inicialitzar a identitat
	objecte = CAP;		// objecte = TETERA;

// Entorn VGI: Variables de control Skybox Cube
	SkyBoxCube = false;		skC_programID = 0;
	skC_VAOID.vaoId = 0;	skC_VAOID.vboId = 0;	skC_VAOID.nVertexs = 0;
	cubemapTexture = 0;

// Entorn VGI: Variables de control del menú Transforma
	transf = false;		trasl = false;		rota = false;		escal = false;
	fact_Tras = 1;		fact_Rota = 90;
	TG.VTras.x = 0.0;	TG.VTras.y = 0.0;	TG.VTras.z = 0;	TGF.VTras.x = 0.0;	TGF.VTras.y = 0.0;	TGF.VTras.z = 0;
	TG.VRota.x = 0;		TG.VRota.y = 0;		TG.VRota.z = 0;	TGF.VRota.x = 0;	TGF.VRota.y = 0;	TGF.VRota.z = 0;
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
	llum_ambient = true;		llumGL[0].encesa = true;
	for (i = 1; i < NUM_MAX_LLUMS; i++) llumGL[i].encesa = false;
	for (i = 0; i < NUM_MAX_LLUMS; i++) {
		llumGL[i].posicio.x = 0.0;		llumGL[i].posicio.y = 0.0;		llumGL[i].posicio.z = 0.0;		llumGL[i].posicio.w = 1.0;
		llumGL[i].difusa.r = 1.0f;		llumGL[i].difusa.g = 1.0f;		llumGL[i].difusa.b = 1.0f;		llumGL[i].difusa.a = 1.0f;
		llumGL[i].especular.r = 1.0f;	llumGL[i].especular.g = 1.0f;	llumGL[i].especular.b = 1.0f;	llumGL[i].especular.a = 1.0f;
		llumGL[i].atenuacio.a = 0.0;	llumGL[i].atenuacio.b = 0.0;	llumGL[i].atenuacio.c = 1.0;
		llumGL[i].restringida = false;
		llumGL[i].spotdirection.x = 0.0;	llumGL[i].spotdirection.y = 0.0;	llumGL[i].spotdirection.z = 0.0; llumGL[i].spotdirection.w = 0.0;
		llumGL[i].spotcoscutoff = 0.0;		llumGL[i].spotexponent = 0.0;
	}

// ---------------- LLUM #0 - (+Z) no restringida, amb atenuació constant (a,b,c) = (0,0,1)
// Posició de la font de llum (x,y,z)=(0,200,0):
	llumGL[0].posicio.x = 0.0;			llumGL[0].posicio.y = 0.0;			llumGL[0].posicio.z = 200.0;	llumGL[0].posicio.w = 1.0;

// Intensitats difusa i especular de la font de llum (r,g,b) = (1,1,1):
	llumGL[0].difusa.r = 1.0f;			llumGL[0].difusa.g = 1.0f;			llumGL[0].difusa.b = 1.0f;		llumGL[0].difusa.a = 1.0f;
	llumGL[0].especular.r = 1.0f;		llumGL[0].especular.g = 1.0f;		llumGL[0].especular.b = 1.0f;	llumGL[0].especular.a = 1.0f;

// Coeficients factor atenuació f_att=1/(ad2+bd+c). Llum sense atenuació per distància (a,b,c)=(0,0,1):
	llumGL[0].atenuacio.a = 0.0;		llumGL[0].atenuacio.b = 0.0;		llumGL[0].atenuacio.c = 1.0;

// Paràmetres font de llum restringida:
	llumGL[0].restringida = false;
	llumGL[0].spotdirection.x = 0.0;	llumGL[0].spotdirection.y = 0.0;	llumGL[0].spotdirection.z = -1.0;
	llumGL[0].spotcoscutoff = cos(25.0 * PI / 180);		llumGL[0].spotexponent = 1.0;		// llumGL[0].spotexponent = 45.0; Model de Warn (10, 500)

// Activació font de llum: ENCESA
	llumGL[0].encesa = true;

// ---------------- LLUM #1 - (+X) no restringida, amb atenuació constant (a,b,c) = (0,0,1)
// Posició de la font de llum (x,y,z)=(75,0,0):
	llumGL[1].posicio.x = 75.0;			llumGL[1].posicio.y = 0.0;			llumGL[1].posicio.z = 0.0;		llumGL[1].posicio.w = 1.0;

// Intensitats difusa i especular de la font de llum (r,g,b) = (1,1,1):
	llumGL[1].difusa.r = 1.0f;			llumGL[1].difusa.g = 1.0f;			llumGL[1].difusa.b = 1.0f;		llumGL[1].difusa.a = 1.0f;
	llumGL[1].especular.r = 1.0f;		llumGL[1].especular.g = 1.0f;		llumGL[1].especular.b = 1.0f;	llumGL[1].especular.a = 1.0f;

// Coeficients factor atenuació f_att=1/(ad2+bd+c). Llum sense atenuació per distància (a,b,c)=(0,0,1):
	llumGL[1].atenuacio.a = 0.0;		llumGL[1].atenuacio.b = 0.0;		llumGL[1].atenuacio.c = 1.0;

// Paràmetres font de llum restringida:
	llumGL[1].restringida = false;
	llumGL[1].spotdirection.x = 0.0;	llumGL[1].spotdirection.y = 0.0;	llumGL[1].spotdirection.z = 0.0;
	llumGL[1].spotcoscutoff = 0.0;		llumGL[1].spotexponent = 0.0;

// Activació font de llum: APAGADA
	llumGL[1].encesa = false;

// ---------------- LLUM #2 - (+Y) no restringida, amb atenuació constant (a,b,c) = (0,0,1)
// Posició de la font de llum (x,y,z)=(0,75,0):
	llumGL[2].posicio.x = 0.0;			llumGL[2].posicio.y = 75.0;			llumGL[2].posicio.z = 0.0;		llumGL[2].posicio.w = 1.0;

// Intensitats difusa i especular de la font de llum (r,g,b) = (1,1,1):
	llumGL[2].difusa.r = 1.0f;			llumGL[2].difusa.g = 1.0f;			llumGL[2].difusa.b = 1.0f;		llumGL[2].difusa.a = 1.0f;
	llumGL[2].especular.r = 1.0f;		llumGL[2].especular.b = 1.0f;		llumGL[2].especular.b = 1.0f;	llumGL[2].especular.a = 1.0f;

// Coeficients factor atenuació f_att=1/(ad2+bd+c). Llum amb atenuació per distància (a,b,c)=(0,0.025,1):
	llumGL[2].atenuacio.a = 0.0;		llumGL[2].atenuacio.b = 0.0;		llumGL[2].atenuacio.c = 1.0;

// Paràmetres font de llum restringida:
	llumGL[2].restringida = false;
	llumGL[2].spotdirection.x = 0.0;	llumGL[2].spotdirection.y = -1.0;	llumGL[2].spotdirection.z = 0.0;
	llumGL[2].spotcoscutoff = cos(2.5 * PI / 180);							llumGL[2].spotexponent = 1.0;

// Activació font de llum: APAGADA
	llumGL[2].encesa = false;

// ---------------- LLUM #3 - (Y=X), restringida amb 25 graus obertura i exponent 45, amb atenuació constant (a,b,c) = (0,0,1)
// Posició de la font de llum (x,y,z)=(75,75,75):
	llumGL[3].posicio.x = 75.0;			llumGL[3].posicio.y = 75.0;			llumGL[3].posicio.z = 75.0;		llumGL[3].posicio.w = 1.0;

// Intensitats difusa i especular de la font de llum (r,g,b) = (0,1,0):
	llumGL[3].difusa.r = 0.0f;			llumGL[2].difusa.g = 1.0f;			llumGL[3].difusa.b = 0.0f;		llumGL[3].difusa.a = 1.0f;
	llumGL[3].especular.r = 0.0f;		llumGL[2].especular.g = 1.0f;		llumGL[3].especular.b = 0.0f;	llumGL[3].especular.a = 1.0f;

// Coeficients factor atenuació f_att=1/(ad2+bd+c). Llum sense atenuació per distància (a,b,c)=(0,0,1):
	llumGL[3].atenuacio.a = 0.0;		llumGL[3].atenuacio.b = 0.0;		llumGL[3].atenuacio.c = 1.0;

// Paràmetres font de llum restringida:
	llumGL[3].restringida = true;
	llumGL[3].spotdirection.x = -1.0;	llumGL[3].spotdirection.y = -1.0;	llumGL[3].spotdirection.z = -1.0;
	llumGL[3].spotcoscutoff = cos(25.0 * PI / 180);							llumGL[3].spotexponent = 45.0;

// Activació font de llum: APAGADA
	llumGL[3].encesa = false;

// ---------------- LLUM #4 - (-Z), no restringida, amb atenuació constant (a,b,c) = (0,0,1)
// Posició de la font de llum (x,y,z)=(0,0,-75):
	llumGL[4].posicio.x = 0.0;			llumGL[4].posicio.y = 0.0;			llumGL[4].posicio.z = -75.0;	llumGL[4].posicio.w = 1.0;

// Intensitats difusa i especular de la font de llum (r,g,b) = (1,1,1):
	llumGL[4].difusa.r = 1.0f;			llumGL[4].difusa.g = 1.0f;			llumGL[4].difusa.b = 1.0f;		llumGL[4].difusa.a = 1.0f;
	llumGL[4].especular.r = 1.0f;		llumGL[4].especular.g = 1.0f;		llumGL[4].especular.b = 1.0f;	llumGL[4].especular.a = 1.0f;

// Coeficients factor atenuació f_att=1/(ad2+bd+c). Llum sense atenuació per distància (a,b,c)=(0,0,1):
	llumGL[4].atenuacio.a = 0.0;		llumGL[4].atenuacio.b = 0.0;		llumGL[4].atenuacio.c = 1.0;

// Paràmetres font de llum restringida:
	llumGL[4].restringida = false;
	llumGL[4].spotdirection.x = 0.0;	llumGL[4].spotdirection.y = 0.0;	llumGL[4].spotdirection.z = -1.0;
	llumGL[4].spotcoscutoff = cos(5 * PI / 180);							llumGL[4].spotexponent = 30.0;

// Activació font de llum: APAGADA
	llumGL[4].encesa = false;

// ---------------- LLUM #5 - (-Z), direccional, no restringida, amb atenuació constant (a,b,c) = (0,0,1)
// Vector de la font de llum direccional (x,y,z)=(-1,-1,-1):
	llumGL[5].posicio.x = -1.0;			llumGL[5].posicio.y = -1.0;			llumGL[5].posicio.z = -1.0;		llumGL[5].posicio.w = 0.0;

// Intensitats difusa i especular de la font de llum (r,g,b) = (1,0,0):
	llumGL[5].difusa.r = 1.0f;			llumGL[5].difusa.g = 0.0f;			llumGL[5].difusa.b = 0.0f;		llumGL[5].difusa.a = 1.0f;
	llumGL[5].especular.r = 1.0f;		llumGL[5].especular.g = 0.0f;		llumGL[5].especular.b = 0.0f;	llumGL[5].especular.a = 1.0f;

// Coeficients factor atenuació f_att=1/(ad2+bd+c). Llum sense atenuació per distància (a,b,c)=(0,0,1):
	llumGL[5].atenuacio.a = 0.0;		llumGL[5].atenuacio.b = 0.0;		llumGL[5].atenuacio.c = 1.0;

// Paràmetres font de llum restringida:
	llumGL[5].restringida = false;
	llumGL[5].spotdirection.x = 0.0;	llumGL[5].spotdirection.y = 0.0;	llumGL[5].spotdirection.z = 0.0;
	llumGL[5].spotcoscutoff = 0.0;		llumGL[5].spotexponent = 0.0;

// Activació font de llum: APAGADA
	llumGL[5].encesa = false;

// ---------------- LLUM #6 - Llum Vaixell, configurada a la funció vaixell() en escena.cpp.
// Posició de la font de llum (x,y,z)=(-75,75,75):
	llumGL[6].posicio.x = -75.0;		llumGL[6].posicio.y = 75.0;			llumGL[6].posicio.z = 75.0;		llumGL[6].posicio.w = 1.0;

// Intensitats difusa i especular de la font de llum (r,g,b) = (1,1,1):
	llumGL[6].difusa.r = 1.0f;			llumGL[6].difusa.g = 1.0f;			llumGL[6].difusa.b = 1.0f;		llumGL[6].difusa.a = 1.0f;
	llumGL[6].especular.r = 1.0f;		llumGL[6].especular.g = 1.0f;		llumGL[6].especular.b = 1.0f;	llumGL[6].especular.a = 1;

// Coeficients factor atenuació f_att=1/(ad2+bd+c). Llum sense atenuació per distància (a,b,c)=(0,0,1):
	llumGL[6].atenuacio.a = 0.0;		llumGL[6].atenuacio.b = 0.0;		llumGL[6].atenuacio.c = 1.0;

// Paràmetres font de llum restringida:
	llumGL[6].restringida = false;
	llumGL[6].spotdirection.x = 0.0;	llumGL[6].spotdirection.y = 0.0;	llumGL[6].spotdirection.z = 0.0;
	llumGL[6].spotcoscutoff = 0.0;		llumGL[6].spotexponent = 0.0;

// Activació font de llum: APAGADA
	llumGL[6].encesa = false;

// ---------------- LLUM #7 - Llum Far, configurada a la funció faro() en escena.cpp.
// Posició de la font de llum (x,y,z)=(75,75,-75). Cap posició definida, definida en funció faro() en escena.cpp:
	llumGL[7].posicio.x = 75.0;			llumGL[7].posicio.y = 75.0;			llumGL[7].posicio.z = -75.0;	llumGL[7].posicio.w = 1.0;

// Intensitats difusa i especular de la font de llum (r,g,b) = (1,1,1):
	llumGL[7].difusa.r = 1.0f;			llumGL[7].difusa.g = 1.0f;			llumGL[7].difusa.b = 1.0f;		llumGL[7].difusa.a = 1.0f;
	llumGL[7].especular.r = 1.0f;		llumGL[7].especular.g = 1.0f;		llumGL[7].especular.b = 1.0f;	llumGL[7].especular.a = 1;

// Coeficients factor atenuació f_att=1/(ad2+bd+c). Llum sense atenuació per distància (a,b,c)=(0,0,1):
	llumGL[7].atenuacio.a = 0.0;		llumGL[7].atenuacio.b = 0.0;		llumGL[7].atenuacio.c = 1.0;

// Paràmetres font de llum restringida:
	llumGL[7].restringida = false;
	llumGL[7].spotdirection.x = 0.0;	llumGL[7].spotdirection.y = 0.0;	llumGL[7].spotdirection.z = 0.0;
	llumGL[7].spotcoscutoff = 0.0;		llumGL[7].spotexponent = 0.0;

// Activació font de llum: APAGADA
	llumGL[7].encesa = false;
// ---------------- FI DEFINICIÓ LLUMS

// Entorn VGI: Variables de control del menú Shaders
	shader = CAP_SHADER;	shader_programID = 0;	
	shaderLighting.releaseAllShaders();
	// Càrrega Shader de Gouraud
	shader_programID = 0;
	fprintf(stderr, "Gouraud_shdrML: \n"); 
	if (!shader_programID) shader_programID = shaderLighting.loadFileShaders(".\\shaders\\gouraud_shdrML.vert", ".\\shaders\\gouraud_shdrML.frag");
	shader = GOURAUD_SHADER;

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
	glUseProgram(shader_programID);

	// Entorn VGI: Definició de Viewport, Projecció i Càmara
	ProjectionMatrix = Projeccio_Perspectiva(shader_programID, 0, 0, w, h, OPV.R);

	// Entorn VGI: Definició de la càmera.
	if (camera == CAM_ESFERICA) {
		n[0] = 0;		n[1] = 0;		n[2] = 0;
		ViewMatrix = Vista_Esferica(shader_programID, OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
			front_faces, oculta, test_vis, back_line,
			ilumina, llum_ambient, llumGL, true, ilum2sides,
			eixos, grid, hgrid);
	}

	// Entorn VGI: Dibuix de l'Objecte o l'Escena
	configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes.
	dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
}

// configura_Escena: Funcio que configura els parametres de Model i dibuixa les
//                   primitives OpenGL dins classe Model
void configura_Escena() {

// Aplicar Transformacions Geometriques segons persiana Transformacio i Quaternions
	GTMatrix = instancia(transf, TG, TGF);
}

// dibuixa_Escena: Funcio que crida al dibuix dels diferents elements de l'escana
void dibuixa_Escena() {

	//glUseProgram(shader_programID);

//	Dibuix SkyBox Cúbic.
	if (SkyBoxCube) dibuixa_Skybox(skC_programID, cubemapTexture, Vis_Polar, ProjectionMatrix, ViewMatrix);

//	Dibuix Coordenades Món i Reixes.
	dibuixa_Eixos(eixos_programID, eixos, eixos_Id, grid, hgrid, ProjectionMatrix, ViewMatrix);

//	Dibuix geometria de l'escena amb comandes GL.
	dibuixa_EscenaGL(shader_programID, eixos, eixos_Id, grid, hgrid, objecte, col_obj, sw_material,
		textura, texturesID, textura_map, tFlag_invert_Y,
		npts_T, PC_t, pas_CS, sw_Punts_Control, dibuixa_TriedreFrenet,
		ObOBJ,				// Classe de l'objecte OBJ que conté els VAO's
		ViewMatrix, GTMatrix);
}

/* ------------------------------------------------------------------------- */
/*                            MENUS ImGui                                    */
/* ------------------------------------------------------------------------- */

// Entorn VGI: Dibuixa el menú principal ImGui que controla colors i permet activar altres menús:
//		- menú standard Imgui [show_demo_window()]
//		- menú propi de l'aplicació EntornVGI [show_EntornVGI_window()]
void draw_Menu_ImGui()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 1. Show the EntornVGI window. Finestra amb totes les opcions de l'aplicació.
	if (show_EntornVGI_window)
		ShowEntornVGIWindow(&show_EntornVGI_window); //ShowEntornVGIWindow(&show_EntornVGI_window);

	// 3. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;
		static float PV[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		ImGui::Begin("Status Menu");                          // Create a window called "Status Menu" and append into it.

		ImGui::Text("Finestres EntornVGI: (Simplificat per Gerard ;D)");               // Display some text (you can use a format strings too)
		ImGui::SameLine();
		ImGui::Checkbox("EntornVGI Window", &show_EntornVGI_window);
		ImGui::Separator();
		ImGui::Spacing();

		// Transformació PV de Coord. esfèriques (R,anglev,angleh) --> Coord. cartesianes (PVx,PVy,PVz)
		cam_Esferica[0] = OPV.R;	cam_Esferica[1] = OPV.alfa; cam_Esferica[2] = OPV.beta;
		if (Vis_Polar == POLARZ)
		{
			PV[0] = OPV.R * cos(OPV.beta * PI / 180) * cos(OPV.alfa * PI / 180);
			PV[1] = OPV.R * sin(OPV.beta * PI / 180) * cos(OPV.alfa * PI / 180);
			PV[2] = OPV.R * sin(OPV.alfa * PI / 180);
		}

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("CAMERA:");
		ImGui::PopStyleColor();
		
		ImGui::InputFloat3("Esferiques (R,alfa,beta)", cam_Esferica);
		ImGui::InputFloat3("Cartesianes (PVx,PVy,PVz)", PV);
		
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("COLORS:");
		ImGui::PopStyleColor();
		ImGui::ColorEdit3("Color de Fons", (float*)&clear_colorB); // Edit 3 floats representing a background color
		ImGui::ColorEdit3("Color d'Objecte", (float*)&clear_colorO); // Edit 3 floats representing a object color
		c_fons.r = clear_colorB.x;	c_fons.g = clear_colorB.y;	c_fons.b = clear_colorB.z;	c_fons.a = clear_colorB.w;
		col_obj.r = clear_colorO.x;	col_obj.g = clear_colorO.y;	col_obj.b = clear_colorO.z;		col_obj.a = clear_colorO.w;
		ImGui::Separator();


		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("TRANSFORMA:");
		ImGui::PopStyleColor();
		float tras_ImGui[3] = { (float) TG.VTras.x,(float) TG.VTras.y,(float) TG.VTras.z };
		ImGui::InputFloat3("Traslacio (Tx,Ty,Tz)", tras_ImGui);
		float rota_ImGui[3] = { (float) TG.VRota.x,(float) TG.VRota.y,(float) TG.VRota.z };
		ImGui::InputFloat3("Rotacio (Rx,Ry,Rz)", rota_ImGui);
		float scal_ImGui[3] = { (float) TG.VScal.x,(float) TG.VScal.y,(float) TG.VScal.z };
		ImGui::InputFloat3("Escala (Sx, Sy, Sz)", scal_ImGui);

		ImGui::Text("imgui versions: (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
		ImGui::Spacing();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	ImGui::Render();
}

// Entorn VGI: Funció que mostra el menú principal de l'Entorn VGI amb els seus desplegables.
void MostraEntornVGIWindow(bool* p_open)
{
// Exceptionally add an extra assert here for people confused about initial Dear ImGui setup
// Most functions would normally just crash if the context is missing.
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin("EntornVGI Menu", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	// Menu Bar
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Arxius"))
		{
			//IMGUI_DEMO_MARKER("Menu/File");
			ShowArxiusOptions();
			ImGui::EndMenu();
		}
		
		ImGui::EndMenuBar();
	}

// End of ShowEntornVGIWindow()
	ImGui::PopItemWidth();
	ImGui::End();
}

// Entorn VGI: Funció que mostra els menús popUp "Arxius" per a obrir fitxers i el pop Up "About".
void ShowArxiusOptions()
{
	//IMGUI_DEMO_MARKER("Examples/Menu");
	ImGui::MenuItem("(Arxius menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) {}

	if (ImGui::MenuItem("Obrir Fitxer OBJ", "(<Shift>+#)")) {
		nfdchar_t* nomFitxer = NULL;
		nfdresult_t result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

		if (result == NFD_OKAY) {
			puts("Success!");
			puts(nomFitxer);

			objecte = OBJOBJ;	textura = true;		tFlag_invert_Y = false;
			//char* nomfitx = nomfitxer;
			if (ObOBJ == NULL) ObOBJ = ::new COBJModel;
			else { // Si instància ja s'ha utilitzat en un objecte OBJ
				ObOBJ->netejaVAOList_OBJ();		// Netejar VAO, EBO i VBO
				ObOBJ->netejaTextures_OBJ();	// Netejar buffers de textures
				}

			//int error = ObOBJ->LoadModel(nomfitx);			// Carregar objecte OBJ amb textura com a varis VAO's
			int error = ObOBJ->LoadModel(nomFitxer);			// Carregar objecte OBJ amb textura com a varis VAO's

			//	Pas de paràmetres textura al shader
			glUniform1i(glGetUniformLocation(shader_programID, "textur"), textura);
			glUniform1i(glGetUniformLocation(shader_programID, "flag_invert_y"), tFlag_invert_Y);

			free(nomFitxer);
		}
	}

	if (ImGui::MenuItem("Obrir Fractal", "(<Shift>+#)")) {
		nfdchar_t* nomFitxer = NULL;
		nfdresult_t result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

		if (result == NFD_OKAY) {	puts("Success!");
									puts(nomFitxer);
									free(nomFitxer);
								}
		else if (result == NFD_CANCEL) puts("User pressed cancel.");
			else printf("Error: %s\n", NFD_GetError());
		}

	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowArxiusOptions();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "(<Shift>+#))")) {}
	if (ImGui::MenuItem("Save As..")) {}

	ImGui::Separator();
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

// Demonstrate most Dear ImGui features (this is big function!)
// You may execute this function to experiment with the UI and understand what it does.
// You may then search for keywords in the code when you are interested by a specific feature.
void ShowEntornVGIWindow(bool* p_open)
{
	int i = 0; // Variable per a menús de selecció.
	static int selected = -1;

// Exceptionally add an extra assert here for people confused about initial Dear ImGui setup
// Most functions would normally just crash if the context is missing.
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

// Examples Apps(accessible from the "Examples" menu)
	static bool show_app_main_menu_bar = false;
	static bool show_app_documents = false;
	static bool show_app_console = false;
	static bool show_app_log = false;
	static bool show_app_layout = false;
	static bool show_app_property_editor = false;
	static bool show_app_long_text = false;
	static bool show_app_auto_resize = false;
	static bool show_app_constrained_resize = false;
	static bool show_app_simple_overlay = false;
	static bool show_app_fullscreen = false;
	static bool show_app_window_titles = false;
	static bool show_app_custom_rendering = false;

	// Dear ImGui Tools/Apps (accessible from the "Tools" menu)
	static bool show_app_metrics = false;
	static bool show_app_debug_log = false;
	static bool show_app_stack_tool = false;
	static bool show_app_about = false;
	static bool show_app_style_editor = false;

	if (show_app_metrics)
		ImGui::ShowMetricsWindow(&show_app_metrics);
	if (show_app_debug_log)
		ImGui::ShowDebugLogWindow(&show_app_debug_log);
	if (show_app_stack_tool)
		ImGui::ShowStackToolWindow(&show_app_stack_tool);
	if (show_app_about)
		ImGui::ShowAboutWindow(&show_app_about);
	if (show_app_style_editor)
	{
		ImGui::Begin("Dear ImGui Style Editor", &show_app_style_editor);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

// Demonstrate the various window flags. Typically you would just use the default!
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
	if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

// We specify a default position/size in case there's no data in the .ini file.
// We only do it to make the demo applications a little more welcoming, but typically this isn't required.
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	// Main body of the Demo window starts here.
	if (!ImGui::Begin("EntornVGI Menu", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

// Entorn VGI: Menu Bar (Pop Ups desplagables (Arxius, About)
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Arxius"))
		{	
			ShowArxiusOptions();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Text("Finestra de menus d'EntornVGI");
	ImGui::Spacing();

// Entorn VGI: DESPLEGABLES ---------------------------------------------------
	if (ImGui::CollapsingHeader("CAMERA"))
	{
		if (ImGui::Button("Origen Esferica (<Shift>+O)")) OnCameraEsferica();;
		if (ImGui::BeginTable("split", 3))
		{
			ImGui::TableNextColumn(); ImGui::Checkbox("Mobil (<Shift>+M)", &mobil);
			ImGui::TableNextColumn(); ImGui::Checkbox("Zoom? (<Shift>+Z)", &zzoom);
			ImGui::TableNextColumn(); ImGui::Checkbox("Satelit? (<Shift>+T)", &satelit);
			ImGui::EndTable();
		}
		ImGui::Separator();
		ImGui::Spacing();
	}

// DESPLEGABLE VISTA
	//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("VISTA"))
	{
		if (ImGui::BeginTable("split", 2))
		{	
			ImGui::TableNextColumn(); ImGui::Checkbox("Eixos? (<Shift>+E)", &eixos);
			ImGui::TableNextColumn(); ImGui::Checkbox("SkyBox? (<Shift>+S)", &SkyBoxCube);
			ImGui::EndTable();
		}
		ImGui::Spacing();
		ImGui::Separator();

		// Configurar opció Vista -> SkyBox?
		if (SkyBoxCube) OnVistaSkyBox();
							
		ImGui::Spacing();
		ImGui::Separator();
	}

// DESPLEGABLE VISTA
	//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("OBJECTE"))
	{
		// Entorn VGI. Mostrar Opcions desplegable OBJECTES
		//IMGUI_DEMO_MARKER("Widgets/Basic/RadioButton");

		// Using the _simplified_ one-liner Combo() api here
		// See "Combo" section for examples of how to use the more flexible BeginCombo()/EndCombo() api.
		//IMGUI_DEMO_MARKER("Widgets/Basic/Combo");
		const char* items[] = { "Cap(<Shift>+B)", "Cub (<Shift>+C)", "Cub RGB (<Shift>+D)", "Esfera (<Shift>+E)", "Tetera (<Shift>+T)", 
			"Arc (<Shift>+R)", "Matriu Primitives (<Shift>+H)", "Matriu Primitives VAO (<Shift>+V)", "Tie (<Shift>+I)", "Arxiu OBJ", 
			"Bezier (<Shift>+F5)", "B-spline (<Shift>+F6)", "Lemniscata (<Shift>+F7)", "Hermitte (<Shift>+F8)", "Catmull-Rom (<Shift>+F9)" };
		//static int item_current = 0;
		ImGui::Combo(" ", &oObjecte, items, IM_ARRAYSIZE(items));
		ImGui::Spacing();

		// Mostrar fram PARAMETRES CORBES si Objectes Corbes seleccionats
		if ((oObjecte > 9) && (oObjecte < 16)) {
			//IMGUI_DEMO_MARKER("Widgets/Selectables/Single Selection CORBES");
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::SeparatorText("PARAMETRES CORBES:");
			ImGui::PopStyleColor();

			if (ImGui::BeginTable("split", 2))
			{
				ImGui::TableNextColumn(); ImGui::Checkbox("Triedre de Frenet? (<Shift>+F11)", &dibuixa_TriedreFrenet);
				ImGui::TableNextColumn(); ImGui::Checkbox("Triedre de Darboux? (<Shift>+F12)", &dibuixa_TriedreDarboux);
				ImGui::TableNextColumn(); ImGui::Checkbox("Punts de Control? (<Shift>+F10)", &sw_Punts_Control);
				ImGui::EndTable();
			}
		}

// EntornVGI: Variables associades a Pop Up OBJECTE
		bool testA = false;
		int error = 0;

		// Entorn VGI. Gestió opcions desplegable OBJECTES segons el valor de la variable selected
			switch (oObjecte)
			{
			case 0: // Opció OBJECTE Cap
				if (objecte != CAP) OnObjecteCap();
				break;
			case 1: // Opció OBJECTE Cub
				if (objecte != CUB) OnObjecteCub();
				break;
			case 2:	// Opció OBJECTE Cub RGB
				if (objecte != CUB_RGB) OnObjecteCubRGB();
				break;
			case 3: // Opció OBJECTE Esfera
				if (objecte != ESFERA) OnObjecteEsfera();
				break;
			case 4: // Opció OBJECTE Tetera
				if (objecte != TETERA) OnObjecteTetera();
				break;
			case 5: // Opció OBJECTE Arc
				if (objecte != ARC) OnObjecteArc();
				break;
			case 6: // Opció OBJECTE MatrVAOiu Primitives
				if (objecte != MATRIUP) OnObjecteMatriuPrimitives();
				break;
			case 7: // Opció OBJECTE Matriu Primitives VAO
				if (objecte != MATRIUP_VAO) OnObjecteMatriuPrimitivesVAO();
				break;
			case 8: // Opció OBJECTE Tie
				if (objecte != TIE) OnObjecteTie();
				break;
			case 9: // Opció OBJECTE Arxiu OBJ
				if (objecte != OBJOBJ) OnArxiuObrirFitxerObj();
				break;
			case 10: // Opció OBJECTE CORBA BEZIER
				if (objecte != C_BEZIER) OnObjecteCorbaBezier();
				break;
			case 11: // Opció OBJECTE CORBA B-SPLINE
				if (objecte != C_BSPLINE) OnObjecteCorbaBSpline();
				break;
			case 12: // Opció OBJECTE CORBA LEMNISCATA
				if (objecte != C_LEMNISCATA) OnObjecteCorbaLemniscata();
				break;
			case 13: // Opció OBJECTE CORBA HERMITTE
				if (objecte != C_HERMITTE) OnObjecteCorbaHermitte();
				break;
			case 14: // Opció OBJECTE CORBA CATMULL-ROM
				if (objecte != C_CATMULL_ROM) OnObjecteCorbaCatmullRom();
				break;
			default: 
				// Opció per defecte: CAP
				OnObjecteCap();
				break;
			}
	}

	// DESPLEGABLE VISTA
	//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("OCULTACIONS"))
	{
		//IMGUI_DEMO_MARKER("Widgets/Selectables/Single Selection OCULTACIONS");
		if (ImGui::BeginTable("split", 2))
		{	ImGui::TableNextColumn(); ImGui::Checkbox("Test Visibilitat? (<Ctrl>+C)", &test_vis);
			ImGui::TableNextColumn(); ImGui::Checkbox("Z-Buffer? (<Ctrl>+B)", &oculta);
			ImGui::EndTable();
		}
			ImGui::Separator();
		if (ImGui::BeginTable("split", 2))
		{	ImGui::TableNextColumn(); ImGui::Checkbox("Front Faces? (<Ctrl>+D)", &front_faces);
			ImGui::EndTable();
		}
	}

// DESPLEGABLE ILUMINACIO
//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("ILUMINACIO"))
	{
		// Entorn VGI. Mostrar Opcions desplegable TIPUS ILUMINACIO
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("TIPUS ILUMINACIO:");
		ImGui::PopStyleColor();

		
		static ImGuiComboFlags flags = ( 0 && ImGuiComboFlags_PopupAlignLeft && ImGuiComboFlags_NoPreview && ImGuiComboFlags_NoArrowButton);

		//IMGUI_DEMO_MARKER("Widgets/Selectables/Single Selection REFLEXIO MATERIAL");
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("REFLEXIO MATERIAL:");
		ImGui::PopStyleColor();
		ImGui::Checkbox("Material (T) / Color (F) - (<Ctrl>+F10)", &sw_material[4]);
		ImGui::Separator();
		ImGui::Spacing();

		// Pas de color material o color VAO al shader
		glUniform1i(glGetUniformLocation(shader_programID, "sw_material"), sw_material[4]);

		if (ImGui::BeginTable("split", 2))
		{	ImGui::TableNextColumn(); ImGui::Checkbox("Emissio? (<Ctrl>+F6)", &sw_material[0]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Ambient? (<Ctrl>+F7)", &sw_material[1]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Difusa? (<Ctrl>+F8)", &sw_material[2]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Especular? (<Ctrl>+F9)", &sw_material[3]);
			ImGui::EndTable();
		}

		// Pas de propietats de materials al shader
		if (!shader_programID)
		{	glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[0]"), sw_material[0]);
			glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[1]"), sw_material[1]);
			glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[2]"), sw_material[2]);
			glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[3]"), sw_material[3]);
		}

		//IMGUI_DEMO_MARKER("Widgets/Selectables/Single Selection TEXTURES");
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("TEXTURES:");
		ImGui::PopStyleColor();
		ImGui::Checkbox("Textura? (<Ctrl>+I)", &textura);
		ImGui::SameLine();


		static int clickITS = 0;
		if (ImGui::Button("Imatge Textura SOIL (<Ctrl>+J)")) clickITS++;
		// EntornVGI: Si s'ha apretat el botó "Image Textura SOIL"
		if (clickITS)	{
			clickITS = 0;
			// Mostrar diàleg de fitxer i carregar imatge com a textura.
			OnIluminacioTexturaFitxerimatge();
			}

		ImGui::Spacing();
		ImGui::Checkbox("Flag_Invert_Y? (<Ctrl>+K)", &tFlag_invert_Y);
		if ((tFlag_invert_Y) && (!shader_programID)) glUniform1i(glGetUniformLocation(shader_programID, "flag_invert_y"), tFlag_invert_Y);
	}

// DESPLEGABLE LLUMS
//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("LLUMS"))
	{
		ImGui::Checkbox("Llum Ambient? (<Ctrl>+A)", &llum_ambient);
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::BeginTable("split", 2))
			{	ImGui::TableNextColumn(); 
				ImGui::Checkbox("Llum #0 (+Z) (<Ctrl>+0)?", &llumGL[0].encesa);
				ImGui::TableNextColumn();
				ImGui::Checkbox("Llum #1 (+X) (<Ctrl>+1)?", &llumGL[1].encesa);
				ImGui::TableNextColumn();
				ImGui::Checkbox("Llum #2 (+Y) (<Ctrl>+2)?", &llumGL[2].encesa);
				ImGui::TableNextColumn();
				ImGui::Checkbox("Llum #3 (Z=Y=X) (<Ctrl>+3)?", &llumGL[3].encesa);
				ImGui::TableNextColumn();
				ImGui::Checkbox("Llum #4 (-Z)?(<Ctrl>+4)", &llumGL[4].encesa);
				ImGui::TableNextColumn();
				ImGui::Checkbox("Llum #5?(<Ctrl>+5)", &llumGL[5].encesa);
				ImGui::TableNextColumn();
				ImGui::Checkbox("Llum #6?(<Ctrl>+6)", &llumGL[6].encesa);
				ImGui::TableNextColumn();
				ImGui::Checkbox("Llum #7?(<Ctrl>+7)", &llumGL[7].encesa);
				ImGui::EndTable();
			}

		// Activar llum ambient i llums (GL_LIGHT0-GL_LIGTH7)
		if (llum_ambient) OnLlumsLlumAmbient();

		if (llumGL[0].encesa)	OnLlumsLlum0();

		if (llumGL[1].encesa)	OnLlumsLlum1();

		if (llumGL[2].encesa)	OnLlumsLlum2();

		if (llumGL[3].encesa)	OnLlumsLlum3();

		if (llumGL[4].encesa)	OnLlumsLlum4();

		if (llumGL[5].encesa)	OnLlumsLlum5();

		if (llumGL[6].encesa)	OnLlumsLlum6();

		if (llumGL[7].encesa)	OnLlumsLlum7();
	}

// DESPLEGABLE SHADERS
//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("SHADERS"))
	{
		// Combo Boxes are also called "Dropdown" in other systems
		// Expose flags as checkbox for the demo
		static ImGuiComboFlags flagsS = (0 && ImGuiComboFlags_PopupAlignLeft && ImGuiComboFlags_NoPreview && ImGuiComboFlags_NoArrowButton);

		// Using the generic BeginCombo() API, you have full control over how to display the combo contents.
		// (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
		// stored in the object itself, etc.)
		const char* itemsS[] = { "Flat (<Ctrl>+L)", "Gouraud (<Ctrl>+G)", "Phong (<Ctrl>+P)",
			"Carregar fitxers Shader [.vert,.frag] (<Ctrl>+V)" };
		const char* combo_preview_value = itemsS[oShader];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("Tipus de Shader", combo_preview_value, flagsS))
		{	for (int n = 0; n < IM_ARRAYSIZE(itemsS); n++)
			{	const bool is_selected = (oShader == n);
				if (ImGui::Selectable(itemsS[n], is_selected))
					oShader = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		// Entorn VGI. Definició de nou Program que substituirà l'actual
		GLuint newShaderID = 0;
		// Entorn VGI. Gestió opcions desplegable TIPUS ILUMINACIO segons el valor de la variable selected
		switch (oShader)
		{
		case 0: // Opció SHADER Flat
			if (shader != FLAT_SHADER)	OnShaderFlat();
			break;

		case 1:	// Opció SHADER Gouraud
			if (shader != GOURAUD_SHADER)	OnShaderGouraud();
			break;

		case 2:	// Opció SHADER Phong
			if (shader != PHONG_SHADER)	OnShaderPhong();
			break;

		case 3:	// Opció SHADER Carregar fitxers shader (.vert, .frag)
			if (shader != FILE_SHADER) OnShaderLoadFiles();
			break;

		}
	}

// End of ShowDemoWindow()
	ImGui::PopItemWidth();
	ImGui::End();
}

// Obrir fitxer Fractal
void OnArxiuObrirFractal()
{
// TODO: Agregue aquí su código de controlador de comandos

	nfdchar_t* nomFitxer = NULL;
	nfdresult_t result; // = NFD_OpenDialog(NULL, NULL, &nomFitxer);

	// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.MNT)
	result = NFD_OpenDialog(".mnt", NULL, &nomFitxer);

	if (result == NFD_OKAY) {
		puts("Fitxer Fractal Success!");
		puts(nomFitxer);

		objecte = O_FRACTAL;
		// Entorn VGI: TO DO -> Llegir fitxer fractal (nomFitxer) i inicialitzar alçades

		free(nomFitxer);
		}

}

// OnArchivoObrirFitxerObj: Obrir fitxer en format gràfic OBJ
void OnArxiuObrirFitxerObj()
{
// TODO: Agregue aquí su código de controlador de comandos
	nfdchar_t* nomFitxer = NULL;
	nfdresult_t result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

	if (result == NFD_OKAY) {
		// Entorn VGI: Variable de tipus nfdchar_t* 'nomFitxer' conté el nom del fitxer seleccionat
		puts("Success!");
		puts(nomFitxer);

		//if (ObOBJ != NULL) delete ObOBJ;
		objecte = OBJOBJ;	textura = true;		tFlag_invert_Y = false;

		if (ObOBJ == NULL) ObOBJ = ::new COBJModel;
			else {	// Si instància ja s'ha utilitzat en un objecte OBJ
					ObOBJ->netejaVAOList_OBJ();		// Netejar VAO, EBO i VBO
					ObOBJ->netejaTextures_OBJ();	// Netejar buffers de textures
			}

		//int error = ObOBJ->LoadModel(nomfitx);			// Carregar objecte OBJ amb textura com a varis VAO's
		int error = ObOBJ->LoadModel(nomFitxer);			// Carregar objecte OBJ amb textura com a varis VAO's

		//	Pas de paràmetres textura al shader
		glUniform1i(glGetUniformLocation(shader_programID, "textur"), textura);
		glUniform1i(glGetUniformLocation(shader_programID, "flag_invert_y"), tFlag_invert_Y);
		free(nomFitxer);
	}
}

// Obrir fitxer que conté paràmetres Font de Llum (fitxers .lght)
void OnArxiuObrirFitxerFontLlum()
{
// TODO: Agregue aquí su código de controlador de comandos
	nfdchar_t* nomFitxer = NULL;
	nfdresult_t result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

	if (result == NFD_OKAY) {
		// Entorn VGI: Variable de tipus nfdchar_t* 'nomFitxer' conté el nom del fitxer seleccionat
		puts("Success!");
		puts(nomFitxer);
	
		bool err = llegir_FontLlum(nomFitxer); // Llegir Fitxer de Paràmetres Font de Llum
	}
}

// llegir_FontLlum: Llegir fitxer .lght que conté paràmetres de la font de lluym i-èssima. 
// Retorna booleà a TRUE si s'ha fet la lectura correcte, FALSE en cas contrari.
// bool llegir_FontLlum(CString nomf)
bool llegir_FontLlum(char* nomf)
{
	int i, j;
	FILE* fd;

	//	ifstream f("altinicials.dat",ios::in);
	//    f>>i; f>>j;
	if ((fd = fopen(nomf, "rt")) == NULL)
	{
		fprintf(stderr, "%s", "ERROR: File .lght was not opened");
		//LPCWSTR texte1 = reinterpret_cast<LPCWSTR> ("ERROR:");
		//LPCWSTR texte2 = reinterpret_cast<LPCWSTR> ("File .lght was not opened");
		//MessageBox(NULL, texte1, texte2, MB_OK);
		//MessageBox(texte1, texte2, MB_OK);
		//AfxMessageBox("file was not opened");
		return false;
	}
	fscanf(fd, "%i \n", &i);
	if (i < 0 || i >= NUM_MAX_LLUMS) return false;
	else {
		fscanf(fd, "%i \n", &j); // Lectura llum encesa
		if (j == 0) llumGL[i].encesa = false; else llumGL[i].encesa = true;
		fscanf(fd, "%lf %lf %lf %lf\n", &llumGL[i].posicio.x, &llumGL[i].posicio.y, &llumGL[i].posicio.z, &llumGL[i].posicio.w);
		fscanf(fd, "%lf %lf %lf \n", &llumGL[i].difusa.r, &llumGL[i].difusa.g, &llumGL[i].difusa.b);
		fscanf(fd, "%lf %lf %lf \n", &llumGL[i].especular.r, &llumGL[i].especular.g, &llumGL[i].especular.b);
		fscanf(fd, "%lf %lf %lf \n", &llumGL[i].atenuacio.a, &llumGL[i].atenuacio.b, &llumGL[i].atenuacio.c);
		fscanf(fd, "%i \n", &j); // Lectura booleana font de llum restringida.
		if (j == 0) llumGL[i].restringida = false; else llumGL[i].restringida = true;
		fscanf(fd, "%lf %lf %lf \n", &llumGL[i].spotdirection.x, &llumGL[i].spotdirection.y, &llumGL[i].spotdirection.z);
		fscanf(fd, "%f \n", &llumGL[i].spotcoscutoff);
		fscanf(fd, "%f \n", &llumGL[i].spotexponent);
	}
	fclose(fd);

	return true;
}


// Obrir fitxers del SkyBox
void OnArxiuObrirSkybox()
{
// TODO: Agregue aquí su código de controlador de comandos
	nfdchar_t* folderPath = NULL;
	nfdresult_t result = NFD_OpenDialog(NULL, NULL, &folderPath);
	std::vector<std::string> faces;
}

/* --------------------------------------------------------------------------------- */
/*					4. CÀMERA: Esfèrica (Mobil, Zoom, ZoomO, Satelit), Navega, Geode */
/* --------------------------------------------------------------------------------- */
// CÀMERA: Mode Esfèrica (Càmera esfèrica en polars-opció booleana)
void OnCameraEsferica()
{
	OPV.R = 15.0;		OPV.alfa = 0.0;		OPV.beta = 0.0;				// Origen PV en esfèriques
	mobil = true;		zzoom = true;		satelit = false;
	Vis_Polar = POLARZ;
}

// CÀMERA--> ESFERICA: Mobil. Punt de Vista Interactiu (opció booleana)
void OnVistaMobil()
{
	mobil = !mobil;

	if (mobil) {
		transX = false;	transY = false; transZ = false;
	}
}

// CÀMERA--> ESFERICA: Zoom. Zoom Interactiu (opció booleana)
void OnVistaZoom()
{
	zzoom = !zzoom;
// Desactivació de Transformacions Geomètriques via mouse si Zoom activat.
	if (zzoom) {	transX = false;	transY = false;	transZ = false;
					zzoomO = false;
	}
}

// CÀMERA--> ESFERICA: Satèlit. Vista interactiva i animada en que increment de movimen és activat per mouse (opció booleana)
void OnVistaSatelit()
{
	satelit = !satelit;
	if (satelit) {	mobil = true;
					m_EsfeIncEAvall.alfa = 0.0;
					m_EsfeIncEAvall.beta = 0.0;
	}
}

/* -------------------------------------------------------------------------------- */
/*					5. VISTA: Pantalla Completa, Pan i Eixos	                    */
/* -------------------------------------------------------------------------------- */
// VISTA: Mode de Desplaçament horitzontal i vertical per pantalla del Punt de Vista (pan) (opció booleana)


// VISTA: Visualitzar eixos coordenades món (opció booleana)
void OnVistaEixos()
{
	eixos = !eixos;
}


// SKYBOX: Visualitzar Skybox en l'escena (opció booleana)
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
/*					7. OBJECTE					                             */
/* ------------------------------------------------------------------------- */

// OBJECTE: Cap objecte
void OnObjecteCap()
{
	objecte = CAP;
	netejaVAOList();
// Entorn VGI: Eliminar buffers de textures previs del vector texturesID[].
	for (int i = 0; i < NUM_MAX_TEXTURES; i++) {
		if (texturesID[i]) {
			glDeleteTextures(1, &texturesID[i]);
			texturesID[i] = 0;
		}
	}
// Entorn VGI: Alliberar memòria i textures objecte OBJ, si creades.
	if (ObOBJ != NULL) {
		ObOBJ->netejaVAOList_OBJ();		// Netejar VAO, EBO i VBO
		ObOBJ->netejaTextures_OBJ();	// Netejar buffers de textures
	}

}

// OBJECTE: Cub
void OnObjecteCub()
{
	objecte = CUB;
	netejaVAOList();											// Neteja Llista VAO.
	SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
	Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(1.0));		// Genera EBO de cub mida 1 i el guarda a la posició GLUT_CUBE.
}


// OBJECTE: Cub RGB
void OnObjecteCubRGB()
{
	objecte = CUB_RGB;
	netejaVAOList();						// Neteja Llista VAO.
	Set_VAOList(GLUT_CUBE_RGB, loadglutSolidCubeRGB_EBO(1.0));	// Genera EBO de cub mida 1 i el guarda a la posició GLUT_CUBE_RGB.
}


// OBJECTE Esfera
void OnObjecteEsfera()
{
	objecte = ESFERA;
	netejaVAOList();
	SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
	Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(1.0, 30, 30));
}


// OBJECTE Tetera
void OnObjecteTetera()
{
	objecte = TETERA;
	netejaVAOList();						// Neteja Llista VAO.
	SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
	Set_VAOList(GLUT_TEAPOT, loadglutSolidTeapot_VAO()); //Genera VAO tetera mida 1 i el guarda a la posició GLUT_TEAPOT.
}


// --------------- OBJECTES COMPLEXES

// OBJECTE ARC
void OnObjecteArc()
{
	CColor color_Mar;
	color_Mar.r = 0.5;	color_Mar.g = 0.4; color_Mar.b = 0.9; color_Mar.a = 1.0;
	objecte = ARC;
	netejaVAOList();						// Neteja Llista VAO.
	SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
	Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(1.0));		// Càrrega Cub de costat 1 com a EBO a la posició GLUT_CUBE.
	Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(0.5, 20, 20));	// Càrrega Esfera a la posició GLU_SPHERE.
	Set_VAOList(GLUT_TEAPOT, loadglutSolidTeapot_VAO());		// Carrega Tetera a la posició GLUT_TEAPOT.
	Set_VAOList(MAR_FRACTAL_VAO, loadSea_VAO(color_Mar));		// Carrega Mar a la posició MAR_FRACTAL_VAO.
}


// OBJECTE Tie
void OnObjecteTie()
{
	objecte = TIE;
	netejaVAOList();						// Neteja Llista VAO.
	SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
	Set_VAOList(GLUT_CYLINDER, loadgluCylinder_EBO(5.0f, 5.0f, 0.5f, 6, 1));// Càrrega cilindre com a VAO.
	Set_VAOList(GLU_DISK, loadgluDisk_EBO(0.0f, 5.0f, 6, 1));	// Càrrega disc com a VAO
	Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(10.0f, 80, 80));	// Càrrega disc com a VAO
	Set_VAOList(GLUT_USER1, loadgluCylinder_EBO(5.0f, 5.0f, 2.0f, 6, 1)); // Càrrega cilindre com a VAO
	Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(1.0));			// Càrrega cub com a EBO
	Set_VAOList(GLUT_TORUS, loadglutSolidTorus_EBO(1.0, 5.0, 20, 20));
	Set_VAOList(GLUT_USER2, loadgluCylinder_EBO(1.0f, 0.5f, 5.0f, 60, 1)); // Càrrega cilindre com a VAO
	Set_VAOList(GLUT_USER3, loadgluCylinder_EBO(0.35f, 0.35f, 5.0f, 80, 1)); // Càrrega cilindre com a VAO
	Set_VAOList(GLUT_USER4, loadgluCylinder_EBO(4.0f, 2.0f, 10.25f, 40, 1)); // Càrrega cilindre com a VAO
	Set_VAOList(GLUT_USER5, loadgluCylinder_EBO(1.5f, 4.5f, 2.0f, 8, 1)); // Càrrega cilindre com a VAO
	Set_VAOList(GLUT_USER6, loadgluDisk_EBO(0.0f, 1.5f, 8, 1)); // Càrrega disk com a VAO
}

// ----------------- OBJECTES CORBES BEZIER, LEMNISCATA i B-SPLINE


// OBJECTE Corba Bezier
void OnObjecteCorbaBezier()
{
	nfdchar_t* nomFitxer = NULL;
	nfdresult_t result = NFD_OpenDialog("crv", NULL, &nomFitxer);
	objecte = C_BEZIER;		sw_material[4] = true;
	if (result == NFD_OKAY) {
		// Entorn VGI: Variable de tipus nfdchar_t* 'nomFitxer' conté el nom del fitxer seleccionat
		puts("Success!");
		puts(nomFitxer);
		npts_T = llegir_ptsC(nomFitxer);	// Llegir Fitxer amb els punts de Control de la corba.
		netejaVAOList();						// Neteja Llista VAO.
		SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
		Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(5.0, 20, 20));	// Genera esfera i la guarda a la posició GLUT_CUBE.
		Set_VAOList(CRV_BEZIER, load_Bezier_Curve_EBO(npts_T, PC_t, pas_CS, false)); // Genera corba i la guarda a la posició CRV_BEZIER.
		free(nomFitxer);
		}
}


// OBJECTE Corba Lemniscata 3D
void OnObjecteCorbaLemniscata()
{
	objecte = C_LEMNISCATA;		sw_material[4] = true;
	netejaVAOList();						// Neteja Llista VAO.
	SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
	Set_VAOList(CRV_LEMNISCATA3D, load_Lemniscata3D_EBO(800, pas_CS * 20.0)); // Genera corba i la guarda a la posició CRV_LEMNISCATA3D.
}


// OBJECTE Corba Hermitte
void OnObjecteCorbaHermitte()
{
	nfdchar_t* nomFitxer = NULL;
	nfdresult_t result = NFD_OpenDialog("crv", NULL, &nomFitxer);
	objecte = C_HERMITTE;	sw_material[4] = true;
	if (result == NFD_OKAY) {
		puts("Success!");
		puts(nomFitxer);
		npts_T = llegir_ptsC(nomFitxer);
		netejaVAOList();						// Neteja Llista VAO.
		SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
		// Definir Esfera EBO per a indicar punts de control de la corba
		Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(5.0, 20, 20));	// Guarda (vaoId, vboId, nVertexs) a la posició GLUT_CUBE.
		// Definir Corba HERMITTE com a VAO o EBO
		//Set_VAOList(CRV_HERMITTE, load_Hermitte_Curve_VAO(npts_T, PC_t, pas_CS)); // Genera corba i la guarda a la posició CRV_HERMITTE.
		Set_VAOList(CRV_HERMITTE, load_Hermitte_Curve_EBO(npts_T, PC_t, pas_CS));	// Genera corba i la guarda a la posició CRV_HERMITTE.
		free(nomFitxer);
		}
}

// OBJECTE Corba Catmull Rom (interpolació per punts)
void OnObjecteCorbaCatmullRom()
{
	nfdchar_t* nomFitxer = NULL;
	nfdresult_t result = NFD_OpenDialog("crv", NULL, &nomFitxer);
	objecte = C_CATMULL_ROM;	sw_material[4] = true;
	if (result == NFD_OKAY) {
		puts("Success!");
		puts(nomFitxer);
		npts_T = llegir_ptsC(nomFitxer);
		netejaVAOList();						// Neteja Llista VAO.
		SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
		Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(5.0, 20, 20));	// Guarda (vaoId, vboId, nVertexs) a la posició GLUT_CUBE.
		// Definir Corba CATMULL ROM com a VAO o EBO
		//Set_VAOList(CRV_CATMULL_ROM, load_CatmullRom_Curve_VAO(npts_T, PC_t, pas_CS)); // Genera corba i la guarda a la posició CRV_CATMULL_ROM.
		Set_VAOList(CRV_CATMULL_ROM, load_CatmullRom_Curve_EBO(npts_T, PC_t, pas_CS));	// Genera corba i la guarda a la posició CRV_CATMULL_ROM.
		free(nomFitxer);
		}
}


// OBJECTE Corba B-Spline
void OnObjecteCorbaBSpline()
{
	nfdchar_t* nomFitxer = NULL;
	nfdresult_t result = NFD_OpenDialog("crv", NULL, &nomFitxer);
	objecte = C_BSPLINE;	sw_material[4] = true;
	if (result == NFD_OKAY) {
		puts("Success!");
		puts(nomFitxer);
		npts_T = llegir_ptsC(nomFitxer);
		netejaVAOList();						// Neteja Llista VAO.
		SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
		Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(5.0, 20, 20));	// Guarda (vaoId, vboId, nVertexs) a la posició GLUT_CUBE.
		// Definr Corba BSpline com a VAO
			//Set_VAOList(CRV_BSPLINE, load_BSpline_Curve_VAO(npts_T, PC_t, pas_CS)); // Genera corba i la guarda a la posició CRV_BSPLINE.
		Set_VAOList(CRV_BSPLINE, load_BSpline_Curve_EBO(npts_T, PC_t, pas_CS)); // Genera corba i la guarda a la posició CRV_BSPLINE.
		free(nomFitxer);
		}
}


// OBJECTE Punts de Control: Activació de la visualització dels Punts de control de les Corbes (OPCIÓ BOOLEANA)
void OnObjectePuntsControl()
{
	if (objecte != C_BEZIER || objecte != C_BSPLINE || objecte != C_LEMNISCATA || objecte != C_HERMITTE || objecte != C_CATMULL_ROM) 
		sw_Punts_Control = !sw_Punts_Control;
}



// OBJECTE --> CORBES: Activar o desactivar visualització Triedre de Frenet de les corbes 
//					Lemniscata, Bezier i BSpline (OPCIÓ BOOLEANA)
void OnCorbesTriedreFrenet()
{
	if (objecte != C_BEZIER || objecte != C_BSPLINE || objecte != C_LEMNISCATA || objecte != C_HERMITTE || objecte != C_CATMULL_ROM)
		dibuixa_TriedreFrenet = !dibuixa_TriedreFrenet;
}


// OBJECTE --> CORBES: Activar o desactivar visualització Triedre de Darboux de les corbes Loxodroma (OPCIÓ BOOLEANA)
void OnCorbesTriedreDarboux()
{
	if (objecte != C_BEZIER || objecte != C_BSPLINE || objecte != C_LEMNISCATA || objecte != C_HERMITTE || objecte != C_CATMULL_ROM)
		dibuixa_TriedreDarboux = !dibuixa_TriedreDarboux;
}


// OBJECTE Matriu Primitives
void OnObjecteMatriuPrimitives()
{
	objecte = MATRIUP;
}


// OBJECTE Matriu Primitives VAO
void OnObjecteMatriuPrimitivesVAO()
{
	objecte = MATRIUP_VAO;
	netejaVAOList();						// Neteja Llista VAO.
	SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
	Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(1.0f));
	Set_VAOList(GLUT_TORUS, loadglutSolidTorus_EBO(2.0, 3.0, 20, 20));
	Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(1.0, 20, 20));
}

/* ------------------------------------------------------------------------- */
/*					9. OCULTACIONS											 */
/* ------------------------------------------------------------------------- */

void OnOcultacionsFrontFaces()
{
	front_faces = !front_faces;
}


// OCULTACIONS: Test de Visibilitat? (opció booleana).
void OnOcultacionsTestvis()
{
	test_vis = !test_vis;
}


// OCULTACIONS: Z-Buffer? (opció booleana).
void OnOcultacionsZBuffer()
{
	oculta = !oculta;
}

//CONTINUAR NETEJANT DESDE AQUÍ
/* ------------------------------------------------------------------------- */
/*					10. IL.LUMINACIÓ										 */
/* ------------------------------------------------------------------------- */

// ILUMINACIÓ->REFLECTIVITAT MATERIAL / COLOR: Activació i desactivació de la reflectivitat pròpia del material com a color.
void OnMaterialReflmaterial()
{
// TODO: Agregue aquí su código de controlador de comandos
	sw_material[4] = !sw_material[4];	sw_il = true;

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_material"), sw_material[4]);
}


// ILUMINACIÓ->REFLECTIVITAT MATERIAL EMISSIÓ: Activació i desactivació de la reflectivitat pròpia del material.
void OnMaterialEmissio()
{
// TODO: Agregue aquí su código de controlador de comandos
	sw_material[0] = !sw_material[0];

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[0]"), sw_material[0]);
}


// ILUMINACIÓ->REFLECTIVITAT MATERIAL AMBIENT: Activació i desactivació de la reflectivitat ambient del material.
void OnMaterialAmbient()
{
// TODO: Agregue aquí su código de controlador de comandos
	sw_material[1] = !sw_material[1];

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[1]"), sw_material[1]);
}


// ILUMINACIÓ->REFLECTIVITAT MATERIAL DIFUSA: Activació i desactivació de la reflectivitat difusa del materials.
void OnMaterialDifusa()
{
// TODO: Agregue aquí su código de controlador de comandos
	sw_material[2] = !sw_material[2];

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[2]"), sw_material[2]);
}


// ILUMINACIÓ->REFLECTIVITAT MATERIAL ESPECULAR: Activació i desactivació de la reflectivitat especular del material.
void OnMaterialEspecular()
{
// TODO: Agregue aquí su código de controlador de comandos
	sw_material[3] = !sw_material[3];

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[3]"), sw_material[3]);
}


// ILUMINACIÓ: Textures?: Activació (TRUE) o desactivació (FALSE) de textures.
void OnIluminacioTextures()
{
	// TODO: Agregue aquí su código de controlador de comandos
	textura = !textura;

//	Pas de textura al shader
	glUniform1i(glGetUniformLocation(shader_programID, "texture"), textura);
}


// ILUMINACIÓ --> TEXTURA: Càrrega fitxer textura per llibreria SOIL
void OnIluminacioTexturaFitxerimatge()
{
// TODO: Agregue aquí su código de controlador de comandos
	nfdchar_t* nomFitxer = NULL;
	nfdresult_t result = NFD_OpenDialog(NULL, NULL, &nomFitxer);
	
	t_textura = TEXTURA_FITXERIMA;		tFlag_invert_Y = true;
	textura = true;
	
	if (result == NFD_OKAY) {
		// Entorn VGI: Variable de tipus nfdchar_t* 'nomFitxer' conté el nom del fitxer seleccionat
		puts("Success!");
		puts(nomFitxer);

		// Entorn VGI: Eliminar buffers de textures previs del vector texturesID[].
		for (int i = 0; i < NUM_MAX_TEXTURES; i++) {
			if (texturesID[i]) {
				//err = glIsTexture(texturesID[i]);
				glDeleteTextures(1, &texturesID[i]);
				//err = glIsTexture(texturesID[i]);
				texturesID[i] = 0;
			}
		}

		// EntornVGI: Carregar fitxer textura i definir buffer de textura.Identificador guardat a texturesID[0].
		texturesID[0] = loadIMA_SOIL(nomFitxer);

		//	Pas de textura al shader
		glUniform1i(glGetUniformLocation(shader_programID, "texture0"), GLint(0));
		glUniform1i(glGetUniformLocation(shader_programID, "flag_invert_y"), tFlag_invert_Y);
		free(nomFitxer);
		}
}


// ILUMINACIÓ --> TEXTURA: FLAG_INVERT_Y Inversió coordenada t de textura (1-cty) per a textures SOIL (TRUE) 
//			o no (FALSE) per a objectes 3DS i OBJ.
void OnIluminacioTexturaFlagInvertY()
{
// TODO: Agregue aquí su código de controlador de comandos
	if (textura) tFlag_invert_Y = !tFlag_invert_Y;

//	Pas de paràmetres textura al shader
	glUniform1i(glGetUniformLocation(shader_programID, "flag_invert_y"), tFlag_invert_Y);
}

/* ------------------------------------------------------------------------- */
/*					11. LLUMS												 */
/* ------------------------------------------------------------------------- */

// LLUMS: Activació / Desactivació llum ambient 
void OnLlumsLlumAmbient()
{
// TODO: Agregue aquí su código de controlador de comandos
	//llum_ambient = !llum_ambient;
	sw_il = true;

// Pas màscara llums al shader
	glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[1]"), (llum_ambient && sw_material[1])); // Pas màscara llums al shader
}

// LLUMS: Activació /Desactivació llum 0 (GL_LIGHT0)
void OnLlumsLlum0()
{
// TODO: Agregue aquí su código de controlador de comandos
	//llumGL[0].encesa = !llumGL[0].encesa;
	sw_il = true;

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[0]"), llumGL[0].encesa);
}


// LLUMS-->ON/OFF: Activació /Desactivació llum 1 (GL_LIGHT1)
void OnLlumsLlum1()
{
// TODO: Agregue aquí su código de controlador de comandos
	//llumGL[1].encesa = !llumGL[1].encesa;
	sw_il = true;

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[1]"), llumGL[1].encesa);
}


// LLUMS-->ON/OFF: Activació /Desactivació llum 2 (GL_LIGHT2)
void OnLlumsLlum2()
{
// TODO: Agregue aquí su código de controlador de comandos
	//llumGL[2].encesa = !llumGL[2].encesa;
	sw_il = true;

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[2]"), llumGL[2].encesa);
}


// LLUMS-->ON/OFF: Activació /Desactivació llum 3 (GL_LIGHT3)
void OnLlumsLlum3()
{
// TODO: Agregue aquí su código de controlador de comandos
	//llumGL[3].encesa = !llumGL[3].encesa;
	sw_il = true;

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[3]"), llumGL[3].encesa);
}


// LLUMS-->ON/OFF: Activació /Desactivació llum 4 (GL_LIGHT4)
void OnLlumsLlum4()
{
// TODO: Agregue aquí su código de controlador de comandos
	//llumGL[4].encesa = !llumGL[4].encesa;
	sw_il = true;

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[4]"), llumGL[4].encesa);
}

// LLUMS-->ON/OFF: Activació /Desactivació llum 5 (GL_LIGHT5)
void OnLlumsLlum5()
{
// TODO: Agregue aquí su código de controlador de comandos
	//llumGL[5].encesa = !llumGL[5].encesa;
	sw_il = true;

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[5]"), llumGL[5].encesa);
}

// LLUMS-->ON/OFF: Activació /Desactivació llum 6 (GL_LIGHT6)
void OnLlumsLlum6()
{
// TODO: Agregue aquí su código de controlador de comandos
	//llumGL[6].encesa = !llumGL[6].encesa;
	sw_il = true;

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[6]"), llumGL[6].encesa);
}

// LLUMS-->ON/OFF: Activació /Desactivació llum 7 (GL_LIGHT7)
void OnLlumsLlum7()
{
// TODO: Agregue aquí su código de controlador de comandos
	//llumGL[7].encesa = !llumGL[7].encesa;
	sw_il = true;

// Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[7]"), llumGL[7].encesa);
}

/* ------------------------------------------------------------------------- */
/*					12. SHADERS												 */
/* ------------------------------------------------------------------------- */

// SHADER FLAT
void OnShaderFlat()
{
// TODO: Agregue aquí su código de controlador de comandos
	GLuint newShaderID = 0;

	shader = FLAT_SHADER;	ilumina = SUAU;
	test_vis = false;		oculta = true;

// Càrrega Shader de Flat
	fprintf(stderr, "Flat Shader: \n");

// Elimina shader anterior
	shaderLighting.DeleteProgram();	shader_programID = 0;
// Càrrega Flat shader
	newShaderID = shaderLighting.loadFileShaders(".\\shaders\\flat_shdrML.vert", ".\\shaders\\flat_shdrML.frag");

// Càrrega shaders dels fitxers
	if (newShaderID) {
		shader_programID = newShaderID;
		// Activa shader.
		glUseProgram(shader_programID); // shaderLighting.use();
	}
	else fprintf(stderr, "%s", "GLSL_Error. Fitxers .vert o .frag amb errors"); // AfxMessageBox(_T("GLSL_Error. Fitxers .vert o .frag amb errors"));
}

// SHADER GOURAUD
void OnShaderGouraud()
{
// TODO: Agregue aquí su código de controlador de comandos
	GLuint newShaderID = 0;

	shader = GOURAUD_SHADER;	ilumina = SUAU;
	test_vis = false;			oculta = true;

// Càrrega Shader de Gouraud
	fprintf(stderr, "Gouraud Shader: \n");

// Elimina shader anterior
	shaderLighting.DeleteProgram();	shader_programID = 0;
// Càrrega Gouraud shader 
	newShaderID = shaderLighting.loadFileShaders(".\\shaders\\gouraud_shdrML.vert", ".\\shaders\\gouraud_shdrML.frag");

// Càrrega shaders dels fitxers
	if (newShaderID) {
		shader_programID = newShaderID;
		// Activa shader.
		glUseProgram(shader_programID); // shaderLighting.use();
	}
	else fprintf(stderr, "%s", "GLSL_Error. Fitxers .vert o .frag amb errors"); // AfxMessageBox(_T("GLSL_Error. Fitxers .vert o .frag amb errors"));
}


// SHADER PHONG
void OnShaderPhong()
{
	GLuint newShaderID = 0;

// TODO: Agregue aquí su código de controlador de comandos
	shader = PHONG_SHADER;	ilumina = SUAU;
	test_vis = false;		oculta = true;

// Càrrega Shader de Phong
	fprintf(stderr, "Phong Shader: \n");

// Elimina shader anterior
	shaderLighting.DeleteProgram();	shader_programID = 0;
// Càrrega Phong Shader
	newShaderID = shaderLighting.loadFileShaders(".\\shaders\\phong_shdrML.vert", ".\\shaders\\phong_shdrML.frag");

// Càrrega shaders dels fitxers
	if (newShaderID) {
		shader_programID = newShaderID;
		// Activa shader.
		glUseProgram(shader_programID); // shaderLighting.use();
	}
	else fprintf(stderr, "%s", "GLSL_Error. Fitxers .vert o .frag amb errors"); // AfxMessageBox(_T("GLSL_Error. Fitxers .vert o .frag amb errors"));
}


// SHADERS: Càrrega Fitxers Shader (.vert, .frag)
void OnShaderLoadFiles()
{
// TODO: Agregue aquí su código de controlador de comandos
	GLuint newShaderID = 0;

	nfdchar_t* nomVertS = NULL;
	nfdchar_t* nomFragS = NULL;

	shader = FILE_SHADER;	ilumina = SUAU;
	test_vis = false;		oculta = true;

// Càrrega fitxer VERT
// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.VERT)
	nfdresult_t resultVS = NFD_OpenDialog("vert,vrt,vs", NULL, &nomVertS);

	if (resultVS == NFD_OKAY) {	puts("Fitxer .vert llegit!");
								puts(nomVertS);
								}
	else if (resultVS == NFD_CANCEL) {	puts("User pressed cancel.");
										return;
									}
	else {	printf("Error: %s\n", NFD_GetError());
			return;
		}

// Càrrega fitxer FRAG
// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.FRAG)
	nfdresult_t resultFS = NFD_OpenDialog("frag,frg,fs", NULL, &nomFragS);

	if (resultVS == NFD_OKAY) {	puts("Fitxer .FRAG llegit!");
								puts(nomFragS);
							}
	else if (resultVS == NFD_CANCEL) {	puts("User pressed cancel.");
										return;
									}
	else {	printf("Error: %s\n", NFD_GetError());
			return;
		}

// Entorn VGI: Carrega dels shaders
// Elimina shader anterior
	shaderLighting.DeleteProgram();	shader_programID = 0;
	newShaderID = shaderLighting.loadFileShaders(nomVertS, nomFragS);
	
// Càrrega shaders dels fitxers
	if (newShaderID) {
		shader_programID = newShaderID;
		// Activa shader.
		glUseProgram(shader_programID); // shaderLighting.use();
		}
	else fprintf(stderr, "%s", "GLSL_Error. Fitxers .vert o .frag amb errors"); // AfxMessageBox(_T("GLSL_Error. Fitxers .vert o .frag amb errors"));
}


// Escriure Binary Program actual en fitxer .bin
void OnShaderPBinaryWrite()
{
// TODO: Agregue aquí su código de controlador de comandos
	nfdchar_t* nomPBinary = NULL;

// Càrrega fitxer .BIN
// Entorn VGI: Obrir diàleg d'escriptura de fitxer (fitxers (*.bin)
	nfdresult_t resultBS = NFD_OpenDialog(NULL, NULL, &nomPBinary);

	if (resultBS == NFD_OKAY) {
		// Entorn VGI: Variable de tipus nfdchar_t* 'nomFitxer' conté el nom del fitxer seleccionat
		puts("Success!");
		puts(nomPBinary);

		// Entorn VGI: To retrieve the compiled Binary Program shader code and write it to a file
		GLint formats = 0;
		glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
		GLint* binaryFormats = new GLint[formats];
		glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, binaryFormats);

		GLint length = 0;
		glGetProgramiv(shader_programID, GL_PROGRAM_BINARY_LENGTH, &length);

		// Retrieve the binary code
		std::vector<GLubyte> buffer(length);
		GLenum* Formats = 0;
		glGetProgramBinary(shader_programID, length, NULL, (GLenum*)Formats, buffer.data());

		// Write the binary to a binary file
		FILE* sb;
		sb = fopen(nomPBinary, "wb");
		fwrite(buffer.data(), length, 1, sb);
		fclose(sb);

		// MISSATGE DE FITXER BEN GRAVAT o MAL GRAVAT
		//AfxMessageBox(_T("Fitxer ben gravat"));
		fprintf(stderr, "%s \n", "Fitxer ben gravat");
		}


}


// Llegir Binary Program de fitxer .bin i instalar i definir com actual.
void OnShaderPBinaryRead()
{
// TODO: Agregue aquí su código de controlador de comandos
	nfdchar_t* nomPBinary = NULL;
	FILE* fd;

	shader = PROG_BINARY_SHADER;		ilumina = SUAU;
	test_vis = false;					oculta = true;

// Càrrega fitxer .BIN
// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.bin)
	nfdresult_t resultBS = NFD_OpenDialog(NULL, NULL, &nomPBinary);

	if (resultBS == NFD_OKAY) {
		// Entorn VGI: Variable de tipus nfdchar_t* 'nomFitxer' conté el nom del fitxer seleccionat
		puts("Success!");
		puts(nomPBinary);

		// Entorn VGI: To read de Shader Program from a file and install it
		GLint filelength = 0;
		GLenum format = 0;

		/* Retrieve the binary code per a obtenir valor variable format
			std::vector<GLubyte> buff(filelength);
			GLint longitut = 0;
			glGetProgramBinary(shader_programID, longitut, NULL, &format, buff.data());
		*/

		// Entorn VGI: Read from a binary file
		FILE* sb;
		sb = fopen(nomPBinary, "rb");
		if (!sb) {
			fprintf(stderr, "%s \n", "GLSL_Error. Unable to open file"); // AfxMessageBox(_T("GLSL_Error. Unable to open file"));
			return;
		}

		// Get file length
		fseek(sb, 0, SEEK_END);
		filelength = ftell(sb);
		fseek(sb, 0, SEEK_SET);

		std::vector<GLubyte> buffer(filelength + 1); // Allocatem buffer amb mida de Binary Program
		fclose(sb);

		sb = fopen(nomPBinary, "rb");
		fread(buffer.data(), filelength, 1, sb);
		fclose(sb);

		// Install shader binary
		GLint formats = 0;
		glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

		GLuint shader_BinProgramID = glCreateProgram();
		glProgramBinary(shader_BinProgramID, formats, buffer.data(), filelength);

		//glLinkProgram(shader_BinProgramID); // Linkedició del program.
		// Check for success/failure
		GLint status;
		glGetProgramiv(shader_BinProgramID, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			// Llista error de linkedició del Shader Program
			GLint maxLength = 0;

			glGetProgramiv(shader_BinProgramID, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(shader_BinProgramID, maxLength, &maxLength, &errorLog[0]);

			// AfxMessageBox(_T("Error Linkedicio Shader Binary Program"));
			fprintf(stderr, "%s \n", "Error Linkedicio Shader Binary Program");
			//fprintf(stderr, "%s \n", "Error Linkedicio Shader Program");

			// Volcar missatges error a fitxer GLSL_Error.LINK
			if ((fd = fopen("GLSL_Error.LINK", "w")) == NULL)
				{	//AfxMessageBox(_T("GLSL_Error.LINK was not opened"));
					fprintf(stderr, "%s \n", "GLSL_Error.LINK was not opened");
				}
			for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
			fclose(fd);
			glDeleteProgram(shader_BinProgramID);		// Don't leak the program.
			}
		else {
			//shaderLighting.DeleteProgram();	// Eliminar shader anterior
			shader_programID = shader_BinProgramID; // Assignar nou Binary Program com l'actual.
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUseProgram(shader_programID);			// Activa shader llegit.
			}
	}
}


/* ------------------------------------------------------------------------- */
/*                            FI MENUS ImGui                                 */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL RATOLI                              */
/* ------------------------------------------------------------------------- */

// OnMouseButton: Funció que es crida quan s'apreta algun botó (esquerra o dreta) del mouse.
//      PARAMETRES: - window: Finestra activa
//					- button: Botó seleccionat (GLFW_MOUSE_BUTTON_LEFT o GLFW_MOUSE_BUTTON_RIGHT)
//					- action: Acció de la tecla: GLFW_PRESS (si s'ha apretat), GLFW_REPEAT, si s'ha repetit pressió i GL_RELEASE, si es deixa d'apretar.
void OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
// Get the cursor position when the mouse key has been pressed or released.
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

// EntornVGI.ImGui: Test si events de mouse han sigut filtrats per ImGui (io.WantCaptureMouse)
// (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(button, action);

// (2) ONLY forward mouse data to your underlying app/game.
	if (!io.WantCaptureMouse) { //<Tractament mouse de l'aplicació>}
		// OnLButtonDown
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			// Entorn VGI: Detectem en quina posició s'ha apretat el botó esquerra del
			//				mouse i ho guardem a la variable m_PosEAvall i activem flag m_ButoEAvall
			m_ButoEAvall = true;
			m_PosEAvall.x = xpos;	m_PosEAvall.y = ypos;
			m_EsfeEAvall = OPV;
		}
		// OnLButtonUp: Funció que es crida quan deixem d'apretar el botó esquerra del mouse.
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{	// Entorn VGI: Desactivem flag m_ButoEAvall quan deixem d'apretar botó esquerra del mouse.
			m_ButoEAvall = false;

			// OPCIÓ VISTA-->SATÈLIT: Càlcul increment desplaçament del Punt de Vista
			if ((satelit) && (projeccio != ORTO))
			{	//m_EsfeIncEAvall.R = m_EsfeEAvall.R - OPV.R;
				m_EsfeIncEAvall.alfa = 0.01f * (OPV.alfa - m_EsfeEAvall.alfa); //if (abs(m_EsfeIncEAvall.alfa)<0.01) { if ((m_EsfeIncEAvall.alfa)>0.0) m_EsfeIncEAvall.alfa = 0.01 else m_EsfeIncEAvall.alfa=0.01}
				m_EsfeIncEAvall.beta = 0.01f * (OPV.beta - m_EsfeEAvall.beta);
				if (abs(m_EsfeIncEAvall.beta) < 0.01)
				{
					if ((m_EsfeIncEAvall.beta) > 0.0) m_EsfeIncEAvall.beta = 0.01;
					else m_EsfeIncEAvall.beta = 0.01;
				}
				//if ((m_EsfeIncEAvall.R == 0.0) && (m_EsfeIncEAvall.alfa == 0.0) && (m_EsfeIncEAvall.beta == 0.0)) KillTimer(WM_TIMER);
				//else SetTimer(WM_TIMER, 10, NULL);
			}
		}
		// OnRButtonDown
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{	// Entorn VGI: Detectem en quina posició s'ha apretat el botó esquerra del
			//				mouse i ho guardem a la variable m_PosEAvall i activem flag m_ButoEAvall
			m_ButoDAvall = true;
			//m_PosDAvall = point;
			m_PosDAvall.x = xpos;	m_PosDAvall.y = ypos;
		}
		// OnLButtonUp: Funció que es crida quan deixem d'apretar el botó esquerra del mouse.
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		{	// Entorn VGI: Desactivem flag m_ButoEAvall quan deixem d'apretar botó esquerra del mouse.
			m_ButoDAvall = false;
		}
	}
}

// OnMouseMove: Funció que es crida quan es mou el mouse. La utilitzem per la 
//				  Visualització Interactiva amb les tecles del mouse apretades per 
//				  modificar els paràmetres de P.V. (R,angleh,anglev) segons els 
//				  moviments del mouse.
//      PARAMETRES: - window: Finestra activa
//					- xpos: Posició X del cursor del mouse (coord. pantalla) quan el botó s'ha apretat.
//					- ypos: Posició Y del cursor del mouse(coord.pantalla) quan el botó s'ha apretat.
void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	double modul = 0;
	GLdouble vdir[3] = { 0, 0, 0 };
	CSize gir = { 0,0 }, girn = { 0,0 }, girT = { 0,0 }, zoomincr = { 0,0 };

	// TODO: Add your message handler code here and/or call default
	if (m_ButoEAvall && mobil && projeccio != CAP)
	{
// Entorn VGI: Determinació dels angles (en graus) segons l'increment
//				horitzontal i vertical de la posició del mouse.
		gir.cx = m_PosEAvall.x - xpos;		gir.cy = m_PosEAvall.y - ypos;
		m_PosEAvall.x = xpos;				m_PosEAvall.y = ypos;
		if (camera == CAM_ESFERICA)
		{	// Càmera Esfèrica
			OPV.beta = OPV.beta - gir.cx / 2.0;
			OPV.alfa = OPV.alfa + gir.cy / 2.0;

			// Entorn VGI: Control per evitar el creixement desmesurat dels angles.
			while (OPV.alfa >= 360)		OPV.alfa = OPV.alfa - 360.0;
			while (OPV.alfa < 0)		OPV.alfa = OPV.alfa + 360.0;
			while (OPV.beta >= 360)		OPV.beta = OPV.beta - 360.0;
			while (OPV.beta < 0)		OPV.beta = OPV.beta + 360.0;
		}
		else { // Càmera Geode
			OPV_G.beta = OPV_G.beta + gir.cx / 2.0;
			OPV_G.alfa = OPV_G.alfa + gir.cy / 2.0;
			// Entorn VGI: Control per evitar el creixement desmesurat dels angles
			while (OPV_G.alfa >= 360.0f)	OPV_G.alfa = OPV_G.alfa - 360.0;
			while (OPV_G.alfa < 0.0f)		OPV_G.alfa = OPV_G.alfa + 360.0;
			while (OPV_G.beta >= 360.f)		OPV_G.beta = OPV_G.beta - 360.0;
			while (OPV_G.beta < 0.0f)		OPV_G.beta = OPV_G.beta + 360.0;
		}
		// Crida a OnPaint() per redibuixar l'escena
		//OnPaint(window);
	}
	else if (m_ButoEAvall && (camera == CAM_NAVEGA) && (projeccio != CAP && projeccio != ORTO)) // Opció Navegació
	{	// Entorn VGI: Canviar orientació en opció de Navegació
		girn.cx = m_PosEAvall.x - xpos;		girn.cy = m_PosEAvall.y - ypos;
		angleZ = girn.cx / 2.0;
		// Entorn VGI: Control per evitar el creixement desmesurat dels angles.
		while (angleZ >= 360,0) angleZ = angleZ - 360;
		while (angleZ < 0.0)	angleZ = angleZ + 360;

		// Entorn VGI: Segons orientació dels eixos Polars (Vis_Polar)
		if (Vis_Polar == POLARZ) { // (X,Y,Z)
			n[0] = n[0] - opvN.x;
			n[1] = n[1] - opvN.y;
			n[0] = n[0] * cos(angleZ * PI / 180.0) - n[1] * sin(angleZ * PI / 180.0);
			n[1] = n[0] * sin(angleZ * PI / 180.0) + n[1] * cos(angleZ * PI / 180.0);
			n[0] = n[0] + opvN.x;
			n[1] = n[1] + opvN.y;
			}
		else if (Vis_Polar == POLARY) { //(X,Y,Z) --> (Z,X,Y)
			n[2] = n[2] - opvN.z;
			n[0] = n[0] - opvN.x;
			n[2] = n[2] * cos(angleZ * PI / 180.0) - n[0] * sin(angleZ * PI / 180.0);
			n[0] = n[2] * sin(angleZ * PI / 180.0) + n[0] * cos(angleZ * PI / 180.0);
			n[2] = n[2] + opvN.z;
			n[0] = n[0] + opvN.x;
			}
		else if (Vis_Polar == POLARX) { //(X,Y,Z) --> (Y,Z,X)
			n[1] = n[1] - opvN.y;
			n[2] = n[2] - opvN.z;
			n[1] = n[1] * cos(angleZ * PI / 180.0) - n[2] * sin(angleZ * PI / 180.0);
			n[2] = n[1] * sin(angleZ * PI / 180.0) + n[2] * cos(angleZ * PI / 180.0);
			n[1] = n[1] + opvN.y;
			n[2] = n[2] + opvN.z;
			}

		m_PosEAvall.x = xpos;		m_PosEAvall.y = ypos;
		// Crida a OnPaint() per redibuixar l'escena
		//OnPaint(window);
	}

	// Entorn VGI: Transformació Geomètrica interactiva pels eixos X,Y boto esquerra del mouse.
	else {
		bool transE = transX || transY;
		if (m_ButoEAvall && transE && transf)
		{	// Calcular increment
			girT.cx = m_PosEAvall.x - xpos;		girT.cy = m_PosEAvall.y - ypos;
			if (transX)
			{	long int incrT = girT.cx;
				if (trasl)
				{	TG.VTras.x += incrT * fact_Tras;
					if (TG.VTras.x < -100000.0) TG.VTras.x = 100000.0;
					if (TG.VTras.x > 100000.0) TG.VTras.x = 100000.0;
				}
				else if (rota)
				{	TG.VRota.x += incrT * fact_Rota;
					while (TG.VRota.x >= 360.0) TG.VRota.x -= 360.0;
					while (TG.VRota.x < 0.0) TG.VRota.x += 360.0;
				}
				else if (escal)
				{	if (incrT < 0) incrT = -1 / incrT;
					TG.VScal.x = TG.VScal.x * incrT;
					if (TG.VScal.x < 0.25) TG.VScal.x = 0.25;
					if (TG.VScal.x > 8192.0) TG.VScal.x = 8192.0;
				}
			}
			if (transY)
			{	long int incrT = girT.cy;
				if (trasl)
				{	TG.VTras.y += incrT * fact_Tras;
					if (TG.VTras.y < -100000) TG.VTras.y = 100000;
					if (TG.VTras.y > 100000) TG.VTras.y = 100000;
				}
				else if (rota)
				{	TG.VRota.y += incrT * fact_Rota;
					while (TG.VRota.y >= 360.0) TG.VRota.y -= 360.0;
					while (TG.VRota.y < 0.0) TG.VRota.y += 360.0;
				}
				else if (escal)
				{	if (incrT <= 0.0) {	if (incrT >= -2) incrT = -2;
										incrT = 1 / Log2(-incrT);
									}
						else incrT = Log2(incrT);
					TG.VScal.y = TG.VScal.y * incrT;
					if (TG.VScal.y < 0.25) TG.VScal.y = 0.25;
					if (TG.VScal.y > 8192.0) TG.VScal.y = 8192.0;
				}
			}
			m_PosEAvall.x = xpos;	m_PosEAvall.y = ypos;
			// Crida a OnPaint() per redibuixar l'escena
			//InvalidateRect(NULL, false);
			//OnPaint(windows);
		}
	}

	// Entorn VGI: Determinació del desplaçament del pan segons l'increment
	//				vertical de la posició del mouse (tecla dreta apretada).
	if (m_ButoDAvall && pan && (projeccio != CAP && projeccio != ORTO))
	{
		//CSize zoomincr = m_PosDAvall - point;
		zoomincr.cx = m_PosDAvall.x - xpos;		zoomincr.cy = m_PosDAvall.y - ypos;
		long int incrx = zoomincr.cx;
		long int incry = zoomincr.cy;

		// Desplaçament pan vertical
		tr_cpv.y -= incry * fact_pan;
		if (tr_cpv.y > 100000.0) tr_cpv.y = 100000.0;
		else if (tr_cpv.y < -100000.0) tr_cpv.y = -100000.0;

		// Desplaçament pan horitzontal
		tr_cpv.x += incrx * fact_pan;
		if (tr_cpv.x > 100000.0) tr_cpv.x = 100000.0;
		else if (tr_cpv.x < -100000.0) tr_cpv.x = -100000.0;

		//m_PosDAvall = point;
		m_PosDAvall.x = xpos;	m_PosDAvall.y = ypos;
		// Crida a OnPaint() per redibuixar l'escena
		//OnPaint(window);
	}
	// Determinació del paràmetre R segons l'increment
	//   vertical de la posició del mouse (tecla dreta apretada)
		//else if (m_ButoDAvall && zzoom && (projeccio!=CAP && projeccio!=ORTO))
	else if (m_ButoDAvall && zzoom && (projeccio != CAP))
	{	//CSize zoomincr = m_PosDAvall - point;
		zoomincr.cx = m_PosDAvall.x - xpos;		zoomincr.cy = m_PosDAvall.y - ypos;
		long int incr = zoomincr.cy / 1.0;

		if (camera == CAM_ESFERICA) {	// Càmera Esfèrica
										OPV.R = OPV.R + incr;
										//if (OPV.R < 0.25) OPV.R = 0.25;
										if (OPV.R < p_near) OPV.R = p_near;
										if (OPV.R > p_far) OPV.R = p_far;
									}
			else { // Càmera Geode
					OPV_G.R = OPV_G.R + incr;
					if (OPV_G.R < 0.0) OPV_G.R = 0.0;
				}

		//m_PosDAvall = point;
		m_PosDAvall.x = xpos;				m_PosDAvall.y = ypos;
		// Crida a OnPaint() per redibuixar l'escena
		//OnPaint(window);
	}
	else if (m_ButoDAvall &&  (camera == CAM_NAVEGA) && (projeccio != CAP && projeccio != ORTO))
	{	// Avançar en opció de Navegació
		if ((m_PosDAvall.x != xpos) && (m_PosDAvall.y != ypos))
		{
			//CSize zoomincr = m_PosDAvall - point;
			zoomincr.cx = m_PosDAvall.x - xpos;		zoomincr.cy = m_PosDAvall.y - ypos;
			double incr = zoomincr.cy / 2.0;
			//	long int incr=zoomincr.cy/2.0;  // Causa assertion en "afx.inl" lin 169
			vdir[0] = n[0] - opvN.x;
			vdir[1] = n[1] - opvN.y;
			vdir[2] = n[2] - opvN.z;
			modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
			vdir[0] = vdir[0] / modul;
			vdir[1] = vdir[1] / modul;
			vdir[2] = vdir[2] / modul;

			// Entorn VGI: Segons orientació dels eixos Polars (Vis_Polar)
			if (Vis_Polar == POLARZ) { // (X,Y,Z)
				opvN.x += incr * vdir[0];
				opvN.y += incr * vdir[1];
				n[0] += incr * vdir[0];
				n[1] += incr * vdir[1];
				}
			else if (Vis_Polar == POLARY) { //(X,Y,Z) --> (Z,X,Y)
				opvN.z += incr * vdir[2];
				opvN.x += incr * vdir[0];
				n[2] += incr * vdir[2];
				n[0] += incr * vdir[0];
				}
			else if (Vis_Polar == POLARX) { //(X,Y,Z) --> (Y,Z,X)
				opvN.y += incr * vdir[1];
				opvN.z += incr * vdir[2];
				n[1] += incr * vdir[1];
				n[2] += incr * vdir[2];
				}

			//m_PosDAvall = point;
			m_PosDAvall.x = xpos;				m_PosDAvall.y = ypos;
			// Crida a OnPaint() per redibuixar l'escena
			//OnPaint(window);
		}
	}

// Entorn VGI: Transformació Geomètrica interactiva per l'eix Z amb boto dret del mouse.
	else if (m_ButoDAvall && transZ && transf)
	{	// Calcular increment
		girT.cx = m_PosDAvall.x - xpos;		girT.cy = m_PosDAvall.y - ypos;
		long int incrT = girT.cy;
		if (trasl)
			{	TG.VTras.z += incrT * fact_Tras;
				if (TG.VTras.z < -100000.0) TG.VTras.z = 100000.0;
				if (TG.VTras.z > 100000.0) TG.VTras.z = 100000.0;
			}
		else if (rota)
			{	incrT = girT.cx;
				TG.VRota.z += incrT * fact_Rota;
				while (TG.VRota.z >= 360.0) TG.VRota.z -= 360.0;
				while (TG.VRota.z < 0.0) TG.VRota.z += 360.0;
			}
		else if (escal)
			{	if (incrT <= 0) {
					if (incrT >= -2.0) incrT = -2.0;
					incrT = 1.0 / Log2(-incrT);
					}
				else incrT = Log2(incrT);
				TG.VScal.z = TG.VScal.z * incrT;
				if (TG.VScal.z < 0.25) TG.VScal.z = 0.25;
				if (TG.VScal.z > 8192.0) TG.VScal.z = 8192.0;
			}
		//m_PosDAvall = point;
		m_PosDAvall.x = xpos;	m_PosDAvall.y = ypos;
		// Crida a OnPaint() per redibuixar l'escena
		//OnPaint(window);
	}
}

// OnMouseWheel: Funció que es crida quan es mou el rodet del mouse. La utilitzem per la 
//				  Visualització Interactiva per modificar el paràmetre R de P.V. (R,angleh,anglev) 
//				  segons el moviment del rodet del mouse.
//      PARAMETRES: -  (xoffset,yoffset): Estructura (x,y) que dóna la posició del mouse 
//							 (coord. pantalla) quan el botó s'ha apretat.
void OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	double modul = 0;
	GLdouble vdir[3] = { 0, 0, 0 };

// EntornVGI.ImGui: Test si events de mouse han sigut filtrats per ImGui (io.WantCaptureMouse)
// (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
	ImGuiIO& io = ImGui::GetIO();
	//io.AddMouseButtonEvent(button, true);

// (2) ONLY forward mouse data to your underlying app/game.
	if (!io.WantCaptureMouse) { // <Tractament mouse de l'aplicació>}
		// Funció de zoom quan està activada la funció pan o les T. Geomètriques
		if ((zzoom || zzoomO) || (transX) || (transY) || (transZ))
		{	if (camera == CAM_ESFERICA) {	// Càmera Esfèrica
				OPV.R = OPV.R + yoffset / 4.0;
				if (OPV.R < 0.25) OPV.R = 0.25;
				//InvalidateRect(NULL, false);
			}
			else if (camera == CAM_GEODE)
			{	// Càmera Geode
				OPV_G.R = OPV_G.R + yoffset / 4.0;
				if (OPV_G.R < 0.0) OPV_G.R = 0.0;
				//InvalidateRect(NULL, false);
			}
		}
		else if (camera == CAM_NAVEGA && !io.WantCaptureMouse)
		{	vdir[0] = n[0] - opvN.x;
			vdir[1] = n[1] - opvN.y;
			vdir[2] = n[2] - opvN.z;
			modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
			vdir[0] = vdir[0] / modul;
			vdir[1] = vdir[1] / modul;
			vdir[2] = vdir[2] / modul;

			// Entorn VGI: Segons orientació dels eixos Polars (Vis_Polar)
			if (Vis_Polar == POLARZ) { // (X,Y,Z)
				opvN.x += (yoffset / 4.0) * vdir[0];
				opvN.y += (yoffset / 4.0) * vdir[1];
				n[0] += (yoffset / 4.0) * vdir[0];
				n[1] += (yoffset / 4.0) * vdir[1];
				}
			else if (Vis_Polar == POLARY) { //(X,Y,Z) --> (Z,X,Y)
				opvN.z += (yoffset / 4.0) * vdir[2];
				opvN.x += (yoffset / 4.0) * vdir[0];
				n[2] += (yoffset / 4.0) * vdir[2];
				n[0] += (yoffset / 4.0) * vdir[0];
				}
			else if (Vis_Polar == POLARX) { //(X,Y,Z) --> (Y,Z,X)
				opvN.y += (yoffset / 4.0) * vdir[1];
				opvN.z += (yoffset / 4.0) * vdir[2];
				n[1] += (yoffset / 4.0) * vdir[1];
				n[2] += (yoffset / 4.0) * vdir[2];
				}
/*
			opvN.x += (yoffset / 4.0) * vdir[0];		//opvN.x += (zDelta / 4.0) * vdir[0];
			opvN.y += (yoffset / 4.0) * vdir[1];		//opvN.y += (zDelta / 4.0) * vdir[1];
			n[0] += (yoffset / 4.0) * vdir[0];		//n[0] += (zDelta / 4.0) * vdir[0];
			n[1] += (yoffset / 4.0) * vdir[1];		//n[1] += (zDelta / 4.0) * vdir[1];
*/
		}
	}
}


/* ------------------------------------------------------------------------- */
/*					     TIMER (ANIMACIÓ)									 */
/* ------------------------------------------------------------------------- */
void OnTimer()
{
	// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	if (anima) {
		// Codi de tractament de l'animació quan transcorren els ms. del crono.

		// Crida a OnPaint() per redibuixar l'escena
		//InvalidateRect(NULL, false);
	}
	else if (satelit) {	// OPCIÓ SATÈLIT: Increment OPV segons moviments mouse.
		//OPV.R = OPV.R + m_EsfeIncEAvall.R;
		OPV.alfa = OPV.alfa + m_EsfeIncEAvall.alfa;
		while (OPV.alfa > 360.0) OPV.alfa = OPV.alfa - 360.0;
		while (OPV.alfa < 0.0) OPV.alfa = OPV.alfa + 360.0;
		OPV.beta = OPV.beta + m_EsfeIncEAvall.beta;
		while (OPV.beta > 360.0) OPV.beta = OPV.beta - 360.0;
		while (OPV.beta < 0.0) OPV.beta = OPV.beta + 360.0;

		// Crida a OnPaint() per redibuixar l'escena
		//OnPaint();
	}
}

// ---------------- Entorn VGI: Funcions locals a main.cpp

// Log2: Càlcul del log base 2 de num
int Log2(int num)
{
	int tlog;

	if (num >= 8192) tlog = 13;
	else if (num >= 4096) tlog = 12;
	else if (num >= 2048) tlog = 11;
	else if (num >= 1024) tlog = 10;
	else if (num >= 512) tlog = 9;
	else if (num >= 256) tlog = 8;
	else if (num >= 128) tlog = 7;
	else if (num >= 64) tlog = 6;
	else if (num >= 32) tlog = 5;
	else if (num >= 16) tlog = 4;
	else if (num >= 8) tlog = 3;
	else if (num >= 4) tlog = 2;
	else if (num >= 2) tlog = 1;
	else tlog = 0;

	return tlog;
}


// -------------------- FUNCIONS CORBES SPLINE i BEZIER

// llegir_ptsC: Llegir punts de control de corba (spline o Bezier) d'un fitxer .crv. 
//				Retorna el nombre de punts llegits en el fitxer.
//int llegir_pts(CString nomf)
int llegir_ptsC(const char* nomf)
{
	int i, j;
	FILE* fd;

	// Inicialitzar vector punts de control de la corba spline
	for (i = 0; i < MAX_PATCH_CORBA; i = i++)
	{	PC_t[i].x = 0.0;
		PC_t[i].y = 0.0;
		PC_t[i].z = 0.0;
	}

	//	ifstream f("altinicials.dat",ios::in);
	//    f>>i; f>>j;
	if ((fd = fopen(nomf, "rt")) == NULL)
	{
		//LPCWSTR texte1 = reinterpret_cast<LPCWSTR> ("ERROR:");
		//LPCWSTR texte2 = reinterpret_cast<LPCWSTR> ("File .crv was not opened");
		//MessageBox(texte1, texte2, MB_OK);
		fprintf(stderr, "ERROR: File .crv was not opened");
		return false;
	}
	fscanf(fd, "%d \n", &i);
	if (i == 0) return false;
	else {
		for (j = 0; j < i; j = j++)
		{	//fscanf(fd, "%f", &corbaSpline[j].x);
			//fscanf(fd, "%f", &corbaSpline[j].y);
			//fscanf(fd, "%f \n", &corbaSpline[j].z);
			fscanf(fd, "%lf %lf %lf \n", &PC_t[j].x, &PC_t[j].y, &PC_t[j].z);

		}
	}
	fclose(fd);

	return i;
}


// Entorn VGI. OnFull_Screen: Funció per a pantalla completa
void OnFull_Screen(GLFWmonitor* monitor, GLFWwindow *window)
{   
	//int winPosX, winPosY;
	//winPosX = 0;	winPosY = 0;

	fullscreen = !fullscreen;

	if (fullscreen) {	// backup window position and window size
						//glfwGetWindowPos(window, &winPosX, &winPosY);
						//glfwGetWindowSize(window, &width_old, &height_old);

						// Get resolution of monitor
						const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

						w = mode->width;	h = mode->height;
						// Switch to full screen
						glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
					}
	else {	// Restore last window size and position
			glfwSetWindowMonitor(window, nullptr, 216, 239, 640, 480, mode->refreshRate);
		}
	}

// -------------------- TRACTAMENT ERRORS
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

// Retrieving monitors
//    int countM;
//   GLFWmonitor** monitors = glfwGetMonitors(&countM);

// Retrieving video modes of the monitor
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

// Initialize API
	//InitAPI();

// Initialize Application control varibles
	InitGL();

// ------------- Entorn VGI: Callbacks
// Set GLFW event callbacks. I removed glfwSetWindowSizeCallback for conciseness
	glfwSetWindowSizeCallback(window, OnSize);										// - Windows Size in screen Coordinates
	glfwSetFramebufferSizeCallback(window, OnSize);									// - Windows Size in Pixel Coordinates
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)OnMouseButton);			// - Directly redirect GLFW mouse button events
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)OnMouseMove);				// - Directly redirect GLFW mouse position events
	glfwSetScrollCallback(window, (GLFWscrollfun)OnMouseWheel);						// - Directly redirect GLFW mouse wheel events
	//glfwSetCharCallback(window, OnTextDown);										// - Directly redirect GLFW char events
	glfwSetErrorCallback(error_callback);											// Error callback
	glfwSetWindowRefreshCallback(window, (GLFWwindowrefreshfun)OnPaint);			// - Callback to refresh the screen

// Entorn VGI; Timer: Lectura temps
	float previous = glfwGetTime();

// Entorn VGI.ImGui: Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

// Entorn VGI.ImGui: Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

// Entorn VGI.ImGui: Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
// Entorn VGI.ImGui: End Setup Dear ImGui context

	//Dibuixa els eixos

	if (!eixos_programID) eixos_programID = shaderEixos.loadFileShaders(".\\shaders\\eixos.VERT", ".\\shaders\\eixos.FRAG");
	if (!shader_programID) shader_programID = shaderLighting.loadFileShaders(".\\shaders\\gouraud_shdrML.vert", ".\\shaders\\gouraud_shdrML.frag");
	if (!eixos_Id) eixos_Id = deixos(); // Funció que defineix els Eixos Coordenades Món com un VAO.
	FonsB();

// Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {  
// Entorn VGI. Timer: common part, do this only once
		now = glfwGetTime();
		delta = now - previous;
		previous = now;

// Entorn VGI. Timer: for each timer do this
		time -= delta;
		if ((time <= 0.0) && (satelit || anima)) OnTimer();

// Poll for and process events
		glfwPollEvents();

// Entorn VGI.ImGui: Dibuixa menú ImGui
		draw_Menu_ImGui();

// Crida a OnPaint() per redibuixar l'escena
		OnPaint(window);

// Entorn VGI.ImGui: Capta dades del menú InGui
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