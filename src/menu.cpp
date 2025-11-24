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

//------------- Variables Globales -------------
bool show_menu_inicio = true;
bool show_menu_ajustes = false;
bool show_jugar = false;
bool show_menu_creditos = false;
bool show_menu_pruebas = false; 
bool juego_pausado = false;

//------------- Variables globales para modo pruebas------------------
bool enable_debug_mode = true; // Variable de configuracion para mostrar el boton (CAMBIAR AQUI)
bool debug_detener_tiempo = false;
bool debug_resetear_todo = false;
int debug_id_enemigo_spawn = 0; // ID por defecto (Basic)
int debug_num_enemigo_spawn = 1; // Cantidad de enemigos a spawnear
bool debug_solicitar_spawn = false;
bool show_submenu_shadows = false;
float debug_speedMult = 1.0f;

//------------- Variables para Sombras y Render -----------------------
float debug_lightDir[3] = { -1.0f, -1.0f, 1.0f }; // Valor inicial por defecto
float debug_boxSize = 25.0f;
float debug_nearPlane = 1.0f;
float debug_farPlane = 100.0f;
int debug_renderMode = 0; // 0 = DEFAULT

// Variable de Brillo (1.0 = Normal, 0.0 = Negro total)
float nivelBrillo = 1.0f;

//------------- PROTOTIPOS DE FUNCIONES ------------------
void menuPausa(bool& salir, const ImGuiViewport* viewport);
void menuPruebas(bool& salir); 
void menuAjustes();
void menuCreditos();
void iniciarPartida(bool& salir);
void menuShadows();

ImVec2 colocarBoton(float porX, float porY);
void cambiarEstiloBotones();
void regresarEstiloBotones();
void cambiarEstiloSlider();
void regresarEstiloSlider();
void aplicarEfectoBrillo();


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
		// Si el submenu de sombras está activo, lo dibujamos
		if (show_submenu_shadows)
		{
			menuShadows();
		}
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
}

void menuPruebas(bool& salir)
{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	// Flags para panel lateral derecho
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize;

	// Dimensiones del panel lateral
	float panelWidth = 220.0f;
	float panelHeight = 550.0f; 

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

		// --- BOTON 3: SPAWN ENEMIGO ---
		ImGui::Text("Spawn Manual:");

		// Hueco interactuable para el ID (Input Int)
		// PushItemWidth ajusta el ancho de la caja de texto
		ImGui::PushItemWidth(180);
		// "##ID" oculta la etiqueta pero sirve de identificador unico
		ImGui::InputInt("##ID_Enemigo", &debug_id_enemigo_spawn);
		ImGui::PopItemWidth();

		// PushItemWidth ajusta el ancho de la caja de texto
		ImGui::PushItemWidth(180);
		// "##ID" oculta la etiqueta pero sirve de identificador unico
		ImGui::InputInt("##Num_Enemigo", &debug_num_enemigo_spawn);
		ImGui::PopItemWidth();
		// Boton para ejecutar la orden usando el numero de arriba
		if (ImGui::Button("SPAWNEAR ID", btnSize))
		{
			debug_solicitar_spawn = true; // Activa la bandera para el main
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// --- BOTON 4: CONFIGURAR SOMBRAS ---
		// Abre/Cierra el submenu de sombras
		if (ImGui::Button("Config. Sombras", btnSize))
		{
			show_submenu_shadows = !show_submenu_shadows;
		}
		ImGui::Spacing();

		// --- DESPLEGABLE RENDER MODE ---
		ImGui::Text("Render Mode:");

		// LISTA DE MODOS
		const char* items[] = {
			"0: Default (Lit)",
			"1: Normals",
			"2: Shadow Map",
			"3: Position",
			"4: Picking ID",
			"5: Wireframe"
		};


		cambiarEstiloSlider(); 
		ImGui::PushItemWidth(180);

		// Creamos el Desplegable
		ImGui::Combo("##renderMode", &debug_renderMode, items, IM_ARRAYSIZE(items));

		ImGui::PopItemWidth();
		regresarEstiloSlider();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// --- BOTON 5: Velocidad ---
		ImGui::Text("Speed:");

		// PushItemWidth ajusta el ancho de la caja de texto
		ImGui::PushItemWidth(180);
		ImGui::InputFloat("##Velocidad", &debug_speedMult);
		ImGui::PopItemWidth();

		ImGui::Spacing();
		ImGui::Separator();
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

//Sub-menu de prueba para control de los parametros del renderizado de sombras
void menuShadows()
{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	// Configuración del Submenú
	ImVec2 size(300, 300);
	// Posición a la izquierda del panel de pruebas
	ImGui::SetNextWindowPos(ImVec2(viewport->WorkSize.x - 550.0f, (viewport->WorkSize.y - size.y) * 0.5f));
	ImGui::SetNextWindowSize(size);

	// Estilo Panel (Igual que Ajustes)
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.17f, 0.9f)); // Gris oscuro
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.85f, 0.55f, 0.25f, 1.0f)); // Cobre
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);

	if (ImGui::Begin("Sombras & Luz", &show_submenu_shadows, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		ImGui::Text("Parametros RenderShadows");
		ImGui::Separator();
		ImGui::Spacing();

		cambiarEstiloSlider(); // Estilo para los inputs

		// 1. Vector Direccion Luz (3 parametros)
		ImGui::Text("Direccion Luz (x, y, z)");
		ImGui::PushItemWidth(250);
		ImGui::InputFloat3("##lightDir", debug_lightDir);
		ImGui::PopItemWidth();
		ImGui::Spacing();

		// 2. Box Size
		ImGui::Text("Box Size");
		ImGui::PushItemWidth(250);
		ImGui::InputFloat("##boxSize", &debug_boxSize, 1.0f, 10.0f);
		ImGui::PopItemWidth();
		ImGui::Spacing();

		// 3. Near Plane
		ImGui::Text("Near Plane");
		ImGui::PushItemWidth(250);
		ImGui::InputFloat("##near", &debug_nearPlane, 0.1f, 1.0f);
		ImGui::PopItemWidth();
		ImGui::Spacing();

		// 4. Far Plane
		ImGui::Text("Far Plane");
		ImGui::PushItemWidth(250);
		ImGui::InputFloat("##far", &debug_farPlane, 1.0f, 10.0f);
		ImGui::PopItemWidth();

		regresarEstiloSlider();

		ImGui::Spacing();
		if (ImGui::Button("Cerrar")) show_submenu_shadows = false;
	}
	ImGui::End();

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(2);
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