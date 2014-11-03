#include"App.h"

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

void App::checkCollision(){

	for (int i = 0; i < Entities.size(); i++){
		for (int y = 0; y < Entities.size(); y++){
			if (i != y){
				if (Entities[i]->checkCollision(*Entities[y]) && Entities[y]->checkCollision(*Entities[i])){
					cout << "Hello World" << endl;
				}
			}
		}
	}
};

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
			Vector A, B;
			A.x = unitX;
			A.y = unitY;
			A.z = 0;
			B = player.matrix*A;
			player.x = B.x;
			player.y = B.y;
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

	float timeLeftOver = 0.0f;

	SpriteSheetTextureID = LoadTexture("SpriteSheet.png");

	player.textureID = SpriteSheetTextureID;
	player.spriteCountX = 16;
	player.spriteCountY = 8;
	player.index = 6;
	player.x = .5;
	player.y = .2;
	player.width = .1;
	player.height = .1;	
	player.rotation = 0;
	Entities.push_back(&player);

	//for (int i = 0; i < 5; i++){

	int i = 0;
		Ast[i].textureID = SpriteSheetTextureID;
		Ast[i].spriteCountX = 16;
		Ast[i].spriteCountY = 8;
		Ast[i].index = 6;
		Ast[i].height = .1;
		Ast[i].width = .1;
		Ast[i].x = 0;
		Ast[i].y = 0;
		Ast[i].rotation = 0;

		Ast[i].velocity_x = RANDOM_NUMBER / 10;
		Ast[i].velocity_y = RANDOM_NUMBER / 10;
		Entities.push_back(&Ast[i]);
	/*}*/


}

void App::Render(){
	glClearColor(55.0f / 255.0f, 84.0f / 255.0f, 229.0f / 255.0f, 1.0f);//Determines default coloring
	glClear(GL_COLOR_BUFFER_BIT);//Makes background default color

	for (int i = 0; i < Entities.size(); i++){
		Entities[i]->Render();
	}
	
	SDL_GL_SwapWindow(displayWindow);
}

void App::FixedUpdate(){
	player.velocity_y += player.acceleration_y*FIXED_TIMESTEP;
	player.y += player.velocity_y*FIXED_TIMESTEP;


	player.velocity_x += player.acceleration_x*FIXED_TIMESTEP;
	player.x += player.velocity_x*FIXED_TIMESTEP;

	player.acceleration_x = 0.0f;

	player.resetCollisions();


	player.velocity_y = lerp(player.velocity_y, 0.0f, FIXED_TIMESTEP*0.5f);
	player.velocity_x = lerp(player.velocity_x, 0.0f, FIXED_TIMESTEP*0.5f);
}

void App::UpdateandRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEP) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEP;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	//Update(elapsed);
	Render();
}

