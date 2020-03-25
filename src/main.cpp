#include "player.h"

/*
  Main driver for Player
 */
int main (int argc, char *argv[]) {
	
	// create player object
	Player *p = new Player();
	
	// run the player client
	int status = p->run(argc, argv);
	
	// free memory and return the exit status of our program
	delete p;
	return status;
}

