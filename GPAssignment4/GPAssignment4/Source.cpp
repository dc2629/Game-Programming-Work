#include"Platformer.h"


int main(int argc, char *argv[]){


	DemoApp App;
	App.Setup();
	while (!App.ProcessEvents()){
		App.Render();

	}
	return 0;

}