#include"Entity.h"
#include<iostream>
#include<algorithm>

SDL_Window* displayWindow;

TextEntity Score;
float elapsed, accumulator;
Entity Background;
SSEntity pShip, pShipBullet, AIShip, AIBullet;
vector<SSEntity> bullets, abullets;

void Setup(){
	//Main Setup
	SDL_Init(SDL_INIT_VIDEO);//Initializes SDL
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?
	glViewport(0, 0, 800, 600);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions

	//Background
	Background.textureLocation = "spacebackground.png";
	Background.height = 5;
	Background.width = 5;

	//Score/Health
	Score.textureLocation = "fontspritesheet.png";
	Score.a = 1.0;
	Score.r = 1.0;
	Score.g = 1.0;
	Score.b = 0.0;
	Score.y = -.95;
	Score.x = 1.3;
	Score.size = .1;
	Score.spacing = -0.05;


	//Player's Ship Details
	pShip.textureLocation = "SpaceShooterSprites.png";
	pShip.spriteCountX = 8;
	pShip.spriteCountY = 8;
	pShip.index = 19;
	pShip.rotation = -90.0f;
	pShip.height = .1;
	pShip.width = .1;
	pShip.x = 0;
	pShip.y = -0.9;
	pShip.health = 3;
	//Player's Bullet Details
	pShipBullet.textureLocation = "SpaceShooterSprites.png";
	pShipBullet.rotation = -90.0f;
	pShipBullet.height = .15;
	pShipBullet.width = .1;
	pShipBullet.spriteCountX = 8;
	pShipBullet.spriteCountY = 8;
	pShipBullet.index = 32;
	pShipBullet.speed = 2;
	Score.text = to_string(pShip.health);
	//AI Ship Details
	AIShip.textureLocation = "SpaceShooterSprites.png";
	AIShip.rotation = 90.0f;
	AIShip.height = .5;
	AIShip.width = .5;
	AIShip.spriteCountX = 8;
	AIShip.spriteCountY = 8;
	AIShip.index = 20;
	AIShip.x = 0;
	AIShip.y = .7;
	AIShip.speed = .6;
	AIShip.health = 10;
	AIShip.direction_x = 1;
	//AI Bullets
	AIBullet.textureLocation = "SpaceShooterSprites.png";
	AIBullet.rotation = 90.0f;
	AIBullet.height = 0.25;
	AIBullet.width = 0.15;
	AIBullet.spriteCountX = 8;
	AIBullet.spriteCountY = 8;
	AIBullet.index = 40;
	AIBullet.speed = -1.2;


	accumulator = 0.0;




	elapsed = 0;

};

bool shouldRemoveBullet(SSEntity bullet) {
	if (bullet.timeAlive > 3) {
		return true;
	}
	else {
		return false;
	}
}

void shootplayerBullet() {
	pShipBullet.visible = true;
	pShipBullet.x = pShip.x - .015;
	pShipBullet.y = pShip.y + 0.05;
	bullets.push_back(pShipBullet);
	pShipBullet.visible = false;
}
void shootAIBullet(){
	AIBullet.visible = true;
	AIBullet.x = AIShip.x;
	AIBullet.y = AIShip.y;
	abullets.push_back(AIBullet);
	AIBullet.visible = false;
}

bool ProcessEvents(SDL_Event& EVENT){

	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {//If the Window is closed or the user quits the program, end the loop.
			return(true);
		}
		else if (EVENT.type == SDL_MOUSEMOTION){
			float unitX = (((float)EVENT.motion.x / 800.0f)*2.66f) - 1.33f;
			pShip.x = unitX;
		}
		else if (EVENT.type == SDL_MOUSEBUTTONDOWN){
			shootplayerBullet();

		}
	}
	return false;
};

bool checkCollision(SSEntity A, SSEntity B){
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
			//Output for pShip, when collision occurs.
			return true;
		}
	}
	return false;
};



void Update(float& lastFrameTicks){
	//Fluid Movement is based on fps
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	accumulator += elapsed*1000;
	//Movement programming
	//const Uint8 *keys = SDL_GetKeyboardState(NULL);
	float x = (float)(AIShip.health*100);

	for (int i = 0; i < bullets.size(); i++){
		bullets[i].y += bullets[i].speed*elapsed;
		bullets[i].timeAlive += elapsed;
	}
	for (int i = 0; i < abullets.size(); i++){
		abullets[i].y += abullets[i].speed*elapsed;
		abullets[i].timeAlive += elapsed;
	}

	if (AIShip.x > 1.3){
		AIShip.direction_x = -1;
		AIShip.x = 1.28;
	}
	else if (AIShip.x < -1.3){
		AIShip.direction_x = 1;
		AIShip.x = -1.28;
	}
	AIShip.x += AIShip.speed*elapsed*AIShip.direction_x;
	while ( (accumulator > x)){
		shootAIBullet();
		accumulator = 0;
	}

};

void Render(){
	//glClearColor(0.0f, 1.0f, 0.0f, 1.0f);//Determines default coloring
	glClear(GL_COLOR_BUFFER_BIT);//Makes background default color

	Background.Draw();
	Score.DrawText();
	pShip.DrawSpriteSheetSprite();
	for (int i = 0; i < abullets.size(); i++){
		if (abullets[i].visible){
			abullets[i].DrawSpriteSheetSprite();
			if (checkCollision(abullets[i], pShip)){

				pShip.health--;
				Score.text = to_string(pShip.health);
				abullets[i].visible = false;
			}

		}


	}
	for (int i = 0; i < bullets.size(); i++){
		if (bullets[i].visible){
			bullets[i].DrawSpriteSheetSprite();
			if (checkCollision(bullets[i], AIShip)){

				AIShip.health--;
				bullets[i].visible = false;
			}

		}
	}
	bullets.erase(remove_if(bullets.begin(), bullets.end(), shouldRemoveBullet), bullets.end());

	AIShip.DrawSpriteSheetSprite();

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