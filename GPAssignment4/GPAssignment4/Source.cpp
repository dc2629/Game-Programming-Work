#include"DemoApp.h"

int main(int argc, char *argv[]){


	DemoApp App;

	while (!App.ProcessEvents()){
		App.Render();
	}
	return 0;

}