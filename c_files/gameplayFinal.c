/* C file which contains the functions needed to alter the LED matrix state.
Created by Kai Kaneshina and Gabriel Quiroz*/

// Include files
#include "EasyPIO.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////
////////////// STRUCTURES, CONSTANTS, AND GLOBAL VARIABLES ///////////////////
//////////////////////////////////////////////////////////////////////////////

// Tuple type which will contain information about ghosts and pacman
typedef struct{
	int row;
	int col;
	int dir;
	int time;
	char block;
}Tuple;


// Global Constants
#define NORTH 1
#define EAST 2
#define SOUTH 4
#define WEST 8
#define MAXVAL 999

// Global Variables
Tuple pacmanInfo;
Tuple ghostInfo;
int cycles;
int win;
int waitTime = 25;
int lives = 6;
int ghostTimer = 0;
int randomness = 5;
int counter = 0;

// Array with the eight chars that represent colors
char difChar[8] = {'W', 'F', 'I', 'P', 'E', 'G', 'L', 'K'};

// 2D short array specifying a cells distance from pacman
short aiArray[32][32];

// 2D char array that will display the win case
char winArray[32][32];

// 2D char array that will contain the current gamestate
char gameArray[32][32];

// 2D char array that contains the current encoding of our game
char initGameArray[32][32] = {
//    0    1    2    3    4    5    6    7    8    9   10	11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31
	{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
	{'W', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'W', 'W', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'F', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'F', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'F', 'F', 'F', 'F', 'W', 'F', 'F', 'F', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'F', 'F', 'F', 'W', 'F', 'F', 'F', 'F', 'F', 'W'},
	{'W', 'W', 'F', 'W', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'W', 'F', 'W', 'W'},
	{'W', 'F', 'F', 'W', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'W', 'F', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'F', 'F', 'F', 'F', 'F', 'W', 'F', 'F', 'F', 'F', 'W', 'W', 'F', 'F', 'F', 'F', 'W', 'F', 'F', 'F', 'F', 'F', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'F', 'F', 'F', 'F', 'F', 'W', 'W', 'F', 'F', 'F', 'F', 'F', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'W', 'F', 'F', 'F', 'W', 'F', 'W', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'W', 'F', 'W', 'F', 'F', 'F', 'W', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'F', 'F', 'F', 'F', 'F', 'W', 'F', 'F', 'F', 'F', 'F', 'W', 'W', 'G', 'W', 'W', 'W', 'F', 'F', 'F', 'F', 'F', 'W', 'F', 'F', 'F', 'F', 'F', 'F', 'W'},
	{'W', 'W', 'W', 'W', 'W', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'W', 'W', 'W' ,'W', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'W', 'W', 'W', 'W', 'W'},
	{'W', 'W', 'W', 'W', 'W', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'W', 'W', 'W' ,'W', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'W', 'W', 'W', 'W', 'W'},
	{'W', 'F', 'F', 'F', 'F', 'F', 'F', 'W', 'F', 'F', 'F', 'F', 'F', 'W', 'W', 'W', 'W', 'W', 'W', 'F', 'F', 'F', 'F', 'F', 'W', 'F', 'F', 'F', 'F', 'F', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'W', 'F', 'F', 'F', 'W', 'F', 'W', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'W', 'F', 'W', 'F', 'F', 'F', 'W', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'P', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'F', 'F', 'F', 'F', 'F', 'W', 'W', 'F', 'F', 'F', 'F', 'F', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'F', 'F', 'F', 'F', 'F', 'W', 'F', 'F', 'F', 'F', 'W', 'W', 'F', 'F', 'F', 'F', 'W', 'F', 'F', 'F', 'F', 'F', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'F', 'W', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'W', 'F', 'F', 'W'},
	{'W', 'W', 'F', 'W', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'F', 'W', 'W', 'F', 'W', 'W'},
	{'W', 'F', 'F', 'F', 'F', 'F', 'W', 'F', 'F', 'F', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'F', 'F', 'F', 'W', 'F', 'F', 'F', 'F', 'F', 'W'},
	{'W', 'F', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'F', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'W', 'F', 'W', 'F', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'F', 'W'},
	{'W', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'W', 'W', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'W'},
	{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'}
};

// 2D char array that will display the lose case of our game
char loseArray[32][32] = {
//	   0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31
	 {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
	 {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
	 {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
 	 {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
 	 {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
 	 {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},
 	 {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'},
 	 {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'},
     {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'}

};


//////////////////////////////////////////////////////////////////////////////
///////////////////////// FUNCTIONS FOR THE GAME /////////////////////////////
//////////////////////////////////////////////////////////////////////////////


// Returns the lowest of two shorts. This function is used for the ghost AI.
short min(short a, short b){
	if (a < b){
		return a;
	}
	return b;
}


// Function that converts characters to RGB bits. SPI takes in 8 bits,
// so we send 4 bits corresponding to the current row we are at, as well as
// 4 bits corresponding to row + 16 in the matrix.
char CharToBits(char board) {
                if (board  == 'W') return 0b0001;  //blue
                if (board  == 'F') return 0b0111;  //white
                if (board  == 'I') return 0b0010;  //green
				if (board  == 'P') return 0b0110;  //yellow
				if (board  == 'E') return 0b0000;  //black
        		if (board  == 'G') return 0b0100;  //red
				if (board  == 'L') return 0b0011;  //cyan
				if (board  == 'K') return 0b0101;  //purple
}


// Sends the current game array RGB data to the FPGA. Returns the direction
// data for pacman in the variable returned.
char sendCurrentGameArray(char map[][32]){
	int i, j;
	char received, senddata, senddata1, combined;
    for (i = 1; i < 16; i++){
       	for (j = 0; j < 32; j++){
			senddata = CharToBits(map[i][j]);
			senddata1 = CharToBits(map[i+16][j]);
            combined = (senddata<<4) | senddata1;
            received = spiSendReceive(combined) & 0b11111111;
        }
	}
	for (j = 0; j < 32; j++){
		senddata = CharToBits(map[0][j]);
		senddata1 = CharToBits(map[16][j]);
        combined = (senddata<<4) | senddata1;
        received = spiSendReceive(combined) & 0b11111111;
	}
	return received;
}


// Initializes a new gameArray by looping though the values of initGameArray.
// Initializes SPI communication between the pi and FPGA.
void initArray(){
	int i, j;
	for (i = 0; i < 32; i++){
		for (j = 0; j< 32; j++){
			gameArray[i][j] = initGameArray[i][j];
		}
	}
	spiInit(500000, 0);
}


// Tests to see if there is any food in the gameArray. If there is no food,
// returns a 1.
int gameWon(){
	int i, j;
	int counter = 0;
	for (i = 0; i < 32; i++){
		for (j = 0; j < 32; j++){
			if (gameArray[i][j] == 'F'){
				return 0;
			}
		}
	}
	return 1;
}


// Generates the win array by randomly choosing values of the difChar array.
void genWinArray(){
	spiInit(100000, 0);
	int i, j;
	for (i = 0; i < 32; i++){
		for (j = 0; j < 32; j++){
			int id = rand() % 8;
			winArray[i][j] = difChar[id];
		}
	}
}


// Initializes data for the ghost of the game.
Tuple initGhost(Tuple ghost){
	ghost.dir = 0;
	ghost.time = 10;
	ghost.block = 'W';
	return ghost;
}


// Resets pacman's data for when he dies in the game.
void resetPacmanInfo(){
	pacmanInfo.dir = 0;
	pacmanInfo.row = 21;
	pacmanInfo.col = 15;
	pacmanInfo.block = 'E';
	gameArray[pacmanInfo.row][pacmanInfo.col] = 'P';
}


// Resets the ghost's data for when he kills pacman in the game.
void resetGhostInfo(){
	ghostInfo.dir = 0;
	ghostInfo.block = 'W';
	ghostInfo.row = 14;
	ghostInfo.col = 15;
	gameArray[ghostInfo.row][ghostInfo.col] = 'G';
}

// Updates pacman's current data (including position and direction).
void getPacmanInfo(char map[][32], int direction){
	int i, j;
	pacmanInfo.dir = direction;
	for (i = 0; i < 32; i++){
		for (j = 0; j < 32; j++){
			if (map[i][j] == 'P'){
				pacmanInfo.row = i;
				pacmanInfo.col = j;
			}
		}
	}
}


// Updates the ghost's current position data.
Tuple getGhostInfo(char map[][32]){
	int i, j;
	for (i = 0; i < 32; i++){
		for (j = 0; j < 32; j++){
			if (map[i][j] == 'G'){
				ghostInfo.row = i;
				ghostInfo.col = j;
			}
		}
	}
}


// Returns 1 if a given space is not a wall, else returns 0.
int freeSpace(int row, int col){
	if (gameArray[row][col] == 'W'){
		return 0;
	}
	return 1;
}


// Returns a 1 if the direction either pacman or the ghost wants
// to move to is a valid direction. Else returns a 0.
int canMove(Tuple object){
	int nextRow;
	int nextCol;
	int curRow = object.row;
	int curCol = object.col;
	if (object.dir == NORTH){
		nextRow = curRow - 1;
		freeSpace(nextRow, curCol);
	}
	else if (object.dir == SOUTH){
		nextRow = curRow + 1;
		freeSpace(nextRow, curCol);
	}
	else if (object.dir == EAST){
		nextCol = curCol + 1;
		freeSpace(curRow, nextCol);
	}
	else if (object.dir == WEST){
		nextCol = curCol - 1;
		freeSpace(curRow, nextCol);
	}
	// The following case was added if the user has not pressed a direction
	// on the keypad.
	else{			
		return 0;
	}
}


// If a ghost is on top of pacman, sets their current location to empty space,
// resets both pacman's and the ghost's info, decreases the number of lives by
// 1, resets the ghost timer, and returns 0. else, returns 1.
int isAlive(char map[][32]){
	if (ghostInfo.block == 'P' || pacmanInfo.block == 'G'){
		map[ghostInfo.row][ghostInfo.col] = 'E';
		map[pacmanInfo.row][pacmanInfo.col] = 'E';
		resetGhostInfo();
		resetPacmanInfo();
		lives -= 1;
		printf("Number of Lives Remaining: %d\n", (int) lives);
		ghostTimer = 0;
		return 0;
	}
	return 1;
}


// First tests if pacman can move in the giver direction. If he can, updates the 
// game matrix based on the movements of pacman.
void pacmanMovement(char map[][32]){
	if (canMove(pacmanInfo)){
		if (pacmanInfo.dir == NORTH){
			map[pacmanInfo.row][pacmanInfo.col] = 'E';
			pacmanInfo.block = map[pacmanInfo.row - 1][pacmanInfo.col];
			map[pacmanInfo.row - 1][pacmanInfo.col] = 'P';
		}
		else if (pacmanInfo.dir == SOUTH){
			map[pacmanInfo.row][pacmanInfo.col] = 'E';
			pacmanInfo.block = map[pacmanInfo.row + 1][pacmanInfo.col];
			map[pacmanInfo.row + 1][pacmanInfo.col] = 'P';
		}
		else if (pacmanInfo.dir == EAST){
			map[pacmanInfo.row][pacmanInfo.col] = 'E';
			pacmanInfo.block = map[pacmanInfo.row][pacmanInfo.col + 1];
			map[pacmanInfo.row][pacmanInfo.col + 1] = 'P';
		}
		else{
			map[pacmanInfo.row][pacmanInfo.col] = 'E';
			pacmanInfo.block = map[pacmanInfo.row][pacmanInfo.col - 1];
			map[pacmanInfo.row][pacmanInfo.col - 1] = 'P';
		}
	}
}


// Resets the AI array by setting each index of the array to MAXVAL, and then
// sets pacman's current location in the AI array to 0.
void resetAIArray(){
	int i, j;
	for (i = 0; i < 32; i++){
		for (j = 0; j < 32; j++){
			aiArray[i][j] = MAXVAL;
		}
	}
	aiArray[pacmanInfo.row][pacmanInfo.col] = 0;
}


// Updates the aiArray with a cell's distance from pacman
char updateAIArray(int row, int col){
	int i,j;

	char changed = 0;
	short minVal = aiArray[row][col];

	// For loop iterates through surrounding blocks of given row and column and
	// finds the lowest value from all nine cells (or returns the current minVal)
	for (i = 0; i < 9; i++){
		minVal = min(aiArray[row + (i%3) - 1][col + (i/3) - 1] + 1, minVal);
	}

	// If minVal is the same value of the given row and column, we know the aiArray
	// hasn't changed values. Thus, we are one step closer to update ghost movement
	changed = minVal != aiArray[row][col];
	aiArray[row][col] = minVal;

	// Goes through each of the 9 surrounding cells and calls updateAIArray on them if 
	// the cell is a free space and the cell has a value greater than the current cell.
	for (j = 0; j < 9; j ++){
		int newRow = row + (j%3) - 1;
		int newCol = col + (j/3) - 1;
		if (freeSpace(newRow, newCol) && aiArray[newRow][newCol] > (minVal + 1)){
			// This is done in order to break from the while loop in send6.c (only want
			// to stop updating aiArray if nothing else is changing)
			changed |= updateAIArray(newRow, newCol);
		}
	}
	return changed;
}


//AI for ghost movement. Updates the direction that the ghost should move.
void updateGhostDirection(int randomnessVal){
	int bestVal = 998;
	int bestDir = 0;

	// Uses the randomness variable to cause the ghost go in a random direction. As randomness
	// variable increases, chance for ghost to go in random direction decreases.
	if ( rand() % randomnessVal == 0 ) {
		int x, y;
		int dir = 0;
		do {
			// Directions are coded as a power of 2 --> LSL.
			dir = 1 << ( rand() % 4 );
			if ( dir == NORTH ) {
				x = ghostInfo.row - 1;
				y = ghostInfo.col;
			} else if ( dir == SOUTH ) {
				x = ghostInfo.row + 1;
				y = ghostInfo.col;
			} else if ( dir == EAST ) {
				x = ghostInfo.row;
				y = ghostInfo.col + 1;
			} else if ( dir == WEST ) {
				x = ghostInfo.row;
				y = ghostInfo.col - 1;
			}
		} while ( !freeSpace(x, y) );
		// Ghost gets current info and then returns.
		ghostInfo.dir = dir;
		return;
	}

	// Case where we don't have random direction, uses aiArray to find best direction to move.
	if (aiArray[ghostInfo.row - 1][ghostInfo.col] < bestVal){
		bestVal = aiArray[ghostInfo.row - 1][ghostInfo.col];
		bestDir = NORTH;
	}
	if (aiArray[ghostInfo.row + 1][ghostInfo.col] < bestVal){
		bestVal = aiArray[ghostInfo.row + 1][ghostInfo.col];
		bestDir = SOUTH;
	}
	if (aiArray[ghostInfo.row][ghostInfo.col + 1] < bestVal){
		bestVal = aiArray[ghostInfo.row][ghostInfo.col + 1];
		bestDir = EAST;
	}
	if (aiArray[ghostInfo.row][ghostInfo.col - 1] < bestVal){
		bestVal = aiArray[ghostInfo.row][ghostInfo.col - 1];
		bestDir = WEST;
	}
	ghostInfo.dir = bestDir;
}


// Tests to see if teh ghost can move based on ghostTimer (ghost.time must be
// greater than ghostTimer for the ghost to be able to move).
int ghostCanMove(Tuple ghost){
	if (ghostTimer < ghost.time){
		return 0;
	}
	else{
		return 1;
	}
}


// First tests if the ghost can move in the giver direction. If he can, 
// updates the game matrix based on the movements of pacman.
void ghostMovement(char map[][32], Tuple ghost){
	if (canMove(ghostInfo)){
		if (ghostInfo.dir == NORTH){
			map[ghostInfo.row][ghostInfo.col] = ghostInfo.block;
			ghostInfo.block = map[ghostInfo.row - 1][ghostInfo.col];
			map[ghostInfo.row - 1][ghostInfo.col] = 'G';
		}
		else if (ghostInfo.dir == SOUTH){
			map[ghostInfo.row][ghostInfo.col] = ghostInfo.block;
			ghostInfo.block = map[ghostInfo.row + 1][ghostInfo.col];
			map[ghostInfo.row + 1][ghostInfo.col] = 'G';
		}
		else if (ghostInfo.dir == EAST){
			map[ghostInfo.row][ghostInfo.col] = ghostInfo.block;
			ghostInfo.block = map[ghostInfo.row][ghostInfo.col + 1];
			map[ghostInfo.row][ghostInfo.col + 1] = 'G';
		}
		else{
			map[ghostInfo.row][ghostInfo.col] = ghostInfo.block;
			ghostInfo.block = map[ghostInfo.row][ghostInfo.col - 1];
			map[ghostInfo.row][ghostInfo.col - 1] = 'G';
		}
	}
}