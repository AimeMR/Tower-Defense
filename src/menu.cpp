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
bool juego_pausado = false;

// Variable de Brillo (1.0 = Normal, 0.0 = Negro total)
float nivelBrillo = 1.0f;

// --- PROTOTIPOS DE FUNCIONES ---
void menuPausa(bool& salir, const ImGuiViewport* viewport);
void menuAjustes();
void menuCreditos();
void iniciarPartida(bool& salir);
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

		// AQUI ESTABA EL PROBLEMA: Faltaba NoResize y NoScrollbar
		static ImGuiWindowFlags flags = ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |      // <--- QUITA LA PESTAÑA DE TAMAÑO
			ImGuiWindowFlags_NoScrollbar |   // <--- QUITA BARRAS DE SCROLL
			ImGuiWindowFlags_NoCollapse |    // <--- EVITA COLAPSAR
			ImGuiWindowFlags_NoSavedSettings;

		if (ImGui::Begin("Menu_Principal_Fullscreen", &show_menu_inicio, flags))
		{
			// Aplicamos estilos y fuente 
			cambiarEstiloBotones();
			ImGui::SetWindowFontScale(1.5f);

			// --- BOTÓN JUGAR ---
			ImVec2 btnSize = colocarBoton(0.5f, 0.40f);
			if (ImGui::Button("Jugar", btnSize))
			{
				show_menu_inicio = false;
				show_jugar = true;
				juego_pausado = false;
			}

			// --- BOTÓN AJUSTES ---
			btnSize = colocarBoton(0.5f, 0.55f);
			if (ImGui::Button("Ajustes", btnSize))
			{
				show_menu_inicio = false;
				show_menu_ajustes = true;
			}

			// --- BOTÓN CREDITOS ---
			btnSize = colocarBoton(0.5f, 0.70f);
			if (ImGui::Button("Creditos", btnSize))
			{
				show_menu_inicio = false;
				show_menu_creditos = true;
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
	// -----------------------------------------------------
	if (!juego_pausado)
	{
		// Flags para el HUD (Aseguramos NoResize aquí también)
		ImGuiWindowFlags hudFlags = ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoResize; // <--- Seguridad extra

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

void menuPausa(bool& salir, const ImGuiViewport* viewport)
{
	// Configuración ventana Fullscreen
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	// Fondo negro menu
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

	// Transparencia del fondo (0.8 = bastante opaco)
	ImGui::SetNextWindowBgAlpha(0.8f);

	// NoDecoration ya incluye NoResize, así que esto debería estar bien, pero lo dejo explícito
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

	//Quita el fondo de color negro
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
	ImGui::SetNextWindowBgAlpha(0.0f);

	// Usamos NoDecoration para asegurar que no haya pestañas de resize
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::Begin("Menu_Ajustes_Fullscreen", &show_menu_ajustes, flags))
	{
		// --- TÍTULO GRANDE ---
		ImGui::SetWindowFontScale(3.0f); // Fuente muy grande
		const char* titulo = "CONFIGURACION";
		ImVec2 textSize = ImGui::CalcTextSize(titulo);

		// Centrar título arriba
		ImGui::SetCursorPos(ImVec2((viewport->Size.x - textSize.x) * 0.5f, viewport->Size.y * 0.15f));
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), titulo);

		ImGui::SetWindowFontScale(1.0f); // Restaurar fuente
		ImGui::Spacing();


		// --- RECUADRO DE AJUSTES (PANEL) ---
		// Definimos tamaño del panel
		ImVec2 panelSize(500, 200);
		// Posicionamos el panel en el centro
		ImGui::SetCursorPos(ImVec2((viewport->Size.x - panelSize.x) * 0.5f, viewport->Size.y * 0.40f));

		// Estilo del Panel (Recuadro): Fondo Gris oscuro metalizado y Borde Cobre
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.17f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.85f, 0.55f, 0.25f, 1.0f)); // Cobre
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f); // Esquinas redondeadas
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 3.0f); // Borde grueso visible

		// Creamos el Child (El true activa el borde)
		if (ImGui::BeginChild("PanelBrillo", panelSize, true))
		{
			// Centrado vertical de los elementos dentro del panel
			ImGui::SetCursorPosY(panelSize.y * 0.25f);

			// --- ESTILO DEL SLIDER Y TEXTO ---
			cambiarEstiloSlider();
			ImGui::SetWindowFontScale(1.5f); // Texto mediano

			// Etiqueta "Brillo de Pantalla" centrada
			const char* txtBrillo = "Brillo de Pantalla";
			ImVec2 txtSize = ImGui::CalcTextSize(txtBrillo);
			ImGui::SetCursorPosX((panelSize.x - txtSize.x) * 0.5f);
			ImGui::Text(txtBrillo);

			ImGui::Spacing();

			// Slider centrado
			float sliderWidth = panelSize.x * 0.8f;
			ImGui::SetCursorPosX((panelSize.x - sliderWidth) * 0.5f);
			ImGui::PushItemWidth(sliderWidth);

			ImGui::SliderFloat("##brillo", &nivelBrillo, 0.1f, 1.0f, "%.2f");

			ImGui::PopItemWidth();
			ImGui::SetWindowFontScale(1.0f);
			regresarEstiloSlider();
		}
		ImGui::EndChild();

		// Restaurar estilos del panel
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);

		
		// --- BOTÓN CERRAR ---
		cambiarEstiloBotones();
		ImGui::SetWindowFontScale(1.5f);
		// Usamos colocarBoton para situarlo abajo
		ImVec2 btnSize = colocarBoton(0.5f, 0.85f);
		if (ImGui::Button("Guardar y Cerrar", btnSize))
		{
			show_menu_ajustes = false;
			show_menu_inicio = true;
		}
		regresarEstiloBotones();

		ImGui::SetWindowFontScale(1.0f);
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