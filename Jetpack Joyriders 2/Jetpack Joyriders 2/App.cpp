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

bool App::ProcessEvents(){
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {
			return(true);
		}		////HOW TO DEBUG EVERYTHING !
		//else if (EVENT.type == SDL_MOUSEMOTION){
		//	float unitX = (((float)EVENT.motion.x / 800.0f)*2.66f) - 1.33f;
		//	float unitY = (((float)(600 - EVENT.motion.y) / 600.0f) * 2.0f) - 1.0f;
		//	player.x = unitX;
		//	player.y = unitY;
		//	/*myglTranslate(unitX, unitY, player);*/
		//}
		//else if (EVENT.type == SDL_MOUSEBUTTONDOWN){
		//	//worldToTileCoordinates(player.x, player.y, player.gridX, player.gridY);
		//	checkCollision();

		//	cout << "This is x value: " << player.x << " and y value: " << player.y << endl;
		//	/*cout << "This is the gridX value: " << player.gridX << "and the gridY value: " << player.gridY << endl;*/
		//}

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
	SpriteSheetTextureID = LoadTexture("SpriteSheet.png");
	timer = 0.0f;
	timer2 = 0.0f;
	//SpriteSheetTextureID = NULL;
	animationTime = 0;
	fadeframes = 1.0;
	elapsed = 0;
	screenShakeValue = 0.0f;
	gravity_y = -.0075f;
	//Player
	player.textureID = LoadTexture("characters_3.png");
	player.spriteCountX = 8;
	player.spriteCountY = 4;
	player.index = 8;
	player.x = -.85f;
	player.y = -.6;
	player.width = .2;
	player.height = .2;
	player.rotation = 0;
	Entities.push_back(&player);
	//Floor
	for (int i = 0; i < 0; i++){
		Ast[i].textureID = SpriteSheetTextureID;
		Ast[i].spriteCountX = 16;
		Ast[i].spriteCountY = 8;
		Ast[i].index = 16 + RANDOM_NUMBER * 3;
		Ast[i].height = .1;
		Ast[i].width = .1;
		Ast[i].x = ((float)i) / 9.9 - 1.67;
		Ast[i].y = -.95;
		Ast[i].rotation = 0;
		Ast[i].velocity_x = -0.005;
		Ast[i].velocity_y = 0;
		floor.push_back(&Ast[i]);
	}
	//Player animation stuff
	for (int i = 0; i < 4; i++){
		paIndex1[i] = i + 8;
	}
	numFrames = 4;
	currentindex = 0;
	//Particle Emitter Stuff
	ParticleEmitter temp(25);
	playerParticles = temp;
	playerParticles.position.x = -.85f;
	//Wandering background stuff
	for (int i = 0; i < 4; i++){
		Snakes[i].textureID = LoadTexture("characters_3.png");
		Snakes[i].spriteCountX = 8;
		Snakes[i].spriteCountY = 4;
		Snakes[i].index = 24;
		Snakes[i].height = .15;
		Snakes[i].width = .15;
		Snakes[i].y = -.825;
		Snakes[i].x = 5;
		Snakes[i].rotation = 0;
		Snakes[i].velocity_x = -0.005 + (-.003*RANDOM_NUMBER);
		Snakes[i].velocity_y = .05;
		if (i % 2 == 0){
			Snakes[i].scale_x = -1;
		}
		else{
			Snakes[i].scale_x = 1;
		}
		Entities.push_back(&Snakes[i]);
	}
	//Background animation for Snakes
	for (int i = 0; i < 4; i++){
		saIndex[i] = i + 24;
	}
	snakescurrentindex = 0;
	//Bullets
	totalbullets = 8;
	SpriteSheetTextureID = LoadTexture("Arrows.png");
	for (int i = 0; i < totalbullets; i++){
		bullets[i].textureID = SpriteSheetTextureID;
		bullets[i].spriteCountX = 6;
		bullets[i].spriteCountY = 1;
		bullets[i].index = 0;
		bullets[i].height = .15;
		bullets[i].width = .15;
		bullets[i].x = -1.5;
		bullets[i].velocity_x = -0.04;
		bullets[i].set_x = -RANDOM_NUMBER * 100;
		bullets[i].acceleration_x = 0;
		bullets[i].rotation = 0;
		bullets[i].visible = false;
		Entities.push_back(&bullets[i]);
	}
	SpriteSheetTextureID = LoadTexture("Signal.png");
	for (int i = 0; i < totalbullets; i++){
		bulletindicators[i].textureID = SpriteSheetTextureID;
		bulletindicators[i].spriteCountX = 2;
		bulletindicators[i].spriteCountY = 1;
		bulletindicators[i].index = 1;
		bulletindicators[i].height = .075;
		bulletindicators[i].width = .075;
		bulletindicators[i].set_y = -1+2*RANDOM_NUMBER;
		bulletindicators[i].x = 1.27;
		bulletindicators[i].rotation = 0;
		bulletindicators[i].collideTop = false;
		bulletmech.push_back(&bulletindicators[i]);
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

	//fadeIn();


	if (!player.collideBot){
		playerParticles.Render();
	}

	for (int i = 0; i < bulletmech.size(); i++){
		bulletmech[i]->Render();
	}

	for (int i = 0; i < Entities.size(); i++){
		Entities[i]->Render();
	}
	for (int i = 0; i < floor.size(); i++){
		floor[i]->Render();
	}

	SDL_GL_SwapWindow(displayWindow);
}

void App::FixedUpdate(){
	for (int i = 0; i < floor.size(); i++){
		if (floor[i]->x < -2){
			floor[i]->x = 2.04;
		}
		floor[i]->x += floor[i]->velocity_x*FIXED_TIMESTEP;
	}

	for (int i = 1; i < Entities.size(); i++){
		Entities[i]->velocity_x += Entities[i]->acceleration_x*FIXED_TIMESTEP;
		Entities[i]->x += Entities[i]->velocity_x*FIXED_TIMESTEP;
		if (Entities[i]->checkCollision(player) && player.checkCollision(*Entities[i])){
			Entities[i]->collideLeft = true;
		}
	}
	for (int i = 0; i < 8; i++){
		if (player.checkCollision(bullets[i]) && bullets[i].checkCollision(player)){
			player.collideLeft = true;
		}
	}

	player.velocity_y += player.acceleration_y*FIXED_TIMESTEP;
	player.velocity_y += gravity_y*FIXED_TIMESTEP;
	player.y += player.velocity_y*FIXED_TIMESTEP;
	player.acceleration_y = 0;
	if (player.y < -.8){
		player.y = -.8;
		player.collideBot = true;
		player.velocity_y = 0;
	}
	if (player.y > .95){
		player.y = .95;
		player.collideTop = true;
		player.velocity_y = 0;
	}
	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_UP]){
		player.acceleration_y = 0.75f*FIXED_TIMESTEP;// Computers too laggy to run it as it's suppose to.
	}
	if (player.collideLeft){
		//cout << "you're dead!" << endl;
	}


	player.velocity_y = lerp(player.velocity_y, 0.0f, FIXED_TIMESTEP*0.5f);
}

void App::Update(){
	actualElapsed = elapsed - delay;
	timer += actualElapsed;
	timer2 += actualElapsed;
	//Walking animation
	if (player.collideBot){
		if (timer > .2) {
			currentindex++;
			timer = 0.0;
			if (currentindex > numFrames - 1) {
				currentindex = 0;
			}
		}
		player.index = paIndex1[currentindex];
	}
	else{
		player.index = 13;
		playerParticles.position.y = player.y - .1f;

	}
	//Snake movement, death, and respawn.
	for (int i = 0; i < 6; i++){
		if ((Snakes[i].x - player.x) < .6 && player.collideBot){
			Snakes[i].velocity_x = -.003 - (0.001*actualElapsed);
			Snakes[i].scale_x = 1;
		}
		if (timer2 > .2) {
			snakescurrentindex++;
			timer2 = 0.0;
			if (snakescurrentindex > numFrames - 1) {
				snakescurrentindex = 0;
			}
		}
		Snakes[i].index = saIndex[snakescurrentindex];

		if (Snakes[i].x < -1.5 || (Snakes[i].y < -1.2)){
			Snakes[i].x = 4 + RANDOM_NUMBER;
			Snakes[i].y = -.825f;
			Snakes[i].rotation = 0;
			Snakes[i].velocity_y = .05;
			if (RANDOM_NUMBER * 2 > 1){
				Snakes[i].scale_x = -1;
				Snakes[i].velocity_x = -0.005 + (-.003*RANDOM_NUMBER);
			}
			else
			{
				Snakes[i].velocity_x = -0.005 + (-.001*RANDOM_NUMBER);
			}
			Snakes[i].resetCollisions();
			Snakes[i].scale_y = 1;
		}
		if (Snakes[i].collideLeft){
			Snakes[i].velocity_x = .002;
			Snakes[i].y += Snakes[i].velocity_y*FIXED_TIMESTEP;
			Snakes[i].velocity_y += gravity_y*FIXED_TIMESTEP;
			Snakes[i].rotation += 10 * FIXED_TIMESTEP;
		}
	}
	//Speed increase
	for (int i = 1; i < Entities.size(); i++){
		Entities[i]->velocity_x += (-.001*actualElapsed);
	}
	for (int i = 0; i < floor.size(); i++){
		floor[i]->velocity_x += (-.0005*actualElapsed);
	}
	//Particles
	playerParticles.Update(timeLeftOver);
	//Bullets
	for (int i = 0; i < totalbullets; i++){
		if (bulletindicators[i].collideTop){
			bullettimers[i] += actualElapsed;
			if (bullettimers[i] <= 2.5){
				float animationValue = mapValue(bullettimers[i], 0, 1750, 0.0f, 1.0);
				bulletindicators[i].y = bulletindicators[i].set_y;
				bulletindicators[i].set_y = lerp(bulletindicators[i].y, player.y, animationValue);
			}
			else
			{
				bullettimers[i] = 0;
				bullets[i].x = 2;
				bullets[i].y = bulletindicators[i].set_y;
				bulletindicators[i].collideTop = false;
				bullets[i].velocity_x = -0.04;
				bullets[i].visible = true;
				bulletindicators[i].index = 1;
			}
		}
		if (bullets[i].x < bullets[i].set_x){
			bulletindicators[i].collideTop = true;
		}
	}
	//Added Bullet Animations
	for (int i = 0; i < totalbullets; i++){
		
		
		//Stupid fix for unknown issue.
		if (!bulletindicators[i].collideTop){
			bulletindicators[i].visible = false;
		}
		else{
			bulletindicators[i].visible = true;
		}
		//Animations
		if (bullets[i].visible){
			bulletanimationtimers[i] += actualElapsed;
			if (bulletanimationtimers[i] > .035) {
				bulletindex[i]++;
				bulletanimationtimers[i] = 0.0;
			}
			if (bulletindex[i] > 5){
				bulletindex[i] = 0;
			}
			bullets[i].index = bulletindex[i];
		}
		if (bulletindicators[i].collideTop){
			indanimationtimer[i] += actualElapsed;
			if (indanimationtimer[i] > 2.3) {
				bulletindicators[i].index = 0;
				indanimationtimer[i] = 0.0;
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

	player.resetCollisions();
}

