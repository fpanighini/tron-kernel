#include <tron.h>

#define DEFAULT_CANVAS_COLOR black
#define BOARD_SCALE 8
#define CANVAS_SCALE 4
#define FONT_SIZE 16

#define PLAYER1_WON "PLAYER 1 WON"
#define PLAYER2_WON "PLAYER 2 WON"
#define DRAW "IT'S A DRAW!"

// VER score counter
//int player1Score = 0, player2Score = 0;


int mainTron() {
    do {
        clear();
        startGame();
        endInfo();
    } while(getChar() != 'q');
    // VER restart
    return 0;
}

void startGame() {

    Canvas canvas;
    Player p1, p2;
    dimensions(&canvas);

    initializePlayers(&p1, &p2, &canvas);
    initializeCanvas(&canvas);

    // VER cambios de color opcional y borde
    drawCanvas(&canvas, DEFAULT_CANVAS_COLOR);
    drawPlayers(&p1, &p2, &canvas);

    countdown();

    drawCanvas(&canvas, DEFAULT_CANVAS_COLOR);

    playTron(&p1, &p2, &canvas);
}

void playTron(Player *p1, Player *p2, Canvas *canvas) {

    drawInfo();
    int game = 1, oldKey = -1, newKey = -1;

    canvas->board[p1->x][p1->y] = 1;
    canvas->board[p2->x][p2->y] = 1;

    while (game) {

        newKey = getKey();

        if(input(p1, p2, &oldKey, newKey))
            return;		

        tick(p1);
        tick(p2);

        if(!validPositions(p1, p2, canvas))
            return;

        canvas->board[p1->x][p1->y] = 1;
        canvas->board[p2->x][p2->y] = 1;

        drawPlayers(p1, p2, canvas);
    }
}

int validPositions(Player *p1, Player *p2, Canvas *canvas) {
    int player1Lost = 0, player2Lost = 0;
    int P1X = p1->x;
    int P1Y = p1->y;
    int P2X = p2->x;
    int P2Y = p2->y;

    int opposedDir = p1->dir - p2->dir;
    int posYDiff = P1Y - P2Y;
    int posXDiff = P1X - P2X;


    // VER BEEP cuando pierde

    if(!isValidPoint(P1X, P1Y, canvas)) {
        player1Lost = 1;
        p1->color = white;
    }

    if(!isValidPoint(P2X, P2Y, canvas)) {
        player2Lost = 1;
        p2->color = white;
    }
    
    if(P1X == P2X && (posYDiff<2 && posYDiff>-2) && (p1->dir == UP || p1->dir == DOWN) && (opposedDir == 1 || opposedDir == -1)) 
        return gameTied(p1, p2, canvas);
    
    if(P1Y == P2Y && (posXDiff<2 && posXDiff>-2) && ((p1->dir == LEFT && p2->dir != DOWN) || p1->dir == RIGHT) && (opposedDir == 1 || opposedDir == -1))
        return gameTied(p1, p2, canvas);

    if(canvas->board[P1X][P1Y])
        player1Lost = canvas->board[P1X][P1Y];
    
    if(canvas->board[P2X][P2Y])
        player2Lost = canvas->board[P2X][P2Y];

    if((P1X == P2X && P1Y == P2Y) || (player1Lost && player2Lost))
        return gameTied(p1, p2, canvas);
    
    if (player1Lost) {
        p1->color = white;
        drawPlayers(p1, p2, canvas);
        endGame(PLAYER2_WON, canvas);
        return 0;
    }

    if (player2Lost) {
        p2->color = white;
        drawPlayers(p1, p2, canvas);
        endGame(PLAYER1_WON, canvas);
        return 0;
    }
    
    return 1;
}

int gameTied(Player *p1, Player *p2, Canvas *canvas) {
    p1->color = white;
    p2->color = white;
    drawPlayers(p1, p2, canvas);
    endGame(DRAW, canvas);
    return 0;
}

// VER SCALE
void dimensions(Canvas *canvas) {
    canvas->width = getScreenWidth() / BOARD_SCALE;
    canvas->height = getScreenHeight() / BOARD_SCALE;
}

void drawPlayer(Player * p, Canvas *canvas){
    int y = (getScreenHeight() - canvas->height*CANVAS_SCALE)/2;
    int x = (getScreenWidth() - canvas->width*CANVAS_SCALE)/2;
    drawRectangle(x + p->x*CANVAS_SCALE - CANVAS_SCALE/2, y + p->y*CANVAS_SCALE - CANVAS_SCALE/2, CANVAS_SCALE, CANVAS_SCALE, p->color);
}

void drawPlayers(Player *p1, Player *p2, Canvas *canvas) {
    drawPlayer(p1, canvas);
    drawPlayer(p2, canvas);
}

void drawCanvas(Canvas *canvas, Color color) {
    int y = (getScreenHeight() - canvas->height*CANVAS_SCALE)/2 - CANVAS_SCALE/2;
    int x = (getScreenWidth() - canvas->width*CANVAS_SCALE)/2 - CANVAS_SCALE/2;
    Color borderColor = gray;
    int borderWidth = 10;
    drawRectangle(x-borderWidth/2, y-borderWidth/2, canvas->width*CANVAS_SCALE+borderWidth, canvas->height*CANVAS_SCALE+borderWidth, borderColor);
    drawRectangle(x, y, canvas->width*CANVAS_SCALE, canvas->height*CANVAS_SCALE, color);
}

int getKey() {
    return getTimedChar();
}

// VER cartel con msj
void pause() {
    clearInfo();
    char * pauseMsg = " Game paused, to continue press P";
    int recWidth = 280;
    int recHeight = 35;
    int x = getScreenWidth()/2 - recWidth/2; 
    int y = FONT_SIZE*8;
    int lineAt = y/FONT_SIZE;

    drawRectangle(x, y - 10, recWidth, recHeight, white);
    printStringAt(x, lineAt, pauseMsg, black);

    int c = 0;
    while(c != 'p'){
        c = getChar();
    }

    drawRectangle(x, y - 10, recWidth, recHeight, black);
    drawInfo();
}

void drawInfo() {
    char * pauseMsg = "   To pause press P, to restart press R";
    int recWidth = 340;
    int recHeight = 35;
    int x = getScreenWidth()/2 - recWidth/2; 
    int y = FONT_SIZE*8;
    int lineAt = y/FONT_SIZE;

    drawRectangle(x, y - 10, recWidth, recHeight, white);
    printStringAt(x, lineAt, pauseMsg, black);
}

void clearInfo() {
    int recWidth = 340;
    int recHeight = 35;
    int x = getScreenWidth()/2 - recWidth/2; 
    int y = FONT_SIZE*8;

    drawRectangle(x, y - 10, recWidth, recHeight, black);
}

void endGame(char* string, Canvas *canvas) {
    endInfo();
    
    beep(1100);
    int recWidth = canvas->width*0.8 + 5;
    int recHeight = 35;
    int x = getScreenWidth()/2 - recWidth/2; 
    int y = getScreenHeight() - getScreenHeight()/8 - recHeight/2 - 10;
    int lineAt = y/FONT_SIZE + 1;

    drawRectangle(x, y, recWidth, recHeight, white);
    printStringAt(x + 5, lineAt, string, black);
}

void endInfo() {
    clearInfo();
    char * restartMsg = " To restart press any key!";
    int recWidth = 220;
    int recHeight = 35;
    int x = getScreenWidth()/2 - recWidth/2; 
    int y = getScreenHeight()/8 - recHeight/2;
    int lineAt = y/FONT_SIZE + 1;

    drawRectangle(x, y - 10, recWidth, recHeight, gray);
    printStringAt(x, lineAt, restartMsg, white);

    char * quitMsg = "  To quit press Q";
    recWidth = 160;
    x = getScreenWidth()/2 - recWidth/2; 
    y = FONT_SIZE*8;
    lineAt = y/FONT_SIZE;

    drawRectangle(x, y - 10, recWidth, recHeight, gray);
    printStringAt(x, lineAt, quitMsg, white);
}

/**
 * @brief Funcion que modifica las coordenadas de un jugador en una posicion.
 *
 * @param player jugador.
 */
void tick(Player *player) {
    switch(player->dir) {
        case UP:
            player->y -= 1;
            break;
        case DOWN:
            player->y += 1;
            break;
        case LEFT:
            player->x -= 1;
            break;
        case RIGHT:
            player->x += 1;
            break;
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
    p1->x = (int) canvas->width*0.8;
    p1->y = (int) canvas->height*0.5;
    p1->dir = LEFT;

    p2->color = blue;	
    p2->x = (int) canvas->width*0.2;
    p2->y = (int) canvas->height*0.5;
    p2->dir = RIGHT;
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

    if (newKey == 'r')
        return 1;
    

    if (newKey == 'p')
        pause();
    

    if (*oldKey != newKey){
        *oldKey = newKey;

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
                    p1->dir = UP;
                    p1Key = 1;
                } else if(*oldKey == DOWN_KEY) {
                    p1->dir = DOWN;
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
                        p2->dir = UP;
                    else if (*oldKey == S_KEY)
                        p2->dir = DOWN;
                    break;
            }
        }
    }
    return 0;
}


void countdown() {

    char * msg1 = " 3 ";
    char * msg2 = " 2 ";
    char * msg3 = " 1 ";
    char * msg4 = " GO! ";

    int squareSize = 35;
    int x = getScreenWidth()/2 - squareSize/2;
    int y = getScreenHeight()/2 - squareSize/2 - 10;
    int lineAt = y/FONT_SIZE + 1; 

    drawRectangle(x, y, squareSize, squareSize, white);
    printStringAt(x + 5, lineAt, msg1, black);
    shortSleep(500);

    drawRectangle(x, y, squareSize, squareSize, DEFAULT_CANVAS_COLOR);

    drawRectangle(x, y, squareSize, squareSize, white);
    printStringAt(x + 5, lineAt, msg2, black);
    shortSleep(500);

    drawRectangle(x, y, squareSize, squareSize, DEFAULT_CANVAS_COLOR);

    drawRectangle(x, y, squareSize, squareSize, white);
    printStringAt(x + 5, lineAt, msg3, black);
    shortSleep(500);

    drawRectangle(x, y, squareSize, squareSize, DEFAULT_CANVAS_COLOR);

    drawRectangle(x, y, squareSize, squareSize, white);
    printStringAt(x, lineAt, msg4, black);
    shortSleep(500);
}

/*

void drawMenu() {
    //VER QUE PONER
    int center_x = game->panel->width/2;

    window_draw_string(center_x - 14, 2, "TRON - LIGHT CYCLES!");
    window_draw_string(center_x - 14, 6, "Press any key to start"); 
    window_draw_string(center_x - 7, 10, "Press 'q' to quit"); 
}

*/
