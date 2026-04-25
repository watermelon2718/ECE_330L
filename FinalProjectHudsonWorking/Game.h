#ifndef GAME_H_
#define GAME_H_

#include <string.h> // for memset
#include <stdlib.h> // for returning success, failure
#include "GameConstAndEnums.h" // include constants and enums
#include "Player.h" // player structures and data
#include "Cursor.h" // cursor data 
#include "Button.h" // button
#include "Display.h" // display data
#include "ScrollMessage.h"
#include "Music.h" // music
#include "SoundArrays.h" // sound files

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
	//scrollmessage
	ScrollMessage scrollMessage;
	//displaymode
	DisplayMode displayMode;
	//song
	Music music;

	Button modeButton; // for switching modes 

	Button confirmButton; // for selecting/confirming an option
} Game;

//init
void InitGame(Game *g); // initialization function

//game flow functions
void GameRun(Game *g); // run game control
void Cutscene(Game *g, char *c); // trigger cutscene
int PlaceShip(Game *g); // function to attempt to place ship, returns 0 for success, 1 for fail
int GuessTile(Game *g); // function for guessing at cursor location, returns 0 for success, 1 for fail
void TogglePlayer(Game *g); // toggle player
void DisplayOutput(Game *g); // display to displays
void GamePoll(Game *g); // poll inputs

#endif /* GAME_H_ */