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

#define LINE_COUNTER 1000

/* Tarea 1 */
game_state_t* create_default_state()
{
  // TODO: Implementar esta funcion.

  // PASO 1: reservar espacio para el estado, llámese "default_state" y asegurarse que se reserve correctamente
  // Detalle: se debe guardar espacio suficient, no solo para tablero, sino para todos los campos de "game_state_t".
  game_state_t* default_state = (game_state_t*) malloc(sizeof(game_state_t));
  if (!default_state)
  {
    return NULL;
  }

  // PASO 2: inicializar valores sencillos del mapa de juego
  default_state->num_rows = 18;
  default_state->num_snakes = 1;
  


  // PASO 3: definir el tablero default_state
  // Por cada fila, se reserva espacio en memoria y se asignan los caracteres con un "strcpy"

  default_state->board = (char**) malloc(18 * sizeof(char *));
  if (!default_state->board)
  {
    free(default_state);
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
    default_state->board[i] = (char*) malloc(21 * sizeof(char));
    
    if (!default_state->board[i])
    {
      for (int j = 0; j < i; j++)
      {
        free(default_state->board[j]);
      }
      
      free(default_state->board);
      free(default_state);
      return NULL;
    }


      strcpy(default_state->board[i], pred_tablero[i]);
  }


  // PASO 4: definir la serpiente o serpientes, con variable "snake"
  default_state->snakes = (snake_t* ) malloc(sizeof(snake_t));
  if (!default_state->snakes)
  {
    for (int i = 0; i < 18; i++)
      {
        free(default_state->board[i]);
      }
    
      free(default_state->board);
      free(default_state);
      return NULL;
  }

  // PASO 5: inicializar los valores que dan vida y ubicacion a la serpiente
  default_state->snakes->head_col = 4;
  default_state->snakes->head_row = 2;

  default_state->snakes->tail_col = 2;
  default_state->snakes->tail_row = 2;

  default_state->snakes->live = true;

  return default_state;
}


/* Tarea 2 */
void free_state(game_state_t* state) 
{
  // VERIFICACIÓN 1: si no existe "state", no hay nada que borrar o liberar
  if(state == NULL) 
  {
    return;
  }

  // IDEA: como si fuera limpieza de carro (al menos así lo hago yo): se libera desde adentro, hacia afuera. Desde lo más profundo, hasta lo menos profundo

  // PASO 1: liberar cada fila de memoria reservada para el tablero
  for(int i = 0; i < state->num_rows; i++)
  {
  	free(state->board[i]);
  }

  // PASO 2: liberar la memoria del puntero del tablero
  free(state->board);

  // PASO 3: liberar los datos (y todos sus parámetros) de la estructura "snakes"
  free(state->snakes);

  // PASO 4: liberar "state" como tal
  free(state);
}


/* Tarea 3 */
void print_board(game_state_t* state, FILE* fp)
{
  // IDEA: recorrer cada fila del tablero e imprimirla

  // PASO 1 (y unico): recorrer las filas del tablero e imprimir cada una
  for(int i = 0; i< state->num_rows; i++)
  {
  	fprintf(fp, "%s\n", state->board[i]);
  }
  
  return;
}


/**
 * Guarda el estado actual a un archivo. No modifica el objeto/struct state.
 * (ya implementada para que la utilicen)
*/
void save_board(game_state_t* state, char* filename)
{
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}


/* Tarea 4.1 */
/**
 * Funcion de ayuda que obtiene un caracter del tablero dado una fila y columna
 * (ya implementado para ustedes).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) 
{
  // Como si fueran coordenadas (x, y) solo se devuelve la coordenada solicitada
  return state->board[(unsigned int) row][(unsigned int) col];
}


/**
 * Funcion de ayuda que actualiza un caracter del tablero dado una fila, columna y
 * un caracter.
 * (ya implementado para ustedes).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch)
{
  // Como si fueran coordenadas (x, y) solo se asigna el caracter recibido a la coordenada solicitada
  state->board[row][col] = ch;
}


/**
 * Retorna true si la variable c es parte de la cola de una snake.
 * La cola de una snake consiste de los caracteres: "wasd"
 * Retorna false de lo contrario.
*/
static bool is_tail(char c) 
{
  // VERIFICACIÓN: si el caracter recibido es una cola conjunto cola = {a, w, d, s}
  if(c == 'a' || c == 'w' || c == 'd' || c == 's')
  {
  	return true;                                        // Es una cola
  }
  return false;                                         // Sino, no es una cola
}


/**
 * Retorna true si la variable c es parte de la cabeza de una snake.
 * La cabeza de una snake consiste de los caracteres: "WASDx"
 * Retorna false de lo contrario.
*/
static bool is_head(char c)
{
  // VERIFICACION: misma idea que con "is_tail", solo que con las letras mayusculas; conjunto cabezas = {A, W, D, S}
  if(c == 'A' || c == 'W' || c == 'D' || c == 'S' || c == 'x')    // NOTA: también cuentan las cabezas de serpiente muerta (por eso se incluye la "x")
  {
  	return true;                                                  // Es una cabeza
  }
  return false;                                                   // Sino , no es una cabeza
}


/**
 * Retorna true si la variable c es parte de una snake.
 * Una snake consiste de los siguientes caracteres: "wasd^<v>WASDx"
*/
static bool is_snake(char c)
{
  // VERIFICACION: si el caracter "is_head" o "is_tail" o es bla bla bla...
  if ((c == '^' || c == '<' || c == 'v' || c == '>') || (is_head(c) || is_tail(c)))
  {
  	return true;                                                                      // Estamos ante una serpiente
  }
  return false;                                                                       // Sino, no es serpiente
}


/**
 * Convierte un caracter del cuerpo de una snake ("^<v>")
 * al caracter que correspondiente de la cola de una
 * snake ("wasd").
*/
static char body_to_tail(char c)
{
  // Retornar cola asociada a la dirección de la flechita (<, ^, >, v)
  switch (c) 
  {
    case '^': return 'w';

    case '<': return 'a';

    case 'v': return 's';

    case '>': return 'd';

    default: return '?';
  }
}


/**
 * Convierte un caracter de la cabeza de una snake ("WASD")
 * al caracter correspondiente del cuerpo de una snake
 * ("^<v>").
*/
static char head_to_body(char c)
{
  // Misma idea que "body_to_tail"
  switch (c) {
    case 'W':
      return '^';
    case 'A':
      return '<';
    case 'S':
      return 'v';
    case 'D':
      return '>';
    default:
      return '?';
  }
  
}


/**
 * Retorna cur_row + 1 si la variable c es 'v', 's' o 'S'.
 * Retorna cur_row - 1 si la variable c es '^', 'w' o 'W'.
 * Retorna cur_row de lo contrario
*/
static unsigned int get_next_row(unsigned int cur_row, char c)
{
  // TODO: Implementar esta funcion.
  if(c == 'v' || c == 's' || c == 'S' )
  {
  	return (unsigned int) cur_row + 1;
  }
  
  if(c == '^' || c == 'w' || c == 'W')
  {
  	return (unsigned int) cur_row - 1;
  }

  return cur_row;
}


/**
 * Retorna cur_col + 1 si la variable c es '>' or 'd' or 'D'.
 * Retorna cur_col - 1 si la variable c es '<' or 'a' or 'A'.
 * Retorna cur_col de lo contrario
*/
static unsigned int get_next_col(unsigned int cur_col, char c)
{
  // TODO: Implementar esta funcion.
  if(c == '>' || c == 'd' || c == 'D' )
  {
  	return cur_col + 1;
  }

  else if(c == '<' || c == 'a' || c == 'A')
  {
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
static char next_square(game_state_t* state, unsigned int snum)
{
  // TODO: Implementar esta funcion.

  // PASO 1: obtenemos la cabeza, mediante su coordenada (head_row, head_col)
  char cabeza = state -> board[state -> snakes[snum].head_row][state->snakes[snum].head_col];
  unsigned int fila = 0, columna = 0;

  // VERIFICACIÓN: si "cabeza" es una cabeza viva o muerta (suena redundante, pero sí)
  if(is_head(cabeza))
  {
    // VERIFICACION: si la cabeza es de movimiento vertical
  	if(cabeza == 'W' || cabeza == 'S')
    {
      // Obtenemos valores de fila (fila de al lado) y columna (misma columna)
	  	fila = get_next_row(state -> snakes[snum].head_row, cabeza);
	  	columna = state -> snakes[snum].head_col;
	  }
  
    // VERIFICACION: si la cabeza es de movimiento horizonal
    else if(cabeza == 'A' || cabeza == 'D')
    {
      // Obtenemos valores de fila (misma fila) y columna (columna de al lado)
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
static void update_head(game_state_t* state, unsigned int snum)
{
  // Misma idea para obtener cabeza
  char cabeza = state->board[state->snakes[snum].head_row][state->snakes[snum].head_col];
  unsigned int fila = 0, columna = 0;

  //NOTA: no podría usar la función "is_head()", porque solo aplica para cabezas vivas (is_head también toma en cuenta cabezas muertas)
  if(cabeza == 'W' || cabeza == 'S' || cabeza == 'A' || cabeza == 'D') 
  {
    // VERIFICACION: si la cabeza es de movimiento vertical
  	if(cabeza == 'W' || cabeza == 'S')
    {
	  	fila = get_next_row(state->snakes[snum].head_row, cabeza);
	  	columna = state->snakes[snum].head_col;
	  }
    
    // VERIFICACION: si la cabeza es de movimiento horizontal
    else if(cabeza == 'A' || cabeza == 'D')
    {
	  	fila = state->snakes[snum].head_row;
	  	columna = get_next_col(state->snakes[snum].head_col, cabeza);
	  }

    // Seteamos el tableto, con los nuevos valroes de fila, columna y cabeza del tablero anterior
    set_board_at(state, fila, columna, cabeza);
    set_board_at(state, state->snakes[snum].head_row, state->snakes[snum].head_col, head_to_body(cabeza));
	
    // Seteamos los nuevos valores de la serpeinte "snum"
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
static void update_tail(game_state_t* state, unsigned int snum)
{
  // PASO 1: obtener las coordenadas (fila, columna) de la cola de la serpiente "snum"
  unsigned int fila = state->snakes[snum].tail_row;
  unsigned int columna = state->snakes[snum].tail_col;

  // VERIFICACION: si se trata de la cola "w"
  if(get_board_at(state, fila, columna) == 'w')
  {
  	set_board_at(state, fila-1, columna, body_to_tail(get_board_at(state, fila-1, columna)));
  	set_board_at(state, fila, columna, ' ');
  	state->snakes[snum].tail_row = fila-1;
  }
  
  // VERIFICACION: si se trata de la cola "a"
  else if(get_board_at(state, fila, columna) == 'a')
  {
  	set_board_at(state, fila, columna-1, body_to_tail(get_board_at(state, fila, columna-1)));
  	set_board_at(state, fila, columna, ' ');
  	state->snakes[snum].tail_col = columna-1;
  }

  // VERIFICACION: si se trata de la cola "s"
  else if(get_board_at(state, fila, columna) == 's')
  {
  	set_board_at(state, fila+1, columna, body_to_tail(get_board_at(state, fila+1, columna)));
  	set_board_at(state, fila, columna, ' ');
  	state->snakes[snum].tail_row = fila+1;
  }
  
  // VERIFICACION: si se trata de la cola "d"
  else if(get_board_at(state, fila, columna) == 'd')
  {
  	set_board_at(state, fila, columna+1, body_to_tail(get_board_at(state, fila, columna+1)));
  	set_board_at(state, fila, columna, ' ');
  	state->snakes[snum].tail_col = columna+1;
  }

  return;
}


/* Tarea 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state))
{
  // TODO: Implementar esta funcion.
  for(int i = 0; i < state->num_snakes; i++)
  {
  	if(next_square(state, i) == 'W' || next_square(state, i) == 'A' || next_square(state, i) == 'S' || next_square(state, i) == 'D' || next_square(state, i) == 'w' || next_square(state, i) == 'a' || next_square(state, i) == 's' || next_square(state, i) == 'd' || next_square(state, i) == '^' || next_square(state, i) == '<' || next_square(state, i) == 'v' || next_square(state, i) == '>' || next_square(state, i) == '#')
    {
  		set_board_at(state, state->snakes[i].head_row, state->snakes[i].head_col, 'x');
		state->snakes[i].live = false;	
	  }
    
    else if(next_square(state, i) == '*')
    {
	  	update_head(state, i);
	  	add_food(state);
	  }
    
    else if(next_square(state, i) == ' ')
    {
	  	update_head(state, i);
	  	update_tail(state, i);
	  }
  }

  return;
}

/* Tarea 5 */
game_state_t* load_board(char* filename) 
{

  // PASO 1: abrir el archivo con nombre recibido en "filename"
  FILE* file = fopen(filename, "r");

  // PASO 1.1.: verificación casual. Si el archivo no existe o es nulo, retornamos nada.
  if (file == NULL)
  {
    return NULL;
  }

  // NOTA: llegados a este punto, nunca se hizo una reserva de espacio, por lo que en la verificacion anterior
  //       no es necesario realizar alguna liberacion de memoria. Todo bien :D


  // PASO 2: obtener la cantidad de lineas que posee el archivo leido en "file"

  char buff [LINE_COUNTER];
  int lines_amount = 0;

  while(fgets(buff, LINE_COUNTER, file))
  {
    lines_amount++;
  }

  // PASO 3: reservar espacio para el estado de juego "current_state"
  game_state_t* current_state = (game_state_t*) malloc(sizeof(game_state_t));

  // PASO 3.1.: definir todos los parametros que pertenecen al estado "current_state", como: serpientes, filas, tablero, etc
  current_state -> board = malloc(sizeof(char*) * lines_amount);
  current_state -> num_rows = lines_amount;
  current_state -> num_snakes = 0;
  current_state -> snakes = NULL;

  // NOTA: hasta ahora, solo hemos definido e inicializado algunos valores, pero no hemos leido nada del archivo...
  //       notar que, con el ciclo "while" anterior, recorrimos linea por linea, del archivo. Por lo tanto, debemos
  //       regresar al inicio del mismo, con la funcion "rewind".

  rewind(file);

  // PASO 4. leer cada linea del archivo y armar el tablero
  int i = 0;
  
  while (fgets(buff, LINE_COUNTER, file))   // Mientras aun haya lineas por leer
  {
    size_t len = strlen(buff);              // Se define la longitud de la linea leida

    // Si el ultimo caracter de la linea leida es el de salto de linea
    if (buff[len - 1] == '\n')             
    {
        buff[len - 1] = '\0';               // Se elimina y sustituye por el caracter nulo (para evitar saltos de linea)
        len--;
    }

    // Una vez asegurada la linea, reservamos bloque de memoria en esa parte del tablero
    current_state->board[i] = malloc((len + 1) * sizeof(char));

    // Copiamos la linea leida de "buff", en la fila "i" del tablero
    strcpy(current_state->board[i], buff);
    i++;
  }

  // PASO EXTRA (que observador soy - Tony): cerrar el archivo "file" :)
  fclose(file);

  // PASO 5: regresar el estado "current_state" con el tablero listo.
  return current_state;
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
static void find_head(game_state_t* state, unsigned int snum)
{
  // IDEA: ubicar con coordenadas (x,y) a la cola de la serpiente. Luego, recorrer todo su cuerpo hasta alcanzar una letra "A", "W", "S", "D"

  // PASO 1: ubicar coordenada (x, y) de la cola de la serpiente "snake"
  int current_snake_row = state -> snakes[snum].tail_row;
  int current_snake_col = state -> snakes[snum].tail_col;  

  // Si algo no existe (espero que no pase, porque muere todo el programa)
  if (state == NULL || state->snakes == NULL) 
  {
    return;   // Regresar
  }

  // PASO 2: recorrer el cuerpo de la serpiente, desde (x, y)
  // Siempre quise usar una de estas cosas "do-while"
  do
  {
    char current_direction = get_board_at(state, current_snake_row, current_snake_col);     // Guardamos el caracter (^, >, v, <) que se encuentra en la "current_row" y "current_col"
    current_snake_row = get_next_row(current_snake_row, current_direction);                 // Se mueve a la siguiente fila, en dependendica del caracter de "current_direction"
    current_snake_col = get_next_col(current_snake_col, current_direction);                 // Se mueve a la siguiente columna, en dependendica del caracter de "current_direction"

  } while (!is_head(get_board_at(state, current_snake_row, current_snake_col)));            // Esto sucedera hasta que (x, y) identifique a una cabeza
  

  state -> snakes[snum].head_row = current_snake_row;
  state -> snakes[snum].head_col = current_snake_col;

  return;
}

/* Tarea 6.2 */
game_state_t* initialize_snakes(game_state_t* state)
{

  // IDEA: recorrer el tablero salvajemente, en busca de colas (a, w, d, s)

  int alive_snakes_counter = 0;                                           // Contador de serpientes vivas 

  // PASO 1: recorrer tablero con foble "for" (ya que es una matriz bidemnsional)
    
  // Recorrer fila por fila del tablero
  for (int row = 0; row < state->num_rows; row++)
  {
    // Recorrer cola por cola de cada fila
    for (int col = 0; col < strlen(state->board[row]); col++)
    {
      //Si la coordenada (row, col) es una cola
      if (is_tail(state->board[row][col])) 
      {
        //Hay una serpiente viva
        alive_snakes_counter++;
      }
    }
  }

  // Honestamente, creí que esto era todo en la función "initialize_snakes" pero no funcionaba. Pregunté a Copilot y mencionó que aun faltaba cosas por hacer...

    // PASO 2: crear la cantidad de serpientes "alive_snakes_counter", al reservar espacio para ellas...
    state->num_snakes = alive_snakes_counter;

    state->snakes = malloc(alive_snakes_counter * sizeof(snake_t));

    // PASO 3: volver a recorrer para registrar cada serpiente
    int i = 0;
    
    for (int row = 0; row < state->num_rows; row++)
    {
        for (int col = 0; col < strlen(state->board[row]); col++)
        {
            if (is_tail(state->board[row][col]))
            {
                state->snakes[i].tail_row = row;
                state->snakes[i].tail_col = col;
                state->snakes[i].live = true;

                // Buscamos su cabeza
                find_head(state, i);
                i++;
            }
        }
    }

    return state;
}