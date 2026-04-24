#ifndef GAME_H_
#define GAME_H_

#include <string.h> // for memset
#include <stdlib.h> // for returning success, failure
#include "GameConstAndEnums.h" // include constants and enums
#include "Player.h"
#include "Cursor.h"
#include "Display.h"
#include "Music.h"

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

int PlacePhase(Game *g); // ship placing phase
int GuessPhase(Game *g); // ship guessing phase

int PlaceShip(Game *g); // function to attempt to place ship, returns 0 for success, 1 for fail
int GuessTile(Game *g); // function for guessing at cursor location, returns 0 for success, 1 for fail

#endif /* GAME_H_ */