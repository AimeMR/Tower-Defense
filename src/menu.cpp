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

bool show_menu_inicio = true;
bool show_menu_ajustes = false;
bool show_jugar = false;
bool show_menu_creditos = false;
bool juego_pausado = false;


//--------------------- A PARTIR D'AQUÍ EL NOSTRE CODI
void menu(bool& salir) 
{

	// Inizializa frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	if (show_menu_inicio)
	{
		static bool show_app_debug_log = true;
		

		//if(show_another_window)

		static bool use_work_area = true;
	
		//static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
		static ImGuiWindowFlags flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
	
		// We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
		// Based on your use case you may want one of the other.
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
		ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

		//Menu de incio
	
		if (ImGui::Begin("Example: Fullscreen window", &show_menu_inicio, flags))
		{
			/*
			ImGui::Checkbox("Use work area instead of main area", &use_work_area);
			ImGui::SameLine();
			//HelpMarker("Main Area = entire viewport,\nWork Area = entire viewport minus sections used by the main menu bars, task bars etc.\n\nEnable the main-menu bar in Examples menu to see the difference.");

			ImGui::CheckboxFlags("ImGuiWindowFlags_NoBackground", &flags, ImGuiWindowFlags_NoBackground);
			ImGui::CheckboxFlags("ImGuiWindowFlags_NoDecoration", &flags, ImGuiWindowFlags_NoDecoration);
			ImGui::Indent();
			ImGui::CheckboxFlags("ImGuiWindowFlags_NoTitleBar", &flags, ImGuiWindowFlags_NoTitleBar);
			ImGui::CheckboxFlags("ImGuiWindowFlags_NoCollapse", &flags, ImGuiWindowFlags_NoCollapse);
			ImGui::CheckboxFlags("ImGuiWindowFlags_NoScrollbar", &flags, ImGuiWindowFlags_NoScrollbar);
			ImGui::Unindent();*/

			//if (show_app_debug_log)
				//ImGui::ShowDebugLogWindow(&show_app_debug_log);

			cambiarEstiloBotones();
			ImGui::SetWindowFontScale(1.5f);

			// --- BOTÓN JUGAR ---
			// Lo ponemos un poco más arriba del centro (ej. 40% de altura)
			ImVec2 btnSize = colocarBoton(0.5f, 0.40f);
			if (ImGui::Button("Jugar", btnSize))
			{
				show_menu_inicio = false;
				show_jugar = true;
			}

			// --- BOTÓN AJUSTES ---
			// Lo ponemos un poco más abajo (ej. 55% de altura)
			// Nota: Como usamos SetCursorPos, NO importa que haya un botón antes.
			btnSize = colocarBoton(0.5f, 0.55f);
			if (ImGui::Button("Ajustes", btnSize))
			{
				show_menu_inicio = false;
				show_menu_ajustes = true;
			}

			// --- BOTÓN CREDITOS ---
			// Más abajo (ej. 70% de altura)
			btnSize = colocarBoton(0.5f, 0.70f);
			if (ImGui::Button("Creditos", btnSize))
			{
				show_menu_inicio = false;
				show_menu_creditos = true;
			}

			// --- BOTÓN SALIR ---
			// Esquina inferior izquierda (ej. 10% X, 90% Y)
			// Ojo: Tu código original tenía centrarBotonMenu(0.05, 0.95), ajusta según gusto
			btnSize = colocarBoton(0.1f, 0.90f);
			if (ImGui::Button("Salir", btnSize))
			{
				show_menu_inicio = false;
				salir = true;
			}

			ImGui::SetWindowFontScale(1.0f);
			regresarEstiloBotones();
		}
		ImGui::End();
	}
	//Iniciar juego
	else if (show_jugar)
	{
		iniciarPartida(salir);
	}
	//Menu ajustes
	else if (show_menu_ajustes)
	{
		menuAjustes();
	}
	//Menu settings
	else if (show_menu_creditos)
	{
		menuCreditos();
	}

	ImGui::Render();
}

void cambiarEstiloBotones()
{
	// a) Colores (para opacidad, color de fondo, y borde)
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.3f, 0.6f, 1.0f)); // Fondo opaco (Alpha=1.0f)
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.4f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.2f, 0.5f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // Borde amarillo

	// b) Variables (para el grosor del borde)
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f); // Grosor del borde de 2px
}

void regresarEstiloBotones()
{
	// Sacamos 4 colores (Button, ButtonHovered, ButtonActive, Border)
	ImGui::PopStyleColor(4);

	// Sacamos 1 variable (FrameBorderSize)
	ImGui::PopStyleVar(1);

}

ImVec2 colocarBoton(float porX, float porY)
{
	// 1. Definir tamaño del botón
	ImVec2 button_size(200, 50);

	// 2. Obtener tamaño de la ventana actual (donde dibujas el menú)
	ImVec2 window_size = ImGui::GetWindowSize();

	// 3. Calcular posición ABSOLUTA
	// Formula: (AnchoVentana * Porcentaje) - (MitadAnchoBoton)
	float posX = (window_size.x * porX) - (button_size.x * 0.5f);
	float posY = (window_size.y * porY) - (button_size.y * 0.5f);

	// 4. FORZAR la posición del cursor (sin sumar a la anterior)
	ImGui::SetCursorPos(ImVec2(posX, posY));

	return button_size;
}

void iniciarPartida(bool& salir)
{
	// 1. DIBUJAR EL HUD (BOTÓN PAUSA)
	// Usamos una ventana sin decoración, fondo transparente opcional, y que se auto-ajuste al tamaño
	ImGuiWindowFlags hudFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	// Calculamos la posición: Esquina superior derecha
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	float padding = 10.0f;

	// Posicionamos "aproximadamente" a la derecha. Luego ImGui ajustará el ancho.
	// WorkPos.x + WorkSize.x es el borde derecho. Le restamos un tamaño estimado (ej. 50px) y el padding.
	ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x - 60.0f - padding, viewport->WorkPos.y + padding), ImGuiCond_Always);

	// Fondo semi-transparente para el botoncito
	ImGui::SetNextWindowBgAlpha(0.35f);

	if (ImGui::Begin("HudPausa", NULL, hudFlags))
	{
		// Si pulsamos el botón, activamos la pausa
		if (ImGui::Button("||", ImVec2(50, 50))) // Botón cuadrado de 50x50
		{
			juego_pausado = !juego_pausado;
		}
	}
	ImGui::End();

	// 2. DIBUJAR EL MENÚ DE PAUSA (Si está pausado)
	if (juego_pausado)
	{
		// Este menú SI queremos que bloquee el juego y se centre
		ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::Begin("Menu Pausa", &juego_pausado, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::Text("JUEGO PAUSADO");
			ImGui::Separator();
			ImGui::Spacing();

			// Botón Continuar
			if (ImGui::Button("Continuar", ImVec2(120, 0)))
			{
				juego_pausado = false;
			}

			// Botón Salir al Menú Principal
			if (ImGui::Button("Salir al Menu", ImVec2(120, 0)))
			{
				juego_pausado = false;
				show_jugar = false;
				show_menu_inicio = true;
				// Aquí deberías resetear el juego si es necesario
			}

			// Botón Salir del Programa
			if (ImGui::Button("Salir del Juego", ImVec2(120, 0)))
			{
				salir = true;
			}
		}
		ImGui::End();
	}

}

void menuAjustes()
{
	show_menu_inicio = false;
	ImGui::Begin("Menu", &show_menu_ajustes);
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
	ImGui::Begin("Menu", &show_menu_creditos);
	if (ImGui::Button("Cerrar"))
	{
		show_menu_creditos = false;
		show_menu_inicio = true;
	}
	ImGui::End();
}

// 3. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
/*{
	//ventana incio 
	
	static float f = 0.0f;
	static int counter = 0;
	static float PV[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	ImGui::Begin("Menu inicio");                          // Create a window called "Status Menu" and append into it.

	ImGui::Text("Finestres EntornVGI:");               // Display some text (you can use a format strings too)
	ImGui::SameLine();
	ImGui::Checkbox("EntornVGI Window", &show_EntornVGI_window);
	ImGui::Separator();
	ImGui::Spacing();




	ImGui::End();


}*/
//ImGui::OpenPopup();

