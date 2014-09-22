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
	yarn.x = 0;
	yarn.y = 0;
	yarn.height = 0.15f;
	yarn.width = 0.15f;
	yarn.speed = 0.85f;
	yarn.angle = (float)(rand() % 360);
	yarn.direction_x = (float)(cos(yarn.angle));
	yarn.direction_y = (float)(sin(yarn.angle));

};

bool ProcessEvents(SDL_Event& EVENT){
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {//If the Window is closed or the user quits the program, end the loop.
			return(true);
		}

	}
	return false;
};

bool checkCollision(Entity yarn, Entity cat){
	float projTop = yarn.y + yarn.height / 2;
	float projBot = yarn.y - yarn.height / 2;
	float projRight = yarn.x + yarn.width / 2;
	float projLeft = yarn.x - yarn.width / 2;

	float cTop = cat.y + cat.height / 2;
	float cBot = cat.y - cat.height / 2;
	float cRight = cat.x + cat.width / 2;
	float cLeft = cat.x - cat.width / 2;

	if ((projLeft <= cRight && projRight >= cLeft) || (projRight >= cLeft && projLeft <= cRight))
	{
		if (!(projBot > cTop || projTop < cBot))
		{
			cout << "Collision Occurred" << endl;
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
	//Movement programming for cats
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_UP]){
		if (Rcat.y<.9f)
			Rcat.y += elapsed * 0.8f;
	}
	else if (keys[SDL_SCANCODE_DOWN]){
		if (Rcat.y>-.9f)
			Rcat.y -= elapsed * 0.8f;
	}
	if (keys[SDL_SCANCODE_W]){
		if (Lcat.y<.9f)
			Lcat.y += elapsed * 0.8f;

	}
	else if (keys[SDL_SCANCODE_S]){
		if (Lcat.y>-.9f)
			Lcat.y -= elapsed * 0.8f;
	}

	if (yarn.y > .9)
		yarn.direction_y = -yarn.direction_y;
	if (yarn.y < -.9)
		yarn.direction_y = -yarn.direction_y;
	if (yarn.x > 1.6){
		yarn.x = 0, yarn.y = 0;
		yarn.angle = (float)(rand() % 360);
		yarn.direction_x = (float)(cos(yarn.angle));
		yarn.direction_y = (float)(sin(yarn.angle));
		cout << "Left Side Wins" << endl;

	}
	else if (yarn.x < -1.6){
		yarn.x = 0, yarn.y = 0;
		yarn.angle = (float)(rand() % 360);
		yarn.direction_x = (float)(cos(yarn.angle));
		yarn.direction_y = (float)(sin(yarn.angle));
		cout << "Right Side Wins!" << endl;

	}

	if (checkCollision(yarn, Rcat)){
		yarn.direction_x = -yarn.direction_x;
	}

	if (checkCollision(yarn, Lcat)){
		yarn.direction_x = -yarn.direction_x;
	}

	yarn.x += yarn.direction_x*yarn.speed*elapsed;
	yarn.y += yarn.direction_y*yarn.speed*elapsed;
};

void Render(){
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);//Determines default coloring
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