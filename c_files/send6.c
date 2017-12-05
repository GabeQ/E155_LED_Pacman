/* C file which contains the main function that will run the pacman game from the pi*/


#include "gameplay6.c"
#include "EasyPIO.h"

// Main function that runs the pacman game on the pi
int main(void){

	printf("Number of lives: %d\n", (int) lives);
	// Initializes variables that will be used in the game, SPI, and I/O
    pioInit();
    initArray();
    Tuple ghostInfo = initGhost(ghostInfo);

    //While loop that runs the game. Will break when the user loses game.
	while(1) {
		// Cycles is used to slow the game down (don't make movements until cycles
		// is greater than wait time).
		cycles += 1;
		// Win case; will make ghosts smarter and game faster.
		if (gameWon()){
			int counter = 0;
			while(counter < 300){
				genWinArray();
				char received = sendCurrentGameArray(winArray);
				counter += 1;
			}
			resetPacmanInfo();
			resetGhostInfo();
			randomness += 5;
			waitTime -= 10;
			ghostTimer = 0;
			initArray();
		}

		if (lives == 0){
			win = 0;
			break;
		}

		// Sends gameArray to FPGA, gets keypad data, updates pacman and ghost info, and resets ai array
		char received = sendCurrentGameArray(gameArray);
		getPacmanInfo(gameArray,received);
		getGhostInfo(gameArray);
		resetAIArray();

		// If pacman is alive and cycles > wait time, then allow ghost and pacman movements for one cycle.
		if (isAlive(gameArray)){
			if (cycles > waitTime) {
				cycles = 0;
				//Game logic for pacman
				if (ghostCanMove(ghostInfo)){
					while ( updateAIArray(pacmanInfo.row,pacmanInfo.col) ){
						updateGhostDirection(randomness);
						ghostMovement(gameArray, ghostInfo);
					}
				}
				pacmanMovement(gameArray);
				ghostTimer += 1;

			}
		}
    }
    // Lose case. Must exit program and reset LED matrix to play again.
	while(1){
		char received = sendCurrentGameArray(loseArray);
	}
}
