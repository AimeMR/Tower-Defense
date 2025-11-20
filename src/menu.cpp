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

//--------------------- A PARTIR D'AQUÍ EL NOSTRE CODI ---------------------
void menu(bool& salir)
{
	// Inicializa frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Obtenemos viewport para posicionar ventanas
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	// =========================================================
	// 1. MENÚ DE INICIO (Pantalla Completa)
	// =========================================================
	if (show_menu_inicio)
	{
		// Configuración ventana fullscreen
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		static ImGuiWindowFlags flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

		if (ImGui::Begin("Menu_Principal_Fullscreen", &show_menu_inicio, flags))
		{
			// Aplicamos estilos y fuente grande
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

			// Restauramos estilos y fuente
			ImGui::SetWindowFontScale(1.0f);
			regresarEstiloBotones();
		}
		ImGui::End();
	}
	// =========================================================
	// 2. JUEGO (HUD + PAUSA)
	// =========================================================
	else if (show_jugar)
	{
		iniciarPartida(salir);
	}
	// =========================================================
	// 3. OTROS MENÚS
	// =========================================================
	else if (show_menu_ajustes)
	{
		menuAjustes();
	}
	else if (show_menu_creditos)
	{
		menuCreditos();
	}

	ImGui::Render();
}

// Función auxiliar para posicionar botones centrados en porcentaje
ImVec2 colocarBoton(float porX, float porY)
{
	// 1. Definir tamaño del botón
	ImVec2 button_size(200, 50);

	// 2. Obtener tamaño de la ventana actual
	ImVec2 window_size = ImGui::GetWindowSize();

	// 3. Calcular posición ABSOLUTA
	float posX = (window_size.x * porX) - (button_size.x * 0.5f);
	float posY = (window_size.y * porY) - (button_size.y * 0.5f);

	// 4. FORZAR la posición del cursor
	ImGui::SetCursorPos(ImVec2(posX, posY));

	return button_size;
}

void iniciarPartida(bool& salir)
{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	// -----------------------------------------------------
	// A) HUD (Botón Pausa) - Solo visible si NO está pausado
	// -----------------------------------------------------
	if (!juego_pausado)
	{
		ImGuiWindowFlags hudFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;
		float padding = 10.0f;

		ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x - 60.0f - padding, viewport->WorkPos.y + padding), ImGuiCond_Always);

		// Fondo totalmente invisible (Alpha 0)
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

	// -----------------------------------------------------
	// B) MENÚ DE PAUSA (PANTALLA COMPLETA)
	// -----------------------------------------------------
	if (juego_pausado)
	{
		// 1. Configurar ventana Fullscreen
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		// 2. FORZAR COLOR NEGRO DE FONDO (Importante porque usas tema Light)
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Negro puro

		// 3. Definir Transparencia (Alpha 0.6 = 60% opaco)
		ImGui::SetNextWindowBgAlpha(0.8f);

		ImGuiWindowFlags pausaFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

		if (ImGui::Begin("Menu_Pausa_Fullscreen", &juego_pausado, pausaFlags))
		{
			// Estilos botones
			cambiarEstiloBotones();
			ImGui::SetWindowFontScale(1.5f);

			// Título opcional
			ImGui::SetCursorPosY(viewport->Size.y * 0.2f);

			// --- BOTONES ---
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

			ImGui::SetWindowFontScale(1.0f);
			regresarEstiloBotones();
		}
		ImGui::End();

		// IMPORTANTE: Sacar el color Negro de la pila de estilos
		ImGui::PopStyleColor();
	}
}

void menuAjustes()
{
	show_menu_inicio = false;
	ImGui::Begin("Menu Ajustes", &show_menu_ajustes);
	if (ImGui::Button("Cerrar"))
	{
		show_menu_ajustes = false;
		show_menu_inicio = true;
	}
	ImGui::End();
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

void cambiarEstiloBotones()
{
	// a) Colores
	// Botón Normal: Gris Acero / Plateado Oscuro
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.40f, 0.42f, 0.45f, 1.0f));

	// Botón Hover (Ratón encima): Plateado más brillante
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.55f, 0.57f, 0.60f, 1.0f));

	// Botón Activo (Click): Gris oscuro (presión mecánica)
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.27f, 0.30f, 1.0f));

	// Borde: Color COBRE (Naranja/Marrón metálico)
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.85f, 0.55f, 0.25f, 1.0f));

	// b) Variables
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f); // Grosor del borde
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);   // Un ligero redondeo queda bien con metal
}

void regresarEstiloBotones()
{
	ImGui::PopStyleColor(4); // Sacamos los 4 colores
	ImGui::PopStyleVar(2);   // Sacamos las 2 variables (Size y Rounding)
}