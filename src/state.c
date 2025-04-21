#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

// Definiciones de funciones de ayuda.
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);




/* Tarea 1 */
game_state_t* create_default_state()
{
  // TODO: Implementar esta funcion.

  //PASO 1: reservar espacio para el estado, llámese "predeterminado" y asegurarse que se reserve correctamente
  //Detalle: guardar espacio suficient, no solo para tablero, sino para todos los campos de "game_state_t".
  game_state_t* predeterminado = (game_state_t*) malloc(sizeof(game_state_t));
  if (!predeterminado)
  {
    return NULL;
  }

  //PASO 2: inicializar valores sencillos
  predeterminado->num_rows = 18;
  predeterminado->num_snakes = 1;
  


  //PASO 3: definir el tablero predeterminado
  //Por cada fila, se reserva espacio en memoria y se asignan los caracteres con un "strcpy"

  predeterminado->board = (char**) malloc(18 * sizeof(char *));
    if (!predeterminado->board)
    {
      free(predeterminado);
      return NULL;
    }

  char* pred_tablero[18] =
  {
    "####################",
    "#                  #",
    "# d>D    *         #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "####################"
  };

  for (int i = 0; i < 18; i ++)
  {
    predeterminado->board[i] = (char*) malloc(21 * sizeof(char));
    if (!predeterminado->board[i])
    {
      for (int j = 0; j < i; j++)
      free(predeterminado->board[j]);
      free(predeterminado->board);
      free(predeterminado);
      return NULL;
    }


      strcpy(predeterminado->board[i], pred_tablero[i]);
  }

  //PASO 4: definir la serpiente
  predeterminado->snakes = (snake_t* ) malloc(sizeof(snake_t));
  if (!predeterminado->snakes)
  {
    for (int i = 0; i < 18; i++)
      {
        free(predeterminado->board[i]);
      }
    
      free(predeterminado->board);
      free(predeterminado);
      return NULL;
  }

  predeterminado->snakes->head_col = 4;
  predeterminado->snakes->head_row = 2;

  predeterminado->snakes->tail_col = 2;
  predeterminado->snakes->tail_row = 2;

  predeterminado->snakes->live = true;

  return predeterminado;
}


/* Tarea 2 */
void free_state(game_state_t* state) 
{
  if(state == NULL) return;
  for(int i = 0; i < state->num_rows; i++){
  	free(state->board[i]);
  }
  free(state->board);
  free(state->snakes);
  free(state);
}


/* Tarea 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implementar esta funcion.
  for(int i = 0; i< state->num_rows; i++){
  	fprintf(fp, "%s\n", state->board[i]);
  }
  
  return;
}


/**
 * Guarda el estado actual a un archivo. No modifica el objeto/struct state.
 * (ya implementada para que la utilicen)
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Tarea 4.1 */


/**
 * Funcion de ayuda que obtiene un caracter del tablero dado una fila y columna
 * (ya implementado para ustedes).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}


/**
 * Funcion de ayuda que actualiza un caracter del tablero dado una fila, columna y
 * un caracter.
 * (ya implementado para ustedes).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}


/**
 * Retorna true si la variable c es parte de la cola de una snake.
 * La cola de una snake consiste de los caracteres: "wasd"
 * Retorna false de lo contrario.
*/
static bool is_tail(char c) {
  // TODO: Implementar esta funcion.
  if(c == 'w' || c == 'a' || c == 's' || c == 'd'){
  	return true;
  }
  return false;
}


/**
 * Retorna true si la variable c es parte de la cabeza de una snake.
 * La cabeza de una snake consiste de los caracteres: "WASDx"
 * Retorna false de lo contrario.
*/
static bool is_head(char c) {
  // TODO: Implementar esta funcion.
  if(c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x'){
  	return true;
  }
  return false;
}


/**
 * Retorna true si la variable c es parte de una snake.
 * Una snake consiste de los siguientes caracteres: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implementar esta funcion.
  if(c == 'w' || c == 'a' || c == 's' || c == 'd' || c == '^' || c == '<' || c == 'v' || c == '>' || c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x'){
  	return true;
  }
  return false;
}


/**
 * Convierte un caracter del cuerpo de una snake ("^<v>")
 * al caracter que correspondiente de la cola de una
 * snake ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implementar esta funcion.
  if(c == '^'){
  	return 'w';
  }else if(c == '<'){
  	return 'a';
  }else if(c == 'v'){
  	return 's';
  }else if(c == '>'){
  	return 'd';
  }
  return '?';
}


/**
 * Convierte un caracter de la cabeza de una snake ("WASD")
 * al caracter correspondiente del cuerpo de una snake
 * ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implementar esta funcion.
  if(c == 'W'){
  	return '^';
  }else if(c == 'A'){
  	return '<';
  }else if(c == 'S'){
  	return 'v';
  }else if(c == 'D'){
  	return '>';
  }
  return '?';
}


/**
 * Retorna cur_row + 1 si la variable c es 'v', 's' o 'S'.
 * Retorna cur_row - 1 si la variable c es '^', 'w' o 'W'.
 * Retorna cur_row de lo contrario
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implementar esta funcion.
  if(c == 'v' || c == 's' || c == 'S' ){
  	return cur_row + 1;
  }else if(c == '^' || c == 'w' || c == 'W'){
  	return cur_row - 1;
  }  
  return cur_row;
}


/**
 * Retorna cur_col + 1 si la variable c es '>' or 'd' or 'D'.
 * Retorna cur_col - 1 si la variable c es '<' or 'a' or 'A'.
 * Retorna cur_col de lo contrario
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implementar esta funcion.
  if(c == '>' || c == 'd' || c == 'D' ){
  	return cur_col + 1;
  }else if(c == '<' || c == 'a' || c == 'A'){
  	return cur_col - 1;
  }  
  return cur_col;
}


/**
 * Tarea 4.2
 *
 * Funcion de ayuda para update_state. Retorna el caracter de la celda
 * en donde la snake se va a mover (en el siguiente paso).
 *
 * Esta funcion no deberia modificar nada de state.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  // TODO: Implementar esta funcion.
  char cabeza = state->board[state->snakes[snum].head_row][state->snakes[snum].head_col];
  unsigned int fila = 0, columna = 0;
  if(cabeza == 'W' || cabeza == 'S' || cabeza == 'A' || cabeza == 'D' || cabeza == 'x'){
  	if(cabeza == 'W' || cabeza == 'S'){
	  	fila = get_next_row(state->snakes[snum].head_row, cabeza);
	  	columna = state->snakes[snum].head_col;
	}else if(cabeza == 'A' || cabeza == 'D'){
	  	fila = state->snakes[snum].head_row;
	  	columna = get_next_col(state->snakes[snum].head_col, cabeza);
	}
	return state->board[fila][columna]; 
  }
  return '?';
}


/**
 * Tarea 4.3
 *
 * Funcion de ayuda para update_state. Actualiza la cabeza de la snake...
 *
 * ... en el tablero: agregar un caracter donde la snake se va a mover (¿que caracter?)
 *
 * ... en la estructura del snake: actualizar el row y col de la cabeza
 *
 * Nota: esta funcion ignora la comida, paredes, y cuerpos de otras snakes
 * cuando se mueve la cabeza.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  // TODO: Implementar esta funcion.
  char cabeza = state->board[state->snakes[snum].head_row][state->snakes[snum].head_col];
  unsigned int fila = 0, columna = 0;
  if(cabeza == 'W' || cabeza == 'S' || cabeza == 'A' || cabeza == 'D'){
  	if(cabeza == 'W' || cabeza == 'S'){
	  	fila = get_next_row(state->snakes[snum].head_row, cabeza);
	  	columna = state->snakes[snum].head_col;
	}else if(cabeza == 'A' || cabeza == 'D'){
	  	fila = state->snakes[snum].head_row;
	  	columna = get_next_col(state->snakes[snum].head_col, cabeza);
	}
	set_board_at(state, fila, columna, cabeza);
	set_board_at(state, state->snakes[snum].head_row, state->snakes[snum].head_col, head_to_body(cabeza));
	
	state->snakes[snum].head_row = fila;
	state->snakes[snum].head_col = columna;
  }
  return;
}


/**
 * Tarea 4.4
 *
 * Funcion de ayuda para update_state. Actualiza la cola de la snake...
 *
 * ... en el tablero: colocar un caracter blanco (spacio) donde se encuentra
 * la cola actualmente, y cambiar la nueva cola de un caracter de cuerpo (^<v>)
 * a un caracter de cola (wasd)
 *
 * ...en la estructura snake: actualizar el row y col de la cola
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  // TODO: Implementar esta funcion.
  return;
}

/* Tarea 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implementar esta funcion.
  return;
}

/* Tarea 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implementar esta funcion.
  return NULL;
}


/**
 * Tarea 6.1
 *
 * Funcion de ayuda para initialize_snakes.
 * Dada una structura de snake con los datos de cola row y col ya colocados,
 * atravezar el tablero para encontrar el row y col de la cabeza de la snake,
 * y colocar esta informacion en la estructura de la snake correspondiente
 * dada por la variable (snum)
*/
static void find_head(game_state_t* state, unsigned int snum) {
  // TODO: Implementar esta funcion.
  return;
}

/* Tarea 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implementar esta funcion.
  return NULL;
}
