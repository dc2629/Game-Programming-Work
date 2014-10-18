#include "Platformer.h"

using namespace std;

int main(int argc, char *argv[]){


	DemoApp App;
	App.buildLevel();
	while (!App.ProcessEvents()){
		App.UpdateandRender();

	}

	return 0;
}