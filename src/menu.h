#pragma once

//ImGui: Gestió de finestres de menú ImGui

extern bool show_menu_inicio;
extern bool show_menu_ajustes;
extern bool show_jugar;
extern bool show_menu_creditos;

void menu(bool& salir);
void iniciarPartida();
void menuAjustes();
void menuCreditos();
ImVec2 centrarBotonMenu(float porX, float porY);