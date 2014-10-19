#include "Platformer.h"

using namespace std;

int main(int argc, char *argv[]){


	DemoApp App;
	while (!App.ProcessEvents()){
		App.buildLevel();
		App.UpdateandRender();

	}

	return 0;
}