
#include "Game.h"


using namespace std;




int main(int argc, char* argv[]){
	srand(time(NULL)); // randomizes the choices
	Game g;
	
	//gets arguments sent in by console
	for (int i = 1; i < argc; ++i) 
		g.getArgument(argv[i]);
	
	g.mainMenu();

}