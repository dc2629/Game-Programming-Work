#include"Entity.h"

SDL_Window* displayWindow;

TextEntity Score;
Entity Background;

void Setup(){
	//Main Setup
	SDL_Init(SDL_INIT_VIDEO);//Initializes SDL
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?
	glViewport(0, 0, 800, 600);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions

	Score.textureLocation="fontspritesheet.png";
	Score.a = 1.0;
	Score.r = 1.0;
	Score.g = 1.0;
	Score.b = 0.0;
	Score.y = 0.85;
	Score.x = -0.35;
	Score.size = 0.1;
	Score.spacing = -0.05;
	Score.text = "Your Score : 0";
	Background.textureLocation = "spacebackground.png";
	Background.height =5;
	Background.width = 5;


};

bool ProcessEvents(SDL_Event& EVENT){
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {//If the Window is closed or the user quits the program, end the loop.
			return(true);
		}

	}
	return false;
};

bool checkCollision(Entity A, Entity B){
	float projTop = A.y + A.height / 2;
	float projBot = A.y - A.height / 2;
	float projRight = A.x + A.width / 2;
	float projLeft = A.x - A.width / 2;

	float cTop = B.y + B.height / 2;
	float cBot = B.y - B.height / 2;
	float cRight = B.x + B.width / 2;
	float cLeft = B.x - B.width / 2;

	if ((projLeft <= cRight && projRight >= cLeft) || (projRight >= cLeft && projLeft <= cRight))
	{
		if (!(projBot > cTop || projTop < cBot))
		{
			//Output for player, when collision occurs.
			return true;
		}
	}
	return false;
};

void Update(float& lastFrameTicks){
	//Fluid Movement is based on fps
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	//Movement programming for Bs
	
};

void Render(){
	//glClearColor(0.0f, 1.0f, 0.0f, 1.0f);//Determines default coloring
	glClear(GL_COLOR_BUFFER_BIT);//Makes background default color
	
	Background.Draw();
	Score.DrawText();
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