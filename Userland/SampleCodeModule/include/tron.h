#ifndef _TRON_H_
#define _TRON_H_

#include <color.h>
#include <syscalls.h>
#include <lib.h>

#define MAX_WIDTH 1024
#define MAX_HEIGHT 768

#define	UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

//#define ENTER_KEY 0    // Puede ser 13 tmb (?)
#define LEFT_KEY 1
#define A_KEY 'a'
#define RIGHT_KEY 2
#define D_KEY 'd'
#define UP_KEY 3
#define W_KEY 'w'
#define DOWN_KEY 4
#define S_KEY 's'

typedef struct {

 /**
  * 	Player coordinates
  */ 
 int x, y;

 /**
  * 	Player direction
  */ 
 int dir;

 Color color;

} Player;

typedef struct {

 /**
  * 	Canvas dimensions
  */ 
 int width, height;

 /**
  * 	Logic board
  */ 
 char board[MAX_WIDTH][MAX_HEIGHT];

} Canvas;

int mainTron();

void startGame();

void playTron(Player *p1, Player *p2, Canvas *canvas);

void endGame(char* string, Canvas *canvas);

void dimensions(Canvas *canvas);

void drawPlayer(Player * p, Canvas *canvas);

void drawPlayers(Player *p1, Player *p2, Canvas *canvas);

void shortSleep();

void drawCanvas(Canvas *canvas, Color color);

int getKey();

void pause();

void endGame(char* string, Canvas *canvas);

/**
   * @brief Funcion que modifica las coordenadas de un jugador en una posicion.
   *
   * @param player jugador.
*/
void tick(Player *player);

/**
   * @brief Funcion que inicializa una matriz con ceros dados ancho y altura.
   *
   * @param canvas matriz a inicializar.
   * @param width ancho del canvas.
   * @param height altura del canvas.
*/
void initializeCanvas(Canvas *canvas);

/**
   * @brief Funcion que define los colores, coordenadas y direcciones iniciales de los jugadores.
   *
   * @param p1 jugador 1.
   * @param p2 jugador 2.
   * @param width ancho del canvas.
   * @param height altura del canvas.
*/
void initializePlayers(Player *p1, Player *p2, Canvas *canvas);

/**
   * @brief Funcion que valida si un punto esta incluido en width*height.
   *
   * @param x coordenada x del punto.
   * @param y coordenada y del punto.
   * @param width ancho del canvas.
   * @param height altura del canvas.
   * @return 1 si esta incluido, 0 si no.
*/
int isValidPoint(int x, int y, Canvas *canvas);

/**
   * @brief Funcion que analiza una nueva tecla apretada y modifica, o no, la direccion a la que apunta un jugador.
   *		Tambien permite salir del juego.
   *
   * @param p1 jugador 1.
   * @param p2 jugador 2.
   * @param oldKey ultima tecla apretada.
   * @param newKey nueva tecla apretada.
*/
int input(Player *p1, Player *p2, int *oldKey, int newKey);

#endif
