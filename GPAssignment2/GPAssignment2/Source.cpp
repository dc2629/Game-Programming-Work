#include"Entity.h"

SDL_Window* displayWindow;

Entity Lcat, Rcat, yarn;

void Setup(){
	//Main Setup
	SDL_Init(SDL_INIT_VIDEO);//Initializes SDL
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?
	glViewport(0, 0, 800, 600);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions

	Lcat.textureLocation = "nyan.png";
	Lcat.height = 0.3f;
	Lcat.width = 0.3f;
	Lcat.x = -1.2f;

	Rcat.textureLocation = "rnyan.png";
	Rcat.height = 0.3f;
	Rcat.width = 0.3f;
	Rcat.x = 1.2f;

	yarn.textureLocation = "yarn.png";
	yarn.height = 0.15f;
	yarn.width = 0.15f;


};

bool ProcessEvents(SDL_Event& EVENT){
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {//If the Window is closed or the user quits the program, end the loop.
			return(true);
		}

	}
	return false;
};

void Update(float& lastFrameTicks){
	//Fluid Movement is based on fps
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	//Movement programming for cats
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_UP]){
		if (Lcat.y<.9f)
			Lcat.y += elapsed * 0.8f;
	}
	else if (keys[SDL_SCANCODE_DOWN]){
		if (Lcat.y>-.9f)
			Lcat.y -= elapsed * 0.8f;
	}
	if (keys[SDL_SCANCODE_W]){
		if (Rcat.y<.9f)
			Rcat.y += elapsed * 0.8f;

	}
	else if (keys[SDL_SCANCODE_S]){
		if (Rcat.y>-.9f)
			Rcat.y -= elapsed * 0.8f;
	}
};

void Render(){
	glClearColor(55.0f / 255.0f, 84.0f / 255.0f, 229.0f / 255.0f, 1.0f);//Determines default coloring
	glClear(GL_COLOR_BUFFER_BIT);//Makes background default color
	Lcat.Draw();
	Rcat.Draw();
	yarn.Draw();
};

void CleanUp(){
	SDL_Quit();
}

int main(int argc, char *argv[]){

	Setup();

	SDL_Event EVENT; //Logs the I/O of the user
	float lastFrameTicks = 0.0f;

	while (!ProcessEvents(EVENT)) {
		Update(lastFrameTicks);
		Render();
		SDL_GL_SwapWindow(displayWindow);//Something about there being two windows, swap the one that is visible and the one that is being programmed.
	}

	CleanUp();

	return 0;
}