#include <string.h> // for memset
#include <stdlib.h> // for returning success, failure
#include "Player.h"
#include "Cursor.h"
#include "Display.h"

//Array dimensions for game board
#define H_WIDTH 8; // horizontal array dimensions
#define H_HEIGHT 3;

#define V_WIDTH 16; // vertical array dimensions
#define V_HEIGHT 2;

//Possible states for a tile to be in
typedef enum SquareState {NONE, HIT, MISS, CURSOR, PLACED} SquareState;

//Game phase enumerations
typedef enum GamePhase {PHASE_TITLE, PHASE_PLACE, PHASE_TURN, PHASE_END} GamePhase; //game phases enumeration
typedef enum ActivePlayer {PLAYER1, PLAYER2} ActivePlayer; //active player enumeration

//Game structure -- stores each part of the game data
typedef struct Game {
	//players for the game
	Player players[2];
	//game phase
	GamePhase gamePhase;
	//active player
	ActivePlayer activePlayer;
	//cursor
	Cursor cursor;
	//display
	Display display;
} Game;

//game flow functions
void InitGame(Game *g); // initialization function

int PlaceShip(Game *g); // function to attempt to place ship, returns 0 for success, 1 for fail

int GuessTile(Game *g) // function for guessing at cursor location, returns 0 for success, 1 for fail