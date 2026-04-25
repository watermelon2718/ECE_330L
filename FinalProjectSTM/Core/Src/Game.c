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

    MessageInit(&(g->scrollMessage), "        BATTLESHIP             PRESS TO PLAY              ", 150, 1);
    g->displayMode = MESSAGE;

    // Initialize song to main theme
    MusicInit(&(g->music), main_theme, 40, 1);
    //init buttons
    ButtonInit(&(g->modeButton), GPIOC, GPIO_PIN_10);
    ButtonInit(&(g->confirmButton), GPIOC, GPIO_PIN_11);

    //init pots
    InitPot(&(g->xPot), &(g->cursor), 1, X_AXIS, ADC1);// on channel 1
    InitPot(&(g->yPot), &(g->cursor), 2, Y_AXIS, ADC2);// on channel 2

}

void GameRun(Game *g) {
    switch (g->gamePhase) {

    case PHASE_TITLE:
        //do not move on to the next phase until confirm button pressed
        if(Confirm(g)){
            //change to placing phase
            g->gamePhase = PHASE_PLACE;
            //set message to placing phase banner
            Cutscene(g, "        PLAYER 1 PLACE SHIPS ");
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
            if (p->shipsToPlace == 0) {
                if (g->activePlayer == PLAYER1) {
                    g->cursor.cursorLength = DOUBLE_SEG;
                    TogglePlayer(g);
                    Cutscene(g, "        PLAYER 2 PLACE SHIPS ");
                    g->displayMode = MESSAGE;
                } else {
                    // Player 2 is done, move to guessing phase
                    g->gamePhase = PHASE_TURN;
                    TogglePlayer(g); // Set back to P1 for the first turn
                    Cutscene(g, "        PLAYER 1 GUESS TILE ");
                    g->displayMode = MESSAGE;
                }
            } else if (p->shipsToPlace == 3) {
                g->cursor.cursorLength = SINGLE_SEG;
            }
        }

        break;
    case PHASE_TURN:
        if(g->displayMode == MESSAGE) {
            //if confirm pressed, continue the program flow
            if(Confirm(g)){
                g->displayMode = BOARD; // start displaying theboard
            }
        } else {
            //if confirm is pressed
            if(Confirm(g)) {
                int guess = GuessTile(g);
                if(guess == 0) {
                    TogglePlayer(g);
                    Cutscene(g, (g->activePlayer == PLAYER1) ? "        PLAYER 1 GUESS TILE " : "        PLAYER 2 GUESS TILE ");
                } else if (guess == 1) { // if guess is a hit, increase a player's hitcounter
                    Cutscene(g, (g->activePlayer == PLAYER1) ? "        HIT PLAYER 2 SHIP         GUESS AGAIN " : "        HIT PLAYER 1 SHIP         GUESS AGAIN ");
                    g->players[g->activePlayer].hitCount++;
                    //if all ships are sunk, active player wins
                    if(g->players[g->activePlayer].hitCount >= 7){
                        g->gamePhase = PHASE_END;
                        Cutscene(g, (g->activePlayer == PLAYER1) ? "        PLAYER 1 WINS        PRESS TO PLAY AGAIN " : "        PLAYER 2 WINS        PRESS TO PLAY AGAIN ");
                    } 
                }
                
            }
        }
        break;
    case PHASE_END:
        if(Confirm(g)){
            //change to placing phase
            g->gamePhase = PHASE_TITLE;
            InitGame(g);
            //set message to placing phase banner
            Cutscene(g, "        BATTLESHIP             PRESS TO PLAY              ");
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
                if(g->cursor.cursorLength == DOUBLE_SEG) { g->players[g->activePlayer].vertPlacementArray[g->cursor.xPos][g->cursor.yPos+1] = PLACED;} 
                g->players[g->activePlayer].shipsToPlace--;
                return 0; //success
            } else {
                return -1; // placement failed
            }
            break;

        case HORIZONTAL:
            //if nothing is placed in the placement array of the player placing right now, place there
            if(g->players[g->activePlayer].horizPlacementArray[g->cursor.xPos][g->cursor.yPos] == NONE){
                g->players[g->activePlayer].horizPlacementArray[g->cursor.xPos][g->cursor.yPos] = PLACED; 
                if(g->cursor.cursorLength == DOUBLE_SEG) { g->players[g->activePlayer].horizPlacementArray[g->cursor.xPos+1][g->cursor.yPos] = PLACED;} 
                g->players[g->activePlayer].shipsToPlace--;
                return 0; //success
            } else {
                return -1; // placement failed
            }
            break;        
    }

    return -1;
}
int GuessTile(Game *g) {
    //guess a tile
    switch(g->cursor.cursorMode) {
        case VERTICAL:

            //if tile at cursor hasn't been guessed yet
            if(g->players[g->activePlayer].vertGuessArray[g->cursor.xPos][g->cursor.yPos] == NONE){
                //set the tile in the guess array as a hit if the corresponding tile is placed in the opponent's placement array, otherwise miss
                int isHit = (g->players[1-g->activePlayer].vertPlacementArray[g->cursor.xPos][g->cursor.yPos]==PLACED);
                g->players[g->activePlayer].vertGuessArray[g->cursor.xPos][g->cursor.yPos] = (isHit) ? HIT : MISS;
               
                return isHit; //returns 1 for hit, 0 for miss
            } else {
                return -1; // guess invalid
            }
            break;

        case HORIZONTAL:
            //if tile at cursor hasn't been guessed yet, set it to match opponent's placement array
            if(g->players[g->activePlayer].horizGuessArray[g->cursor.xPos][g->cursor.yPos] == NONE){
               //set the tile in the guess array as a hit if the corresponding tile is placed in the opponent's placement array
                int isHit = (g->players[1-g->activePlayer].horizPlacementArray[g->cursor.xPos][g->cursor.yPos]==PLACED);
                g->players[g->activePlayer].horizGuessArray[g->cursor.xPos][g->cursor.yPos] = (isHit) ? HIT : MISS;
                
                return isHit; //returns 1 for hit, 0 for miss
            } else {
                return -1; // guess invalid
            }
            break;        
    }

    return -1;
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
    UpdatePot(&(g->xPot));
    UpdatePot(&(g->yPot));
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