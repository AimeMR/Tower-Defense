#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"
#include "ImGui\imgui_impl_glfw.h"
#include "ImGui\imgui_impl_opengl3.h"
#include "ImGui\nfd.h" // Native File Dialog

#include "stdafx.h"
#include "shader.h"
#include "visualitzacio.h"
#include "escena.h"
#include "menu.h"

// --- Variables Globales ---
bool show_menu_inicio = true;
bool show_menu_ajustes = false;
bool show_jugar = false;
bool show_menu_creditos = false;
bool show_menu_pruebas = false; 
bool juego_pausado = false;

//--------------Variables globales para modo pruebas------------------
bool enable_debug_mode = true; // Variable de configuracion para mostrar el boton (CAMBIAR AQUI)
bool debug_detener_tiempo = false;
bool debug_resetear_todo = false;
int debug_id_enemigo_spawn = 0; // ID por defecto (Basic)
bool debug_solicitar_spawn = false;




bool show_menu_tienda = false;
bool show_menu_millores = false;
int torreSeleccionada = -1;



// Variable de Brillo (1.0 = Normal, 0.0 = Negro total)
float nivelBrillo = 1.0f;

// --- PROTOTIPOS DE FUNCIONES ---
void menuPausa(bool& salir, const ImGuiViewport* viewport);
void menuPruebas(bool& salir); // Prototipo menu pruebas
void menuAjustes();
void menuCreditos();
void iniciarPartida(bool& salir);
ImVec2 colocarBoton(float porX, float porY);
void cambiarEstiloBotones();
void regresarEstiloBotones();
void cambiarEstiloSlider();
void regresarEstiloSlider();
void aplicarEfectoBrillo();




void menuTiendaTorres(bool& salir, const ImGuiViewport* viewport);
void menuMillores(bool& salir, const ImGuiViewport* viewport);



#include "stb_image.h"
#include <iostream>
using namespace std;


GLuint LoadTexture(const char* path) {
	GLuint textureID; int w, h, c;
	unsigned char* data = stbi_load(path, &w, &h, &c, 4);
	if (!data) { std::cout << "Error loading: " << path << "\n"; return 0; }
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
	return textureID;
}



struct TorreInfo {
	const char* nombre;
	ImTextureID icono;
	int precio;
};

// Variables globals
TorreInfo torres_disponibles[] = {
	{ "Metralladora", nullptr, 3 },
	{ "Congeladora", nullptr, 6 },
	{ "Laser", nullptr, 7 },
	{ "Gas", nullptr, 10 },
	{ "Sniper", nullptr, 6 }
};
int num_torres = sizeof(torres_disponibles) / sizeof(TorreInfo);

int mejoras_torres[5][5] = { 0 };









//--------------------- A PARTIR D'AQUÍ EL NOSTRE CODI ---------------------
void menu(bool& salir)
{
	// Inicializa frame de ImGui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 1. APLICAR EFECTO DE BRILLO (Overlay)
	// Se dibuja antes que todo lo demás para que afecte al juego 3D de fondo
	// pero NO a los menús que dibujamos ahora encima.
	aplicarEfectoBrillo();

	// Obtenemos viewport para posicionar ventana
	const ImGuiViewport* viewport = ImGui::GetMainViewport();



	GLuint LAS = LoadTexture("C:/Users/uanra/Pictures/VGI/laser.png");
	GLuint BER = LoadTexture("C:/Users/uanra/Pictures/VGI/berserker.png");
	GLuint TES = LoadTexture("C:/Users/uanra/Pictures/VGI/tesla.png");
	GLuint GAS = LoadTexture("C:/Users/uanra/Pictures/VGI/gas.png");
	GLuint GEL = LoadTexture("C:/Users/uanra/Pictures/VGI/gel.png");

	for (int i = 0; i < num_torres; i++) {
		torres_disponibles[0].icono = (ImTextureID)BER;
		torres_disponibles[1].icono = (ImTextureID)TES;
		torres_disponibles[2].icono = (ImTextureID)LAS;
		torres_disponibles[3].icono = (ImTextureID)GAS;
		torres_disponibles[4].icono = (ImTextureID)GEL;
	}


	// =========================================================
	// MENÚ DE INICIO (Pantalla Completa)
	// =========================================================
	if (show_menu_inicio)
	{
		// Configuración ventana fullscreen transparente
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		static ImGuiWindowFlags flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

		if (ImGui::Begin("Menu_Principal_Fullscreen", &show_menu_inicio, flags))
		{
			// Aplicamos estilos y fuente 
			cambiarEstiloBotones();
			ImGui::SetWindowFontScale(1.5f);

			// --- BOTÓN JUGAR ---
			ImVec2 btnSize = colocarBoton(0.5f, 0.35f);
			if (ImGui::Button("Jugar", btnSize))
			{
				show_menu_inicio = false;
				show_jugar = true;
				juego_pausado = false;
			}

			// --- BOTÓN AJUSTES ---
			btnSize = colocarBoton(0.5f, 0.50f);
			if (ImGui::Button("Ajustes", btnSize))
			{
				show_menu_inicio = false;
				show_menu_ajustes = true;
			}

			// --- BOTÓN CREDITOS ---
			btnSize = colocarBoton(0.5f, 0.65f);
			if (ImGui::Button("Creditos", btnSize))
			{
				show_menu_inicio = false;
				show_menu_creditos = true;
			}

			// --- BOTÓN MODO PRUEBAS (SOLO SI ESTA ACTIVO) ---
			if (enable_debug_mode)
			{
				btnSize = colocarBoton(0.5f, 0.80f);
				// Cambiamos color para diferenciarlo
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
				if (ImGui::Button("Modo Pruebas", btnSize))
				{
					show_menu_inicio = false;
					show_menu_pruebas = true;
				}
				ImGui::PopStyleColor();
			}

			// --- BOTÓN SALIR --- (Esquina inferior izquierda)
			btnSize = colocarBoton(0.1f, 0.90f);
			if (ImGui::Button("Salir", btnSize))
			{
				show_menu_inicio = false;
				salir = true;
			}

			// Restauramos estilos y fuente normal
			ImGui::SetWindowFontScale(1.0f);
			regresarEstiloBotones();
		}
		ImGui::End();
	}
	// =========================================================
	// ESTADO DE JUEGO (HUD + PAUSA)
	// =========================================================
	else if (show_jugar)
	{
		iniciarPartida(salir);
	}
	// =========================================================
	// MODO PRUEBAS
	// =========================================================
	else if (show_menu_pruebas)
	{
		menuPruebas(salir);
	}
	// =========================================================
	// MENU AJUSTES
	// =========================================================
	else if (show_menu_ajustes)
	{
		menuAjustes();
	}
	// =========================================================
	// CREDITOS
	// =========================================================
	else if (show_menu_creditos)
	{
		menuCreditos();
	}

	// =========================================================
	// MENU TIENDA DE TORRES
	// =========================================================
	else if (show_menu_tienda)
	{
		menuTiendaTorres(salir, viewport);
	}

	// =========================================================
	// MENU MILLORA TORRES
	// =========================================================
	else if (show_menu_millores)
	{
		menuMillores(salir, viewport);
	}

	ImGui::Render();
}

void iniciarPartida(bool& salir)
{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	// -----------------------------------------------------
	// HUD (Botón Pausa)
	// Solo visible si NO está pausado para no superponerse
	// -----------------------------------------------------
	if (!juego_pausado)
	{
		ImGuiWindowFlags hudFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;
		float padding = 10.0f;

		// Posicionamiento esquina superior derecha
		ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x - 60.0f - padding, viewport->WorkPos.y + padding), ImGuiCond_Always);

		// Fondo invisible
		ImGui::SetNextWindowBgAlpha(0.0f);

		if (ImGui::Begin("HudPausa", NULL, hudFlags))
		{
			cambiarEstiloBotones();
			if (ImGui::Button("||", ImVec2(50, 50)))
			{
				juego_pausado = true;
			}
			regresarEstiloBotones();
		}
		ImGui::End();
	}

	if (juego_pausado)
	{
		menuPausa(salir, viewport);
	}

	if (!show_menu_tienda)
	{
		ImGuiWindowFlags hudFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;

		float padding = 20.0f;

		// 🟢 POSICIÓ INFERIOR DRETA
		ImGui::SetNextWindowPos(
			ImVec2(
				viewport->WorkPos.x + viewport->WorkSize.x - 100.0f - padding,
				viewport->WorkPos.y + viewport->WorkSize.y - 100.0f - padding
			),
			ImGuiCond_Always
		);

		ImGui::SetNextWindowBgAlpha(0.0f);

		if (ImGui::Begin("Tenda", NULL, hudFlags))
		{
			cambiarEstiloBotones();
			if (ImGui::Button("Tienda", ImVec2(80, 80)))// Pots posar icona més endavant
			{
				show_menu_tienda = true;
			}
			regresarEstiloBotones();
		}
		ImGui::End();
	}


	if (show_menu_tienda)
	{
		menuTiendaTorres(salir, viewport);
	}


	if (!show_menu_millores)
	{
		ImGuiWindowFlags hudFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;

		float padding = 20.0f;

		// 🟢 POSICIÓ A LA MEITAT DE LA PART DRETA
		ImGui::SetNextWindowPos(
			ImVec2(
				viewport->WorkPos.x + viewport->WorkSize.x - 120.0f,      // cap a la dreta
				viewport->WorkPos.y + viewport->WorkSize.y * 0.35f        // meitat vertical (lleugerament més amunt)
			),
			ImGuiCond_Always
		);

		ImGui::SetNextWindowBgAlpha(0.0f);

		if (ImGui::Begin("Millores", NULL, hudFlags))
		{
			cambiarEstiloBotones();
			if (ImGui::Button("Millores", ImVec2(80, 80)))
			{
				show_menu_millores = true;   // 🔴 Obrir menú de millores (estava malament posat a tienda)
				show_menu_tienda = false;
			}
			regresarEstiloBotones();
		}
		ImGui::End();
	}



	if (show_menu_millores)
	{
		menuMillores(salir, viewport);
	}


}

void menuPruebas(bool& salir)
{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	// Flags para panel lateral derecho
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize;

	// Dimensiones del panel lateral
	float panelWidth = 220.0f;
	float panelHeight = 450.0f; // Aumentamos un poco la altura para que quepa el input

	// Posicionamiento pegado a la derecha, centrado verticalmente
	ImGui::SetNextWindowPos(ImVec2(viewport->WorkSize.x - panelWidth, (viewport->WorkSize.y - panelHeight) * 0.5f));
	ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight));

	// Fondo semitransparente
	ImGui::SetNextWindowBgAlpha(0.5f);

	if (ImGui::Begin("PanelPruebas", NULL, flags))
	{
		cambiarEstiloBotones();
		ImGui::SetWindowFontScale(1.2f);

		// Titulo panel
		ImGui::Text("Debug Tools");
		ImGui::Separator();
		ImGui::Spacing();

		// Tamaño fijo botones
		ImVec2 btnSize(180, 40);

		// --- BOTON 1: PAUSAR / ACTIVAR TIMER ---
		const char* txtBoton = debug_detener_tiempo ? "REANUDAR TIMER" : "PAUSAR TIMER";
		if (ImGui::Button(txtBoton, btnSize))
		{
			debug_detener_tiempo = !debug_detener_tiempo;
		}
		ImGui::Spacing();

		// --- BOTON 2: RESETEAR Y MATAR ---
		if (ImGui::Button("MATAR Y REINICIAR", btnSize))
		{
			debug_resetear_todo = true;
			debug_detener_tiempo = false;
		}
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// --- BOTON 3: SPAWN ENEMIGO (MODIFICADO) ---
		ImGui::Text("Spawn Manual:");

		// Hueco interactuable para el ID (Input Int)
		// PushItemWidth ajusta el ancho de la caja de texto
		ImGui::PushItemWidth(180);
		// "##ID" oculta la etiqueta pero sirve de identificador unico
		ImGui::InputInt("##ID_Enemigo", &debug_id_enemigo_spawn);
		ImGui::PopItemWidth();

		// Boton para ejecutar la orden usando el numero de arriba
		if (ImGui::Button("SPAWNEAR ID", btnSize))
		{
			debug_solicitar_spawn = true; // Activa la bandera para el main
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Boton 4 (Vacio por ahora)
		if (ImGui::Button("Test 4: Reset", btnSize)) {}

		ImGui::Spacing();

		// Boton volver
		if (ImGui::Button("Volver Menu", btnSize))
		{
			show_menu_pruebas = false;
			show_menu_inicio = true;
		}

		ImGui::SetWindowFontScale(1.0f);
		regresarEstiloBotones();
	}
	ImGui::End();
}
void menuPausa(bool& salir, const ImGuiViewport* viewport)
{
	// Configuración ventana Fullscreen
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	// Fondo negro menu
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

	// Transparencia del fondo (0.8 = bastante opaco)
	ImGui::SetNextWindowBgAlpha(0.8f);

	ImGuiWindowFlags pausaFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::Begin("Menu_Pausa_Fullscreen", &juego_pausado, pausaFlags))
	{
		// Establece estilos de botones personalizados
		cambiarEstiloBotones();
		ImGui::SetWindowFontScale(1.5f);

		// Ajuste vertical para el contenido
		ImGui::SetCursorPosY(viewport->Size.y * 0.2f);

		// --- BOTONES DEL MENÚ DE PAUSA ---

		ImVec2 btnSize = colocarBoton(0.5f, 0.40f);
		if (ImGui::Button("Continuar", btnSize))
		{
			juego_pausado = false;
		}

		btnSize = colocarBoton(0.5f, 0.55f);
		if (ImGui::Button("Menu Principal", btnSize))
		{
			juego_pausado = false;
			show_jugar = false;
			show_menu_inicio = true;
		}

		btnSize = colocarBoton(0.1f, 0.90f);
		if (ImGui::Button("Salir Juego", btnSize))
		{
			salir = true;
		}

		// Devuelve estilo original a la fuente y botones
		ImGui::SetWindowFontScale(1.0f);
		regresarEstiloBotones();
	}
	ImGui::End();

	//Quita el fondo de color negro y deja el pre-establecido (claro)
	ImGui::PopStyleColor();
}

void menuAjustes()
{
	show_menu_inicio = false;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	// Configuración ventana Fullscreen (Estilo Industrial)
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	// Fondo negro menu
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	// Transparencia del fondo
	ImGui::SetNextWindowBgAlpha(0.8f);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::Begin("Menu_Ajustes_Fullscreen", &show_menu_ajustes, flags))
	{
		ImGui::SetWindowFontScale(1.5f);

		// --- TÍTULO ---
		const char* titulo = "CONFIGURACION";
		ImVec2 textSize = ImGui::CalcTextSize(titulo);
		ImGui::SetCursorPos(ImVec2((viewport->Size.x - textSize.x) * 0.5f, viewport->Size.y * 0.2f));
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), titulo);

		ImGui::Spacing();

		// --- SLIDER DE BRILLO ---
		// Centramos el slider
		float sliderWidth = 300.0f;
		ImGui::SetCursorPos(ImVec2((viewport->Size.x - sliderWidth) * 0.5f, viewport->Size.y * 0.4f));
		ImGui::PushItemWidth(sliderWidth);

		// Aplicamos estilo personalizado al slider (Cobre y con Borde)
		cambiarEstiloSlider();

		// Etiqueta encima del slider
		ImGui::Text("Brillo de Pantalla");
		ImGui::SetCursorPosX((viewport->Size.x - sliderWidth) * 0.5f); // Recolocamos X porque Text mueve cursor

		ImGui::SliderFloat("##brillo", &nivelBrillo, 0.1f, 1.0f, "Brillo: %.2f");

		regresarEstiloSlider();
		ImGui::PopItemWidth();


		// --- BOTÓN CERRAR ---
		cambiarEstiloBotones();

		// Forzamos la fuente grande para este botón
		ImGui::SetWindowFontScale(1.5f);

		// Usamos colocarBoton para situarlo abajo, igual que el botón "Salir"
		ImVec2 btnSize = colocarBoton(0.5f, 0.80f);
		if (ImGui::Button("Guardar y Cerrar", btnSize))
		{
			show_menu_ajustes = false;
			show_menu_inicio = true;
		}

		ImGui::SetWindowFontScale(1.0f);
		regresarEstiloBotones();
	}
	ImGui::End();

	// Quita el fondo de color negro
	ImGui::PopStyleColor();
}

void menuCreditos()
{
	show_menu_inicio = false;
	ImGui::Begin("Menu Creditos", &show_menu_creditos);
	if (ImGui::Button("Cerrar"))
	{
		show_menu_creditos = false;
		show_menu_inicio = true;
	}
	ImGui::End();
}


void menuTiendaTorres(bool& salir, const ImGuiViewport* viewport)
{
	// Pantalla completa
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	// Fons fosc com el menu pausa
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1));
	ImGui::SetNextWindowBgAlpha(0.85f);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::Begin("Menu_TiendaTorres", &show_menu_tienda, flags))
	{
		ImGui::SetWindowFontScale(1.5f);

		// ----- TÍTOL -----
		const char* titulo = "TIENDA DE TORRES";
		ImVec2 textSize = ImGui::CalcTextSize(titulo);
		ImGui::SetCursorPos(ImVec2(
			(viewport->Size.x - textSize.x) * 0.5f,
			viewport->Size.y * 0.10f
		));
		ImGui::TextColored(ImVec4(1, 1, 1, 1), titulo);

		ImGui::SetWindowFontScale(1.2f);
		ImGui::Spacing(); ImGui::Spacing();

		// ------ MOSTRAR TORRES ------
		float panelWidth = 300.0f;
		float panelHeight = 120.0f;

		ImGui::SetCursorPosY(viewport->Size.y * 0.20f);

		for (int i = 0; i < num_torres; i++)
		{
			TorreInfo& t = torres_disponibles[i];

			ImGui::SetCursorPosX((viewport->Size.x - panelWidth) * 0.5f);

			ImGui::BeginChild(
				t.nombre,
				ImVec2(panelWidth, panelHeight),
				true,
				ImGuiWindowFlags_NoScrollbar
			);

			ImGui::Columns(2, nullptr, false);

			// ----- ICONO -----
			ImGui::Image(t.icono, ImVec2(96, 96));

			ImGui::NextColumn();

			// ----- DATOS (TEXT BLANC) -----
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));

			ImGui::Text("%s", t.nombre);
			ImGui::Text("Precio: %d", t.precio);

			ImGui::PopStyleColor(); // ← RESTAURAR COLOR

			ImGui::Spacing();

			cambiarEstiloBotones();
			if (ImGui::Button("Comprar", ImVec2(120, 40)))
			{
				torreSeleccionada = i;   // marcar torre
				show_menu_tienda = false;
			}
			regresarEstiloBotones();

			ImGui::Columns(1);
			ImGui::EndChild();

			ImGui::Spacing();
		}


		// ----- BOTÓN VOLVER -----
		ImVec2 btnSize = ImVec2(200, 50);
		ImGui::SetCursorPos(ImVec2(
			(viewport->Size.x - btnSize.x) * 0.5f,
			viewport->Size.y * 0.85f
		));

		cambiarEstiloBotones();
		if (ImGui::Button("Volver", btnSize))
		{
			show_menu_tienda = false;
			show_menu_inicio = true;
		}
		regresarEstiloBotones();

		ImGui::SetWindowFontScale(1.0f);
	}

	ImGui::End();
	ImGui::PopStyleColor();
}

void menuMillores(bool& salir, const ImGuiViewport* viewport)
{
	// Pantalla completa
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1));
	ImGui::SetNextWindowBgAlpha(0.85f);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::Begin("Menu_Millores", &show_menu_millores, flags))
	{
		ImGui::SetWindowFontScale(1.5f);

		// ----- TÍTOL -----
		const char* titulo = "MEJORAS DE TORRES";
		ImGui::SetCursorPos(ImVec2(20.0f, viewport->Size.y * 0.10f)); // Títol a l'esquerra
		ImGui::TextColored(ImVec4(1, 1, 1, 1), titulo);

		ImGui::SetWindowFontScale(1.2f);
		ImGui::Spacing(); ImGui::Spacing();

		// ------ MOSTRAR TORRES ------
		float panelWidth = 300.0f;
		float panelHeight = 150.0f;
		float startX = 20.0f; // marge esquerre
		float startY = viewport->Size.y * 0.20f;

		for (int i = 0; i < num_torres; i++)
		{
			TorreInfo& t = torres_disponibles[i];

			// Columna esquerra (Icon + Nom + Preu)
			ImGui::SetCursorPos(ImVec2(startX, startY + i * (panelHeight + 10)));
			ImGui::BeginChild(
				t.nombre,
				ImVec2(panelWidth, panelHeight),
				true,
				ImGuiWindowFlags_NoScrollbar
			);

			ImGui::Columns(2, nullptr, false);
			ImGui::Image(t.icono, ImVec2(96, 96));
			ImGui::NextColumn();

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
			ImGui::Text("%s", t.nombre);
			ImGui::Text("Precio: %d", t.precio);
			ImGui::PopStyleColor();

			ImGui::Columns(1);
			ImGui::EndChild();

			// Columna dreta (Botons millora)
			ImGui::SetCursorPos(ImVec2(startX + panelWidth + 20, startY + i * (panelHeight + 10)));

			for (int m = 0; m < 5; m++)
			{
				bool activat = mejoras_torres[i][m];

				ImVec4 color = activat ?
					ImVec4(1.0f, 0.0f, 0.0f, 1.0f) : // Vermell
					ImVec4(0.5f, 0.5f, 0.5f, 1.0f);  // Gris

				ImGui::PushStyleColor(ImGuiCol_Button, color);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 20.0f);

				char id[32];
				sprintf(id, "##m%d_%d", i, m);

				if (ImGui::Button(id, ImVec2(25, 25)))
					mejoras_torres[i][m] = !mejoras_torres[i][m];

				ImGui::PopStyleVar();
				ImGui::PopStyleColor(3);

				if (m < 4) ImGui::SameLine(); // 4 botons per fila
			}
		}

		// BOTÓ VOLVER
		ImVec2 btnSize = ImVec2(200, 50);
		ImGui::SetCursorPos(ImVec2(
			(viewport->Size.x - btnSize.x) * 0.5f,
			viewport->Size.y * 0.85f
		));

		cambiarEstiloBotones();
		if (ImGui::Button("Volver", btnSize))
		{
			show_menu_millores = false;
			show_menu_inicio = true;
		}
		regresarEstiloBotones();

		ImGui::SetWindowFontScale(1.0f);
	}

	ImGui::End();
	ImGui::PopStyleColor();
}




//////////////////////////////////////////////////////////////////
//					Funciones de diseño							//
//////////////////////////////////////////////////////////////////

// Función para el efecto de oscurecer pantalla
// Crea una ventana falsa que no se puede interactuar con ella
void aplicarEfectoBrillo()
{
	// Si el brillo es máximo, no dibujamos nada para ahorrar recursos
	if (nivelBrillo >= 0.99f) return;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	// Posicionamos una ventana invisible sobre toda la pantalla
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	// Calculamos la opacidad negra: 
	// Brillo 1.0 -> Alpha 0.0 (Transparente)
	// Brillo 0.1 -> Alpha 0.9 (Casi negro)
	ImGui::SetNextWindowBgAlpha(1.0f - nivelBrillo);

	// Fondo negro (evita niebla blanca en temas claros)
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1));

	// Flags importantes: NoInputs permite que los clicks pasen a través de esta capa oscura al juego
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize;

	if (ImGui::Begin("OverlayBrillo", nullptr, flags))
	{
		// Ventana vacía, solo sirve para tintar
	}
	ImGui::End();

	ImGui::PopStyleColor();
}

// Posicionar botones centrados en porcentaje relativo a la ventana
ImVec2 colocarBoton(float porX, float porY)
{
	// Tamaño del botón
	ImVec2 button_size(200, 50);

	// Obtener tamaño de la ventana actual
	ImVec2 window_size = ImGui::GetWindowSize();

	// Calcular posición absoluta
	float posX = (window_size.x * porX) - (button_size.x * 0.5f);
	float posY = (window_size.y * porY) - (button_size.y * 0.5f);

	// Posición del cursor
	ImGui::SetCursorPos(ImVec2(posX, posY));

	return button_size;
}

void cambiarEstiloBotones()
{
	// --- COLORES ---

	// Botón Normal: Plateado Oscuro
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.40f, 0.42f, 0.45f, 1.0f));
	// Botón Hover (Ratón encima): Plateado más claro
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.55f, 0.57f, 0.60f, 1.0f));
	// Botón Activo (Click): Gris oscuro 
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.27f, 0.30f, 1.0f));
	// Borde: Color cobre 
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.85f, 0.55f, 0.25f, 1.0f));

	// --- VARIABLES ---
	// Grosor del borde
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4.0f);
	// Redondeo de esquinas
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
}

void regresarEstiloBotones()
{
	// Sacamos los 4 colores
	ImGui::PopStyleColor(4);
	// Sacamos las 2 variables
	ImGui::PopStyleVar(2);
}

void cambiarEstiloSlider()
{
	// Fondo del slider (Canal): Gris Oscuro
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.20f, 0.20f, 0.20f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.30f, 0.30f, 0.30f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));

	// Agarradera (Knob): Color Cobre 
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.85f, 0.55f, 0.25f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.95f, 0.65f, 0.35f, 1.0f));

	// Borde del slider: Cobre
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.85f, 0.55f, 0.25f, 1.0f));

	// Variables estéticas
	// AQUI SE AGREGA EL BORDE AL SLIDER (MANTENIDO 2.0f para visibilidad):
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 3.0f);
}

void regresarEstiloSlider()
{
	// Sacamos los 6 colores
	ImGui::PopStyleColor(6);
	// Sacamos las 3 variables
	ImGui::PopStyleVar(3);
}