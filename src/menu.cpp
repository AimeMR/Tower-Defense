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
bool show_menu_settings = false;
bool show_jugar = false;
bool show_menu_creditos = false;

//--------------------- A PARTIR D'AQUÍ EL NOSTRE CODI
void menu() {

	static bool show_app_debug_log = true;
	// Inizializa frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//if(show_another_window)

	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

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
			ImGui::Checkbox("Use work area instead of main area", &use_work_area);
			ImGui::SameLine();
			//HelpMarker("Main Area = entire viewport,\nWork Area = entire viewport minus sections used by the main menu bars, task bars etc.\n\nEnable the main-menu bar in Examples menu to see the difference.");

			ImGui::CheckboxFlags("ImGuiWindowFlags_NoBackground", &flags, ImGuiWindowFlags_NoBackground);
			ImGui::CheckboxFlags("ImGuiWindowFlags_NoDecoration", &flags, ImGuiWindowFlags_NoDecoration);
			ImGui::Indent();
			ImGui::CheckboxFlags("ImGuiWindowFlags_NoTitleBar", &flags, ImGuiWindowFlags_NoTitleBar);
			ImGui::CheckboxFlags("ImGuiWindowFlags_NoCollapse", &flags, ImGuiWindowFlags_NoCollapse);
			ImGui::CheckboxFlags("ImGuiWindowFlags_NoScrollbar", &flags, ImGuiWindowFlags_NoScrollbar);
			ImGui::Unindent();

			if (show_app_debug_log)
				ImGui::ShowDebugLogWindow(&show_app_debug_log);

			//Inicia el juego
			if (ImGui::Button("Jugar"))
			{
				show_menu_inicio = false;
				show_jugar = true;
			}
			//Activa el menu de settings
			if (ImGui::Button("Settings"))
			{
				show_menu_inicio = false;
				show_menu_settings = true;
			}
			//Activa el menu de creditos
			if (ImGui::Button("Creditos"))
			{
				show_menu_inicio = false;
				show_menu_creditos = true;
			}
		}
		ImGui::End();
	}
	//Iniciar juego
	else if (show_jugar)
	{
		iniciarPartida();
	}
	//Menu settings
	else if (show_menu_settings)
	{
		menuSettings();
	}
	//Menu settings
	else if (show_menu_creditos)
	{
		menuCreditos();
	}

	ImGui::Render();
}

void iniciarPartida()
{
	
}

void menuSettings()
{
	show_menu_inicio = false;
	ImGui::Begin("Menu", &show_menu_settings);
	if (ImGui::Button("Cerrar"))
	{
		show_menu_settings = false;
		show_menu_inicio = true;
	}
	ImGui::End();

}
void menuCreditos()
{
	show_menu_inicio = false;
	ImGui::Begin("Menu", &show_menu_settings);
	if (ImGui::Button("Cerrar"))
	{
		show_menu_settings = false;
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