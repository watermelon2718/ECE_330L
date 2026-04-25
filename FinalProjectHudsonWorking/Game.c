#include "Game.h" // include all necessary constants and things

void InitGame(Game *g) {
    // CLEAR ALL MEMORY
    memset(g, 0, sizeof(Game));
    //initialize players
    PlayerInit(&(g->players[0]));
    PlayerInit(&(g->players[1]));
    // Set specific non-zero starting values for Game state
    g->gamePhase = PHASE_TITLE;
    g->activePlayer = PLAYER1;
    // Initialize Cursor
    CursorInit(&(g->cursor));
    DisplayInit(&(g->display));

    MessageInit(&(g->scrollMessage), "BATTLESHIP ", 150, 1);
    g->displayMode = MESSAGE;

    // Initialize song to main theme
    MusicInit(&(g->music), main_theme, 40, 1);
    //init buttons
    ButtonInit(&(g->modeButton), GPIOC, GPIO_PIN_10);
    ButtonInit(&(g->confirmButton), GPIOC, GPIO_PIN_11);

    //init pots
    InitPot(&(g->xPot), &(g->cursor), 1, X_AXIS);// on channel 1
    InitPot(&(g->yPot), &(g->cursor), 2, Y_AXIS);// on channel 2

}

void GameRun(Game *g) {
    switch (g->gamePhase) {

    case PHASE_TITLE:
        //do not move on to the next phase until confirm button pressed
        if(Confirm(g)){
            //change to placing phase
            g->gamePhase = PHASE_PLACE;
            //set message to placing phase banner
            Cutscene(g, "P1_PLACE_SHIPS ");
        }
        break;

    case PHASE_PLACE:
        //don't progress until button pressed to clear message
        if(g->displayMode == MESSAGE) {
            //if confirm pressed, continue the program flow
            if(Confirm(g)){
                g->displayMode = BOARD; // start displaying theboard
            }
        } else { // when no longer instructed to halt

            //if confirm pressed, place ship
            if(Confirm(g)) {
                PlaceShip(g);
            }

            //if mode placed, toggle vert/horiz
            if(Mode(g)) {
                CursorHorizVertToggle(&(g->cursor));
            }

            // Get a pointer to the current player
            Player *p = &(g->players[g->activePlayer]);

            // If current player is done placing
            if (p->doubleSegmentShipsLeftToPlace == 0 && p->singleSegmentShipsLeftToPlace == 0) {
                if (g->activePlayer == PLAYER1) {
                    TogglePlayer(g);
                    Cutscene(g, "P2_PLACE_SHIPS ");
                    g->displayMode = MESSAGE;
                } else {
                    // Player 2 is done, move to guessing phase
                    g->gamePhase = PHASE_TURN;
                    TogglePlayer(g); // Set back to P1 for the first turn
                    Cutscene(g, "P1_GUESS_TITLE ");
                    g->displayMode = MESSAGE;
                }
            }
        }

        break;
    case PHASE_TURN:
        //if confirm is pressed
        if(Confirm(g)) {
            if(GuessTile(g) == 0) {

            }
            
        }

    }
}

// function to attempt to place ship, returns 0 for success, 1 for fail
int PlaceShip(Game *g) {
    //checking for horizontal or vertical validity
    switch(g->cursor.cursorMode) {
        case VERTICAL:
            //if nothing is placed in the placement array of the player placing right now, place there
            if(g->players[g->activePlayer].vertPlacementArray[g->cursor.xPos][g->cursor.yPos] == NONE){
                g->players[g->activePlayer].vertPlacementArray[g->cursor.xPos][g->cursor.yPos] = PLACED; 
                return 0; //success
            } else {
                return -1; // placement failed
            }
            break;

        case HORIZONTAL:
            //if nothing is placed in the placement array of the player placing right now, place there
            if(g->players[g->activePlayer].horizPlacementArray[g->cursor.xPos][g->cursor.yPos] == NONE){
                g->players[g->activePlayer].horizPlacementArray[g->cursor.xPos][g->cursor.yPos] = PLACED; 
                return 0; //success
            } else {
                return -1; // placement failed
            }
            break;        
    }
}
int GuessTile(Game *g) {
    //guess a tile
    switch(g->cursor.cursorMode) {
        case VERTICAL:

            //if tile at cursor hasn't been guessed yet
            if(g->players[g->activePlayer].vertGuessArray[g->cursor.xPos][g->cursor.yPos] == NONE){
                //set the tile in the guess array as a hit if the corresponding tile is placed in the opponent's placement array, otherwise miss
                g->players[g->activePlayer].vertGuessArray[g->cursor.xPos][g->cursor.yPos] = (g->players[1-g->activePlayer].vertPlacementArray[g->cursor.xPos][g->cursor.yPos]==PLACED) ? HIT : MISS;
                return 0; //success
            } else {
                return -1; // guess invalid
            }
            break;

        case HORIZONTAL:
            //if tile at cursor hasn't been guessed yet, set it to match opponent's placement array
            if(g->players[g->activePlayer].horizPlacementArray[g->cursor.xPos][g->cursor.yPos] == NONE){
               //set the tile in the guess array as a hit if the corresponding tile is placed in the opponent's placement array
                g->players[g->activePlayer].horizGuessArray[g->cursor.xPos][g->cursor.yPos] = (g->players[1-g->activePlayer].horizPlacementArray[g->cursor.xPos][g->cursor.yPos]==PLACED) ? HIT : MISS;
                return 0; //success
            } else {
                return -1; // guess invalid
            }
            break;        
    }
} // function for guessing at cursor location, returns 0 for success, -1 for fail


//switches player elegantly
void TogglePlayer(Game *g) {
    if(g->activePlayer == PLAYER1) {
        g->activePlayer = PLAYER2;
    } else if (g->activePlayer == PLAYER2) {
        g->activePlayer = PLAYER1;
    }
}

//renders display and outputs, used by systick
void DisplayOutput(Game *g) {
    if(g->displayMode == MESSAGE) {
        DisplayScrollMessage(&(g->scrollMessage));
    } else if (g->displayMode == BOARD) {
        DisplayBoard(g);
    }
}

//polls all inputs, used in the main while(1)
void GamePoll(Game *g) {
    //update buttons
    ButtonUpdate(&(g->modeButton));
    ButtonUpdate(&(g->confirmButton));
    //update potentiometers
    PotUpdate(&(g->xPot));
    PotUpdate(&(g->yPot));
}

//helper functions

//confirm button press
int Confirm(Game *g) {
    return WasPressed(&(g->confirmButton));
}

//mode button press
int Mode(Game *g){
    return WasPressed(&(g->modeButton));
}

//trigger next text phase
void Cutscene(Game *g, char *c) {
    g->displayMode = MESSAGE;
    SetMessage(&(g->scrollMessage), c, 150, 1);
}