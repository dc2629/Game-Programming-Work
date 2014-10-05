#include"Platformer.h"

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

void DemoApp::FixedUpdate(){


	//for (int i = 0; i < MAX_ENEMIES; i++){
	//	if (enemies[i].collideRight){
	//		enemies[i].acceleration_x = -3.0f;
	//	}
	//	if (enemies[i].collideLeft){
	//		enemies[i].acceleration_x = 3.0f;
	//	}
	//	if (enemies[i].collideTop){
	//		enemies[i].enableCollisions = false;
	//	}
	//}
}

bool DemoApp::ProcessEvents(){
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {
			return(true);
		}

	}
	return false;
}

void DemoApp::Setup(){
	State = STATE_GAME_LEVEL;

	//Player's Ship Details
	genEnemy.textureID = SpriteSheetTextureID;
	genEnemy.spriteCountX = 8;
	genEnemy.spriteCountY = 8;
	genEnemy.index = 19;
	genEnemy.rotation = -90.0f;
	genEnemy.height = .1;
	genEnemy.width = .1;
	genEnemy.x = 0;
	genEnemy.y = -0.9;
	genEnemy.health = 3;







	//genEnemy.textureID = SpriteSheetTextureID;
	//genEnemy.width = 0.4f;
	//genEnemy.health = 0.4f;
	//genEnemy.spriteCountX = 16;
	//genEnemy.spriteCountY = 8;
	//genEnemy.index = 80;
	//genEnemy.visible = 1;
	//cout << genEnemy.textureID << endl;
	////for (int i = 0; i < MAX_ENEMIES; i++){

	////	genEnemy.x = 0+i/10;
	////	genEnemy.y = 0+i/10;
	////	cout << genEnemy.textureID << endl;
	////	enemies.push_back(genEnemy);
	////}
}

DemoApp::DemoApp(){
	SDL_Event EVENT;

	SDL_Init(SDL_INIT_VIDEO);//Initializes SDL
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?
	glViewport(0, 0, 800, 600);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions

	SpriteSheetTextureID = LoadTexture("SpriteSheet.png");

	glMatrixMode(GL_MODELVIEW);
	SDL_GL_SwapWindow(displayWindow);
}

DemoApp::~DemoApp(){
	SDL_Quit();
}

void DemoApp::GameRender(){
	genEnemy.Draw();
	//for (int i = 0; i < MAX_ENEMIES; i++){
	//	enemies[i].Draw();
	//	//cout << "Why is it not drawing?" << endl;
	//}
}

void DemoApp::Render(){
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);//Determines default coloring

	glClear(GL_COLOR_BUFFER_BIT);
	genEnemy.Draw();
	cout << genEnemy.textureID << endl;
	//if (State == STATE_GAME_LEVEL){
	//	GameRender();
	//}



	//
}

void DemoApp::UpdateandRender(){
	FixedUpdate();
	Render();
}