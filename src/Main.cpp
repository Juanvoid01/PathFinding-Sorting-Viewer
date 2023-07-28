#include "GUI.h"



int main() {
	

	GUI gui;


	//gui loop


	while (gui.running())
	{

		gui.update();

		gui.render();

	}

	//End of application
	
	return 0;
}


