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
		else if (EVENT.type == SDL_MOUSEMOTION){
			float unitX = (((float)EVENT.motion.x / 800.0f)*2.66f) - 1.33f;
			float unitY = (((float)(600 - EVENT.motion.y) / 600.0f) * 2.0f) - 1.0f;
			player.x = unitX;
			player.y = unitY;
			/*myglTranslate(unitX, unitY, player);*/
		}
		else if (EVENT.type == SDL_MOUSEBUTTONDOWN){
			//worldToTileCoordinates(player.x, player.y, player.gridX, player.gridY);
			checkCollision();

			cout << "This is x value: " << player.x << " and y value: " << player.y << endl;
			/*cout << "This is the gridX value: " << player.gridX << "and the gridY value: " << player.gridY << endl;*/
		}

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
	//SpriteSheetTextureID = NULL;
	bulletindex = 0;
	animationTime = 0;
	fadeframes = 1.0;
	elapsed = 0;
	screenShakeValue = 0.0f;
	gravity_y = -.5;

	player.textureID = SpriteSheetTextureID;
	player.spriteCountX = 16;
	player.spriteCountY = 8;
	player.index = 80;
	player.x = -.85f;
	player.y = .2;
	player.width = .1;
	player.height = .1;
	player.rotation = 0;
	Entities.push_back(&player);

	for (int i = 0; i < 40; i++){
		Ast[i].textureID = SpriteSheetTextureID;
		Ast[i].spriteCountX = 16;
		Ast[i].spriteCountY = 8;
		Ast[i].index = 16+RANDOM_NUMBER*3;
		Ast[i].height = .1;
		Ast[i].width = .1;
		Ast[i].x = ((float)i) / 9.9 - 1.67;
		Ast[i].y = -.95;
		Ast[i].rotation = 0;
		Ast[i].velocity_x = -0.01;
		Ast[i].velocity_y = 0;
		floor.push_back(&Ast[i]);
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
			floor[i]->x = 2;
		}
		floor[i]->x += floor[i]->velocity_x*FIXED_TIMESTEP;
	}

	//player.velocity_y += player.acceleration_y*FIXED_TIMESTEP;
	//player.velocity_y += gravity_y*FIXED_TIMESTEP;
	//player.y += player.velocity_y*FIXED_TIMESTEP;


}

void App::Update(){

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

