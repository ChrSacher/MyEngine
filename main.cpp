

#include <stdio.h>
#include <string>
#include "Maingame.h"


int main( int argc, char* args[] )
{
	Maingame *mygame = new Maingame();
	//Starten
	mygame->run();
	delete(mygame);
	return 0;
}






