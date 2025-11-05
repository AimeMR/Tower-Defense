#include "ImGui\imgui.h"
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


//--------------------- A PARTIR D'AQUÍ EL NOSTRE CODI
void menu(bool& salir) 
{

	static bool show_app_debug_log = true;
	// Inizializa frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

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
	if (show_menu_inicio)
	{
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
			
			//Inicia el juego
			ImVec2 botonJugar = centrarBotonMenu(0.5, 0.40);
			if (ImGui::Button("Jugar", botonJugar))
			{
				show_menu_inicio = false;
				show_jugar = true;
			}

			//Activa el menu de ajustes
			ImVec2 botonAjuste = centrarBotonMenu(0.5, 0.05);
			if (ImGui::Button("Ajustes", botonAjuste))
			{
				show_menu_inicio = false;
				show_menu_ajustes = true;
			}
			
			//Activa el menu de creditos
			ImVec2 botonCreditos = centrarBotonMenu(0.5, 0.05);
			if (ImGui::Button("Creditos", botonCreditos))
			{
				show_menu_inicio = false;
				show_menu_creditos = true;
			}

			//Sale del juego
			ImVec2 botonSalir = centrarBotonMenu(0.05, 0.95);
			if (ImGui::Button("Salir", botonSalir))
			{
				show_menu_inicio = false;
				salir = true;
			}
			ImGui::SetWindowFontScale(1.0f);

			// --- 3. Restablecer estilos (IMPORTANTE) ---
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

ImVec2 centrarBotonMenu(float porX, float porY)
{
	// 1. Define el tamaño del botón
	ImVec2 button_size(200, 50);

	// 2. Obtener el ancho de la región de contenido disponible DENTRO de esta ventana
	float content_ancho = ImGui::GetContentRegionAvail().x;
	float content_altura = ImGui::GetContentRegionAvail().y;
	// 3. Calcular el desplazamiento (offset)
	float offsetX = (content_ancho - button_size.x) * porX;
	float offsetY = (content_altura - button_size.y) * porY;

	// 4. Mover el cursor por el offset
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);
	
	return button_size;
}

void iniciarPartida(bool& salir)
{
	
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