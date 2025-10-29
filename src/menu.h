#pragma once

//ImGui: Gestió de finestres de menú ImGui

extern bool show_menu_inicio;
extern bool show_menu_settings;
extern bool show_jugar;
extern bool show_menu_creditos;

void menu();
void iniciarPartida();
void menuSettings();
void menuCreditos();