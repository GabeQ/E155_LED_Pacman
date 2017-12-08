/* C file which contains the main function that will run the pacman game from the pi*/


#include "gameplayFinal.c"
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
			printf("User won game, game will speed up");
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
			counter = 0;
			initArray();
		}
		// Lose case. Will restart the game.
		if (lives == 0){
			printf("User is out of lives, will reset game\n");
			while(counter < 300){
				char received = sendCurrentGameArray(loseArray);
				counter += 1;
			}
			printf("Number of lives: %d\n", (int) lives);
			lives = 6;
			resetPacmanInfo();
			resetGhostInfo();
			ghostTimer = 0;
			counter = 0;
			waitTime = 25;
			randomness = 2;
			initArray();
		}

		// Sends gameArray to FPGA, gets keypad data, updates pacman and ghost info.
		char received = sendCurrentGameArray(gameArray);
		getPacmanInfo(gameArray,received);
		getGhostInfo(gameArray);
		// If pacman is alive and cycles > wait time, then allow ghost and pacman movements for one cycle.
		if (isAlive(gameArray)){
			if (cycles > waitTime) {
				cycles = 0;
				// Resets aiArray
				resetAIArray();
				// Game logic for movement of ghosts and pacman; increase the ghost timer.
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
}
