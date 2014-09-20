#include"Entity.h"

SDL_Window* displayWindow;

Entity cat;
void Setup(){
	//Main Setup
	SDL_Init(SDL_INIT_VIDEO);//Initializes SDL
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?
	glViewport(0, 0, 800, 600);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions
	cat.textureLocation = "nyan.png";
};

bool ProcessEvents(SDL_Event& EVENT){
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {//If the Window is closed or the user quits the program, end the loop.
			return(true);
		}
	}

};

void Update(){

};


int main(int argc, char *argv[]){

	Setup();

	SDL_Event EVENT; //Logs the I/O of the user
	bool done = false;//Running/Updating Windows requires loops.

	while (!ProcessEvents(EVENT)) {
		glClearColor(55.0f / 255.0f, 84.0f / 255.0f, 229.0f / 255.0f, 1.0f);//Determines default coloring
		glClear(GL_COLOR_BUFFER_BIT);//Makes background default color
		cat.Draw();
		SDL_GL_SwapWindow(displayWindow);//Something about there being two windows, swap the one that is visible and the one that is being programmed.
	}

	SDL_Quit();

	return 0;
}