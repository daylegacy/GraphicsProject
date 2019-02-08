#include "libs.h"



int main() {

	//CREATE WINDOW
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	const int GLmajorVersion = 4;
	const int GLminorVersion = 6;

	Game game("window", WINDOW_WIDTH, WINDOW_HEIGHT, 
		GLmajorVersion, GLminorVersion, true);

	//MAIN LOOP
	while (!game.getWindowShouldClose()) {
		//UPDATE INPUT
		game.update();

		game.render();
	}
	return 0;
}