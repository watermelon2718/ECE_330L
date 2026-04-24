//Array dimensions for game board
#define H_WIDTH 8; // horizontal array dimensions
#define H_HEIGHT 3;

#define V_WIDTH 16; // vertical array dimensions
#define V_HEIGHT 2;

//Possible states for a tile to be in
typedef enum TileState {NONE, HIT, MISS, CURSOR, PLACED} TileState;

//Game phase enumerations
typedef enum GamePhase {PHASE_TITLE, PHASE_PLACE, PHASE_TURN, PHASE_END} GamePhase; //game phases enumeration
typedef enum ActivePlayer {PLAYER1, PLAYER2} ActivePlayer; //active player enumeration
