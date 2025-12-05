#pragma once

//ImGui: Gestió de finestres de menú ImGui

extern bool show_menu_inicio;
extern bool show_menu_ajustes;
extern bool show_jugar;
extern bool show_menu_creditos;
extern bool juego_pausado;
extern bool show_menu_pruebas;
extern bool show_menu_construccion;

extern int idTipoTorreta;
extern int idTorretaSeleccionada;

// --- VARIABLES DE CONTROL DE PRUEBAS ---
extern bool debug_detener_tiempo; // Controla si el timer corre
extern bool debug_resetear_todo;  // Orden de matar y reiniciar
extern int debug_id_enemigo_spawn; // Id del enemigo
extern int debug_num_enemigo_spawn; // Cantidad de enemigos
extern bool debug_solicitar_spawn; // La orden de spawnear
extern bool show_submenu_shadows; // Booleano para ventana de sombras
extern bool show_submenu_light;
extern float debug_speedMult;

// --- VARIABLES SOMBRAS Y RENDER ---
extern float debug_lightDir[3];   // Vector direccion luz
extern float debug_boxSize;       // Tamano caja sombras
extern float debug_nearPlane;     // Plano cercano
extern float debug_farPlane;      // Plano lejano
extern float debug_ambientIntensity;
extern float debug_lightColor[3];
extern int debug_renderMode;      // Modo de renderizado 


void menu(bool& salir);
void iniciarPartida(bool& salir);
void menuPausa(bool& salir, const ImGuiViewport* viewport);
void menuAjustes();
void menuCreditos();
void menuConstruccion();

void menuPruebas(bool& salir);
void menuShadows();
void menuLight();

void aplicarEfectoBrillo();
void cambiarEstiloBotones();
void regresarEstiloBotones();
ImVec2 colocarBoton(float porX, float porY);
void cambiarEstiloSlider();
void regresarEstiloSlider();



