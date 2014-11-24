#include"MatrixEntities.h"
#include"ParticleSystem.h"

#define RANDOM_NUMBER ((float)rand()/(float)RAND_MAX)

SDL_Window* displayWindow;


using namespace std;

void Setup(){
	//Main Setup
	SDL_Init(SDL_INIT_VIDEO);//Initializes SDL
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?
	glViewport(0, 0, 1280, 720);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions
}

bool ProcessEvents(SDL_Event& EVENT){
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {//If the Window is closed or the user quits the program, end the loop.
			return(true);
		}

	}
	return false;
};

void Update(float& lastFrameTicks){

	
}

void Render(){

	////Determines default coloring
	//Makes background default color
	
	//float width = 1, height = 1;

	//GLfloat quad[] = { -width / 2, height / 2,
	//	-width / 2, -height / 2,
	//	width / 2, -height / 2,
	//	width / 2, height / 2
	//};
	//glVertexPointer(2, GL_FLOAT, 0, quad);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glDrawArrays(GL_QUADS, 0, 4);

	SDL_GL_SwapWindow(displayWindow);
};


void CleanUp(){
	SDL_Quit();
}

int main(int argc, char *argv[]){
	SDL_Event EVENT;
	Setup();
	ParticleEmitter p(200);
	float lastFrameTicks = 0.0f;
	while (!ProcessEvents(EVENT)) {
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		glClear(GL_COLOR_BUFFER_BIT);
		
		p.Render();
		p.Update(elapsed);
		SDL_GL_SwapWindow(displayWindow);
	}
	CleanUp();
	return 0;
}