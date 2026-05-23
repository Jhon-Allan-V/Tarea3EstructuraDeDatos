#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>
#include <time.h>

#define FILA x
#define COL y
#define OBSTACULO 0
#define CAMINO 1

// Definición de la estructura para el estado del puzzle
typedef struct {
    int maze[N][N]; // Matriz NxN que representa el tablero
    int x;    // Posición x del agente
    int y;    // Posición x del agente
    int steps; // Pasos realizados hasta la posición actual
    List* actions; //Secuencia de movimientos para llegar al estado
} State;

typedef struct { int accionX, accionY; } accion;

int distancia_L1(State* state) {
    return abs(state->x - (N-1)) + abs(state->y - (N-1));
}

// Función para imprimir el estado actual
void imprimirEstado(const State *estado) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (estado->x==i && estado->y==j) printf(" A ");
            else if (i == 0 && j == 0) printf(" I ");
            else if (i == N-1 && j == N-1) printf(" M ");
            else if (estado->maze[i][j] == 0)
                printf(" . "); // Imprime un espacio en blanco para el espacio vacío
            else
                printf("[X]");
        }
        printf("\n");
    }
}

State crearEstadoInicial(int maze[N][N], int dificultad){
    State estado;
     // Copiar el laberinto generado al estado
    generate_maze(estado.maze,  dificultad);
    estado.x = 0;
    estado.y = 0;
    estado.steps = 0;
    estado.actions = list_create();
    return estado;
}

State *transition(State *actual, accion movimiento){
    State *nuevo = malloc(sizeof(State));
    if (!nuevo) exit(EXIT_FAILURE);

    *nuevo = *actual;

    nuevo -> FILA += movimiento.accionX;
    nuevo -> COL += movimiento.accionY;
    nuevo -> steps += 1;//actual -> steps +1;

    return nuevo;
}

List *getAdjNodes(State *actual){
    List *vecinos = list_create();

    accion acciones[] = {
        {-1,0},//arriba
        {1,0},//abajo
        {0,-1},//izq
        {0,1}//der
    };

    for (unsigned short i = 0; i < 4; i++){
        accion movimiento = acciones[i];

        int nuevaFila = actual -> FILA + movimiento.accionX;
        int nuevaCol = actual -> COL + movimiento.accionY;

        //verificar si se sale de los limites
        if (nuevaFila < 0 || nuevaFila >= N ||
            nuevaCol < 0 || nuevaCol >= N) continue;

        //verificar si estamos en un obstaculo
        if (actual -> maze[nuevaFila][nuevaCol] == OBSTACULO) continue;

        State *nuevo = transition(actual, movimiento);
        list_pushBack(vecinos, nuevo);
    }
    return vecinos;
}
/*
void dfs(const State *estado_inicial){
    State *copiaEstado = malloc(sizeof(State));
    if (!copiaEstado) exit(EXIT_FAILURE);

    copiaEstado = estado_inicial;
    copiaEstado -> actions = list_create();

    Stack *pila = stack_create();
    stack_push(pila, estado_inicial);

    while (!isEmpty(pila)){

    }


}*/

int main() {
    // Inicializar la semilla de aleatoriedad
    srand(time(NULL));

    int maze[N][N];
    int dificultad;

    // Solicitar la dificultad al usuario con validación
    do {
        printf("Ingrese la dificultad del laberinto (porcentaje de obstáculos, 0 a 100): ");
        scanf("%d", &dificultad);
        if (dificultad < 0 || dificultad > 100) {
            printf("Error: Por favor ingrese un valor válido entre 0 y 100.\n");
        }
    } while (dificultad < 0 || dificultad > 100);

    // Estado inicial del puzzle
    // Laberinto generado con la dificultad ingresada por el usuario
    State estado_inicial = crearEstadoInicial(maze, dificultad);

    // Imprime el estado inicial
    //printf("\nEstado inicial del puzzle:\n");
    //imprimirEstado(&estado_inicial);

    printf("Distancia L1: %d\n", distancia_L1(&estado_inicial));
/*
    // Ejemplo de heap (cola con prioridad)
    printf("\n***** EJEMPLO USO DE HEAP ******\nCreamos un Heap e insertamos 3 elementos con distinta prioridad\n");
    Heap* heap = heap_create();
    char* data = strdup("Cinco");
    printf("Insertamos el elemento %s con prioridad -5\n", data);
    heap_push(heap, data, -5 );
    data = strdup("Seis");
    printf("Insertamos el elemento %s con prioridad -6\n", data);
    heap_push(heap, data, -6 );
    data = strdup("Siete");
    printf("Insertamos el elemento %s con prioridad -7\n", data);
    heap_push(heap, data, -7 );

    printf("\nLos elementos salen del Heap ordenados de mayor a menor prioridad\n");
    while (heap_top(heap) != NULL){
        printf("Top: %s\n", (char*) heap_top(heap));
        heap_pop(heap);
    }
    printf("No hay más elementos en el Heap\n");
*/
    char opcion;
    do {
        printf("\npuzzle:\n");
        imprimirEstado(&estado_inicial);
        printf("\n\n***** EJEMPLO MENU ******\n");
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");

        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");

        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
        case '1':
            printf("\nbuscar mejor camino con tda cola\n");
          //dfs(&estado_inicial);
          break;
        case '2':
            printf("\nbuscar mejor camino con tda fila\n");
          //bfs(estado_inicial);
          break;
        case '3':
            printf("\nbuscar mejor camino con tda talento al fallo\n");
          //best_first(estado_inicial);
          break;
        }

        // Evitamos pausar y limpiar pantalla si el usuario eligió salir
        if (opcion != '4') {
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }

  } while (opcion != '4');

  return 0;
}
