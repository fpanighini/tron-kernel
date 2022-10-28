// VER SYS...... USAR LIB (wrappers)

#include <tron.h>

#define DEFAULT_CANVAS_COLOR black
#define WAIT_MILLIS 60
#define SCALE 4
#define PLAYER1_WON "PLAYER 1 WON THE GAME!"
#define PLAYER2_WON "PLAYER 2 WON THE GAME!"
#define DRAW "DRAW!"


int mainTron() {
	sys_clearScreen();
	startGame();
	// VER restart();
	// quitMain();
    return 0;
}

void startGame() {
	Canvas canvas;
    Player p1, p2;
	dimensions(&canvas);

	initializePlayers(&p1, &p2, &canvas);
	initializeCanvas(&canvas);

	// VER cambio de color opcional y borde
	drawCanvas(&canvas, DEFAULT_CANVAS_COLOR);

    playTron(&p1, &p2, &canvas);
}

void playTron(Player *p1, Player *p2, Canvas *canvas) {
	int game = 1, oldKey = -1, newKey = -1, player1Lost = 0, player2Lost = 0;
	int P1X, P1Y, P2X, P2Y;
	

	while (game) {

		newKey = getKey();

		if(input(p1, p2, &oldKey, newKey))
			return;		

		tick(p1);
		tick(p2);

		P1X = p1->x;
		P1Y = p1->y;
		P2X = p2->x;
		P2Y = p2->y;


	    // VER BEEP cuando pierde

		if(!isValidPoint(P1X, P1Y, canvas)) {
			game = 0;
			player1Lost = 1;
			p1->color = white;
		}

		if(!isValidPoint(P2X, P2Y, canvas)) {
			game = 0;
			player2Lost = 1;
			p2->color = white;
		}

		if((P1X == P2X && P1Y == P2Y) || (player1Lost && player2Lost)) {
			game = 0;
			p1->color = white;
			p2->color = white;
			drawPlayers(p1, p2, canvas);
			endGame(DRAW, canvas);
			return;
		} else {
			if (canvas->board[P1X][P1Y] || player1Lost) {
				game = 0;
				p1->color = white;
				drawPlayers(p1, p2, canvas);
				endGame(PLAYER2_WON, canvas);
				return;
			}

			if (canvas->board[P2X][P2Y] || player2Lost) {
				game = 0;
				p2->color = white;
				drawPlayers(p1, p2, canvas);
				endGame(PLAYER1_WON, canvas);
				return;
			}

			canvas->board[P1X][P1Y] = 1;
			canvas->board[P2X][P2Y] = 1;
		}

		drawPlayers(p1, p2, canvas);

		// VER SPEED (hasta un valor x)
		shortSleep(10);
	}
}

// VER SCALE
void dimensions(Canvas *canvas) {
	canvas->width = sys_getScreenWidth() / SCALE;
	canvas->height = sys_getScreenHeight() / SCALE;
}

void drawPlayer(Player * p, Canvas *canvas){
	sys_drawRectangle(p->x - SCALE/2, p->y - SCALE/2, SCALE, SCALE, p->color);
}

void drawPlayers(Player *p1, Player *p2, Canvas *canvas) {
	drawPlayer(p1, canvas);
	drawPlayer(p2, canvas);
}

void shortSleep() {
	sys_wait(WAIT_MILLIS);
}

void drawCanvas(Canvas *canvas, Color color) {
	int y = (sys_getScreenHeight() - canvas->height)/2;
	int x = (sys_getScreenWidth() - canvas->width)/2;
	sys_drawRectangle(x, y, canvas->width, canvas->height, color);
}

// VER en lib.h!!!
int getTimedChar(){
    char c[2] = {1, 0};
    sys_timedRead(0, (char *)&c, 2, 50);
    return c[0];
}

int getKey() {
    return getTimedChar();
}

// VER cartel con msj
void pause() {
	int c = 0;
	while(c != 'p'){
		c = getChar();
	}
}

// VER lib.h
void drawRectangle(int x, int y, int width, int height, Color color) {
	sys_drawRectangle(x, y, width, height, color);
}

// VER lib.h
void printAt(int x, int y, char* string, Color color) {
	sys_writeAt(x, y, string, color);
}

void endGame(char* string, Canvas *canvas) {
	int y = (sys_getScreenHeight() - canvas->height)/2;
	int x = (sys_getScreenWidth() - canvas->width)/2;

	drawRectangle(x, y, canvas->width * 0.4, canvas->height * 0.2, white);
	printAt(x, y, string, black);

	// VER de agregar restart
}

/**
   * @brief Funcion que modifica las coordenadas de un jugador en una posicion.
   *
   * @param player jugador.
*/
void tick(Player *player) {
	switch(player->dir) {
		case UP:
			player->y += 1;
		case DOWN:
    		player->y -= 1;
		case LEFT:
    		player->x -= 1;
		case RIGHT:
    		player->x += 1;
	}
}

/**
   * @brief Funcion que inicializa una matriz con ceros dados ancho y altura.
   *
   * @param canvas estructura con la matriz a inicializar.
*/
void initializeCanvas(Canvas *canvas) {
	for(int i=0; i<canvas->width; i++)
   		for(int j=0; j<canvas->height; j++)
      		canvas->board[i][j] = 0;
}

/**
   * @brief Funcion que define los colores, coordenadas y direcciones iniciales de los jugadores.
   *
   * @param p1 jugador 1.
   * @param p2 jugador 2.
   * @param canvas estructura con el tamanio del board
*/
void initializePlayers(Player *p1, Player *p2, Canvas *canvas) {
	p1->color = red;
	p1->x = (int) canvas->width*0.2;
	p1->y = (int) canvas->height*0.5;
	p1->dir = RIGHT;

	p2->color = blue;	
	p2->x = (int) canvas->width*0.8;
	p2->y = (int) canvas->height*0.5;
	p2->dir = LEFT;
}

/**
   * @brief Funcion que valida si un punto esta incluido en width*height.
   *
   * @param x coordenada x del punto.
   * @param y coordenada y del punto.
   * @param canvas estructura con las medidas del board
   * @return 1 si esta incluido, 0 si no.
*/
int isValidPoint(int x, int y, Canvas *canvas) {
	if(x >= canvas->width || y >= canvas->height || x < 0 || y < 0)
		return 0;

	return 1;
}

/**
   * @brief Funcion que analiza una nueva tecla apretada y modifica, o no, la direccion a la que apunta un jugador.
   *		Tambien permite salir del juego.
   *
   * @param p1 jugador 1.
   * @param p2 jugador 2.
   * @param oldKey ultima tecla apretada.
   * @param newKey nueva tecla apretada.
*/
int input(Player *p1, Player *p2, int *oldKey, int newKey) {
	int p1Key = 0;

	// VER MAYUSCULAS
	if (newKey == 'q' || newKey == 'Q') {
    	return 1;
	}

	if (newKey == 'p' || newKey == 'P') {
    	pause();
	}

	if(*oldKey != newKey) {

		*oldKey = newKey;
		p1Key = 0;

		switch(p1->dir) {
			case UP:
			case DOWN:
				if (*oldKey == LEFT_KEY) {
					p1->dir = LEFT;
					p1Key = 1;
				} else if (*oldKey == RIGHT_KEY) {
					p1->dir = RIGHT;
					p1Key = 1;
				} break;
			case LEFT:
			case RIGHT:
				if(*oldKey == UP_KEY) {
					p1->dir = DOWN;
					p1Key = 1;
				} else if(*oldKey == DOWN_KEY) {
					p1->dir = UP;
					p1Key = 1;
				} break;
		}

		if(!p1Key) {
			switch(p2->dir) {
				case UP:
				case DOWN:
					if (*oldKey == A_KEY)
						p2->dir = LEFT;
					else if (*oldKey == D_KEY)
						p2->dir = RIGHT;
					break;
				case LEFT:
				case RIGHT:
					if (*oldKey == W_KEY)
						p2->dir = DOWN;
					else if (*oldKey == S_KEY)
						p2->dir = UP;
					break;
			}
		}
	}
	return 0;
}





// --------------------------------------  AGREGAR SI TODO SALE  ------------------------------------------------//

/*

	static void drawMenu() {
		//VER QUE PONER

	
		int center_x = game->panel->width/2;

		window_draw_string(center_x - 14, 1, " _____   ___    ___    _  _ ");
		window_draw_string(center_x - 14, 2, "|_   _| | _ \\  / _ \\  | \\| |");
		window_draw_string(center_x - 14, 3, "  | |   |   / | (_) | | .` |");
		window_draw_string(center_x - 14, 4, "  |_|   |_|_\\  \\___/  |_|\\_|");
		window_draw_string(center_x - 14, 6, "PRESS 'ENTER' TO START/RESTART"); 
		window_draw_string(center_x - 2, 8, "OR"); 
		window_draw_string(center_x - 7, 10, "'Q' TO QUIT"); 
	}




	void panelBounds(Panel* panel) {
		// Vertical bound
		for (int y = 0; y < panel->height; y++) {
		panel->map[y * panel->width] = BOUND; 
		panel->map[panel->width - 1 + y * panel->width] = BOUND; 
		}

		// Horizontal bound
		for (int x = 0; x < panel->width; x++) {
		panel->map[x + 0 * panel->width] = BOUND;
		panel->map[x + (panel->height - 1) * panel->width] = BOUND;
		}

		// Corner bounds
		panel->map[panel->width - 1] = BOUND;
		panel->map[panel->length - 1] = BOUND;
		panel->map[0] = BOUND;
		panel->map[(panel->height-1) * panel->width] = BOUND;
	}

	void drawPanel(Panel* panel) {
		int i, x, y;

		for (i = 0; i < panel->length; i++) {

			x = i % panel->width;
			y = (int) i/panel->width;
			if (panel->map[i]) {
				//SYSCALL para dibujar en x,y el color panel->map[id]
				//Window draw
				SYSCALL(x, y, panel->map[i]);
			}
		}
	}


	void clearPanel(Panel* panel) {
		// VER 
		memset(panel->map, BLACK, panel->length * sizeof(int));
	}


    if (gameOver) {

        //restart
        if (game->input->key == ENTER_KEY) {
            restart(game);
        }
        return;
    }



	static void draw_game_over(Game* game) {
    char *text = " YOU LOST ";
    if (((LightCycle *) ptr_array_get(players, 0))->alive) {
        text = " YOU WON ";
    }
    int x = (game->panel->width / 2) - strlen(text)/2;
    attron(COLOR_PAIR(COLOR_WHITE));

    //window_draw_string(x, 0, text); 

    attroff(COLOR_PAIR(COLOR_WHITE));
	}


	static void start_countdown(Game* game, int count) {
		drawPanel(game->panel);

		while(count-- > 0) {

			char text[20];
			sprintf(text, " START IN %d ", count);
			int x = (game->panel->width / 2) - strlen(text)/2;

			attron(COLOR_PAIR(COLOR_WHITE));

			//window_draw_string(x, 0, text); 

			attroff(COLOR_PAIR(COLOR_WHITE));

			//window_refresh();

			sleep(1);
		}
	}
*/