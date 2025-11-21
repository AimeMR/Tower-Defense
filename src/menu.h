#pragma once

//ImGui: Gestió de finestres de menú ImGui

extern bool show_menu_inicio;
extern bool show_menu_ajustes;
extern bool show_jugar;
extern bool show_menu_creditos;
extern bool juego_pausado;
extern bool show_menu_pruebas;

// --- VARIABLES DE CONTROL DE PRUEBAS ---
extern bool debug_detener_tiempo; // Controla si el timer corre
extern bool debug_resetear_todo;  // Orden de matar y reiniciar
extern int debug_id_enemigo_spawn; // Id del enemgo
extern bool debug_solicitar_spawn; // La orden de spawnear


void menu(bool& salir);
void iniciarPartida(bool& salir);
void menuPausa(bool& salir, const ImGuiViewport* viewport);
void menuAjustes();
void menuCreditos();
void menuPruebas(bool& salir);

void aplicarEfectoBrillo();
void cambiarEstiloBotones();
void regresarEstiloBotones();
ImVec2 colocarBoton(float porX, float porY);
void cambiarEstiloSlider();
void regresarEstiloSlider();



