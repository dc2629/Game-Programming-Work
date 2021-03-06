#include"App.h"

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
	float retVal = dstMin + ((value - srcMin) / (srcMax - srcMin) * (dstMax - dstMin));
	if (retVal < dstMin) {
		retVal = dstMin;
	}
	if (retVal > dstMax) {
		retVal = dstMax;
	}
	return retVal;
}

float easeOutElastic(float from, float to, float time) {
	float p = 0.3f;
	float s = p / 4.0f;
	float diff = (to - from);
	return from + diff + (diff*pow(2.0f, -10.0f*time) * sin((time - s)*(2 * PI) / p));
}

float easeIn(float from, float to, float time) {
	float tVal = time*time*time*time*time;
	return (1.0f - tVal)*from + tVal*to;
}

float easeOut(float from, float to, float time) {
	float oneMinusT = 1.0f - time;
	float tVal = 1.0f - (oneMinusT * oneMinusT * oneMinusT *
		oneMinusT * oneMinusT);
	return (1.0f - tVal)*from + tVal*to;
}

float easeInOut(float from, float to, float time) {
	float tVal;
	if (time > 0.5) {
		float oneMinusT = 1.0f - ((0.5f - time)*-2.0f);
		tVal = 1.0f - ((oneMinusT * oneMinusT * oneMinusT * oneMinusT *
			oneMinusT) * 0.5f);
	}
	else {
		time *= 2.0;
		tVal = (time*time*time*time*time) / 2.0;
	}
	return (1.0f - tVal)*from + tVal*to;
}

void App::checkCollision(){

	for (int i = 0; i < Entities.size(); i++){
		for (int y = 0; y < Entities.size(); y++){
			if (i != y){
				if (Entities[i]->checkCollision(*Entities[y]) && Entities[y]->checkCollision(*Entities[i])){
					cout << "hello world" << endl;
				}
			}
		}
	}
};

void App::screenShake(){
	screenShakeValue += FIXED_TIMESTEP;
	cout << screenShakeValue << endl;
	glTranslatef(0.0f, sin(screenShakeValue * 25)* 0.05, 0.0f);
	if (screenShakeValue > 1) {
		for (int i = 0; i < 20; i++)
			bullets[i].collideBot = false;
		screenShakeValue = 0.0f;
	}
}

GLuint LoadTexture(const char* image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);

	return textureID;
}

App::App(){
	Init();
}

App::~App(){
	SDL_Quit();
}

//void myglTranslate(float x, float y, Entity& Entity){
//	Vector A, B;
//	A.x = x;
//	A.y = y;
//	A.z = 0;
//	B = Entity.matrix*A;
//	Entity.x = B.x;
//	Entity.y = B.y;
//}

bool App::ProcessEvents(){
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {
			return(true);
		}		////HOW TO DEBUG EVERYTHING !
		//	else if (EVENT.type == SDL_MOUSEMOTION){
		//		float unitX = (((float)EVENT.motion.x / 800.0f)*2.66f) - 1.33f;
		//		float unitY = (((float)(600 - EVENT.motion.y) / 600.0f) * 2.0f) - 1.0f;
		//		player.x = unitX;
		//		player.y = unitY;
		//		/*myglTranslate(unitX, unitY, player);*/
		//	}
		//	else if (EVENT.type == SDL_MOUSEBUTTONDOWN){
		//		//worldToTileCoordinates(player.x, player.y, player.gridX, player.gridY);
		//		checkCollision();

		//		cout << "This is x value: " << player.x << " and y value: " << player.y << endl;
		//		/*cout << "This is the gridX value: " << player.gridX << "and the gridY value: " << player.gridY << endl;*/
		//	}

	}
	return false;
}

void App::Init(){

	SDL_Event EVENT;

	SDL_Init(SDL_INIT_VIDEO);//Initializes SDL
	displayWindow = SDL_CreateWindow("App", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?
	glViewport(0, 0, 800, 600);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions

	timeLeftOver = 0.0f;
	delay = 0.0f;
	SpriteSheetTextureID = LoadTexture("SpaceShooterSprites.png");
	bulletindex = 0;
	animationTime = 0;
	fadeframes = 1.0;
	elapsed = 0;
	screenShakeValue = 0.0f;

	player.textureID = SpriteSheetTextureID;
	player.spriteCountX = 8;
	player.spriteCountY = 8;
	player.index = 12;
	player.set_x = .5;
	player.set_y = .2;
	player.width = .1;
	player.height = .1;
	player.rotation = 0;
	Entities.push_back(&player);

	for (int i = 0; i < 10; i++){
		Ast[i].textureID = SpriteSheetTextureID;
		Ast[i].spriteCountX = 8;
		Ast[i].spriteCountY = 8;
		Ast[i].index = 45;
		float x = .1 + RANDOM_NUMBER / 10;
		Ast[i].height = x;
		Ast[i].width = x;
		Ast[i].set_x = RANDOM_NUMBER * pow(-1, i);
		Ast[i].set_y = RANDOM_NUMBER * pow(-1, i);
		//myglTranslate(.3, .2, Ast[i]);
		Ast[i].rotation = RANDOM_NUMBER * 360;

		Ast[i].velocity_x = RANDOM_NUMBER / 10;
		Ast[i].velocity_y = RANDOM_NUMBER / 10;
		Entities.push_back(&Ast[i]);
	}

	for (int i = 0; i < 20; i++){
		bullets[i].textureID = SpriteSheetTextureID;
		bullets[i].spriteCountX = 8;
		bullets[i].spriteCountY = 8;
		bullets[i].index = 13;
		bullets[i].visible = false;
		bullets[i].height = .05;
		bullets[i].width = .05;

	}
}

void App::fadeIn() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLfloat quad[] = { -2, 2, -2, -2, 2, -2, 2, 2 };
	GLfloat color[] = { 0.0, 0.0, 0.0, fadeframes, 0.0, 0.0, 0.0, fadeframes, 0.0, 0.0, 0.0, fadeframes, 0.0, 0.0, 0.0, fadeframes };

	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, color);
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_COLOR_ARRAY);
}

void App::Render(){
	glClearColor(55.0f / 255.0f, 84.0f / 255.0f, 229.0f / 255.0f, 1.0f);//Determines default coloring
	glClear(GL_COLOR_BUFFER_BIT);//Makes background default color

	fadeIn();

	for (int i = 0; i < Entities.size(); i++){
		Entities[i]->Render();
	}
	for (int i = 0; i < 20; i++){
		bullets[i].Render();
	}

	
	glLoadIdentity();
	for (int y = 0; y < 20; y++){
		if (bullets[y].collideBot)
			screenShake();
	}



	SDL_GL_SwapWindow(displayWindow);
}

void App::FixedUpdate(){

	for (int y = 0; y < 20; y++){
		bullets[y].x += bullets[y].velocity_x*FIXED_TIMESTEP;// Computers too laggy to run it as it's suppose to.
		bullets[y].y += bullets[y].velocity_y*FIXED_TIMESTEP;// Computers too laggy to run it as it's suppose to.
		for (int i = 1; i < Entities.size(); i++){
			if (bullets[y].checkCollision(*Entities[i]) && Entities[i]->checkCollision(bullets[y])){
				if (bullets[y].visible && Entities[i]->visible){
					Entities[i]->visible = false;
					bullets[y].collideBot = true;
					bullets[y].visible = false;
				}
			}
			if (bullets[y].x > 2.0f || bullets[y].x < -2.0f || bullets[y].y >2.0f || bullets[y].y < -2.0f){
				bullets[y].visible = false;
			}
		}
		if (bullets[y].x > 2.0f || bullets[y].y > 2.0f || bullets[y].y < -2.0f || bullets[y].x < -2.0f)
			bullets[y].visible = false;

	
	}

	for (int i = 0; i < Entities.size(); i++){
		if (Entities[i]->visible){
			if (Entities[i]->x > 1.28){
				Entities[i]->x = 1.28;
				Entities[i]->velocity_x = -Entities[i]->velocity_x;
				Entities[i]->collideRight = true;
			}
			if (Entities[i]->x < -1.28){
				Entities[i]->x = -1.28;
				Entities[i]->velocity_x = -Entities[i]->velocity_x;
				Entities[i]->collideLeft = true;
			}
			if (Entities[i]->y > .95){
				Entities[i]->y = .95;
				Entities[i]->velocity_y = -Entities[i]->velocity_y;
				Entities[i]->collideTop = true;
			}
			if (Entities[i]->y < -.95){
				Entities[i]->y = -.95;
				Entities[i]->velocity_y = -Entities[i]->velocity_y;
				Entities[i]->collideBot = true;
			}

			Entities[i]->velocity_y += Entities[i]->acceleration_y*FIXED_TIMESTEP;
			Entities[i]->y += Entities[i]->velocity_y*FIXED_TIMESTEP;

			for (int y = 0; y < Entities.size(); y++){
				if (i != y && Entities[y]->visible){
					if (Entities[i]->checkCollision(*Entities[y]) && Entities[y]->checkCollision(*Entities[i])){
						if (Entities[i]->y > Entities[y]->y){
							Entities[i]->y += .02;
							Entities[y]->y -= .02;
							Entities[i]->collideBot = true;
							Entities[y]->collideTop = true;
						}
						else{
							Entities[y]->y += .02;
							Entities[i]->y -= .02;
							Entities[y]->collideBot = true;
							Entities[i]->collideTop = true;
						}
						Entities[i]->velocity_y = -Entities[i]->velocity_y;
						Entities[y]->velocity_y = -Entities[y]->velocity_y;

					}
				}
			}

			Entities[i]->velocity_x += Entities[i]->acceleration_x*FIXED_TIMESTEP;
			Entities[i]->x += Entities[i]->velocity_x*FIXED_TIMESTEP;

			for (int y = 0; y < Entities.size(); y++){
				if (i != y && Entities[y]->visible){
					if (Entities[i]->checkCollision(*Entities[y]) && Entities[y]->checkCollision(*Entities[i])){
						if (Entities[i]->x > Entities[y]->x){
							Entities[i]->x += .02;
							Entities[y]->x -= .02;
							Entities[i]->collideLeft = true;
							Entities[y]->collideRight = true;
						}
						else{
							Entities[y]->x += .02;
							Entities[i]->x -= .02;
							Entities[y]->collideLeft = true;
							Entities[i]->collideRight = true;
						}
						Entities[i]->velocity_x = -Entities[i]->velocity_x;
						Entities[y]->velocity_x = -Entities[y]->velocity_x;
					}
				}
			}
			Entities[i]->acceleration_x = 0.0f;
			Entities[i]->acceleration_y = 0.0f;
			Entities[i]->resetCollisions();
		}

	}

	keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_RIGHT]){
		player.rotation -= 25*FIXED_TIMESTEP;// Computers too laggy to run it as it's suppose to.
	}
	if (keys[SDL_SCANCODE_LEFT]){
		player.rotation += 25*FIXED_TIMESTEP;// Computers too laggy to run it as it's suppose to.

	}
	if (keys[SDL_SCANCODE_UP]){
		player.acceleration_y = sin(player.rotation / 180 * PI)*3.0f*FIXED_TIMESTEP;// Computers too laggy to run it as it's suppose to.
		player.acceleration_x = cos(player.rotation / 180 * PI)*3.0f*FIXED_TIMESTEP;// Computers too laggy to run it as it's suppose to.
	}


	player.resetCollisions();


	player.velocity_y = lerp(player.velocity_y, 0.0f, FIXED_TIMESTEP*0.5f);
	player.velocity_x = lerp(player.velocity_x, 0.0f, FIXED_TIMESTEP*0.5f);
}

void App::shootbullet(){
	bullets[bulletindex].x = player.x;
	bullets[bulletindex].y = player.y;
	bullets[bulletindex].rotation = player.rotation;
	bullets[bulletindex].velocity_y = sin(bullets[bulletindex].rotation / 180 * PI)*0.5f;
	bullets[bulletindex].velocity_x = cos(bullets[bulletindex].rotation / 180 * PI)*0.5f;
	bullets[bulletindex].visible = true;
	/*Entities.push_back(&bullets[bulletindex]);*/
	bulletindex++;
	if (bulletindex > 19){
		bulletindex = 0;

	}
}

//bool App::shouldRemoveBullet(Entity bullet) {
//	if (bullet.timeAlive > 0.4) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}

void App::Update(){
	timer += elapsed - delay;
	

	if (timer >= .5 && keys[SDL_SCANCODE_SPACE]){
		shootbullet();
		timer = 0.0;
	}
	
	//animationStart = 0.0f;
	//animationEnd = 3000.0f;
	//if (animationTime < 3050.0f){
	//	animationTime += elapsed;
	//	float animationValue = mapValue(animationTime, animationStart, animationEnd, 0.0f, 1.0f);
	//	player.x = easeOut(0.0, 1.0, animationValue) - 1;
	//}
	animationTime = elapsed;//Why is it that everyone uses animationTime+=elapsed?!?!? I tried doing that but the time doesnt map correctly!!!
	float animationValue = mapValue(animationTime, 3, 5, 0.0f, 1.0);
	float animationValue1 = mapValue(animationTime, 0, 2, 0.0, 1.0);
	fadeframes = lerp(1, 0, animationValue1);
	
	if (elapsed <= 5){
		for (int i = 0; i < Entities.size(); i++){
			/*cout << animationValue<<" "<<animationTime <<" "<<elapsed<< endl;*/
			if (i % 4 == 0){
				Entities[i]->x = Entities[i]->set_x;
				Entities[i]->y = easeOutElastic(1.5, Entities[i]->set_y, animationValue);
			}
			else if(i%4 == 1){
				Entities[i]->x = Entities[i]->set_x;
				Entities[i]->y = easeOutElastic(-1.5, Entities[i]->set_y, animationValue);
			}
			else if (i % 4 == 2){
				Entities[i]->y = Entities[i]->set_y;
				Entities[i]->x = easeOutElastic(-1.5, Entities[i]->set_x, animationValue);
			}
			else if (i % 4 == 3){
				Entities[i]->y = Entities[i]->set_y;
				Entities[i]->x = easeOutElastic(1.5, Entities[i]->set_x, animationValue);
			}
		}
	}

	delay = elapsed;

}

void App::UpdateandRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEP) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEP;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}

	timeLeftOver = fixedElapsed;
	Update();

	Render();

}

