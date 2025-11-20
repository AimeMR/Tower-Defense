#pragma once

//ImGui: Gestió de finestres de menú ImGui

extern bool show_menu_inicio;
extern bool show_menu_ajustes;
extern bool show_jugar;
extern bool show_menu_creditos;
extern bool juego_pausado;

void menu(bool& salir);
void iniciarPartida(bool& salir);
void menuAjustes();
void menuCreditos();

void cambiarEstiloBotones();
void regresarEstiloBotones();
ImVec2 colocarBoton(float porX, float porY);


