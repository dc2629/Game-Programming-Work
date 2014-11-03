#include"App.h"

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
		}
		else if (EVENT.type == SDL_MOUSEBUTTONDOWN){
			worldToTileCoordinates(player.x, player.y, player.gridX, player.gridY);
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
}

void App::Render(){
	glClearColor(55.0f / 255.0f, 84.0f / 255.0f, 229.0f / 255.0f, 1.0f);//Determines default coloring
	glClear(GL_COLOR_BUFFER_BIT);//Makes background default color

	for (int i = 0; i < Entities.size(); i++){
		Entities[i]->Render();
	}
	
	SDL_GL_SwapWindow(displayWindow);
}

bool App::readHeader(ifstream &stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}

	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool App::readLayerData(ifstream &stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}

void App::buildLevel() {
	//change based map
	int SPRITE_COUNT_X = 16;
	int SPRITE_COUNT_Y = 8;

	vector<float> vertexData;
	vector<float> texCoordData;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (levelData[y][x] != 0) {

				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});
				texCoordData.insert(texCoordData.end(), { u + 0.002f, v,
					u + 0.002f, v + (spriteHeight),
					u + spriteWidth - 0.002f, v + (spriteHeight),
					u + spriteWidth - 0.002f, v
				});


			}
		}
	}
	levelvertexData = vertexData;
	leveltexCoordData = texCoordData;
}

void App::worldToTileCoordinates(float X, float Y, int& gridX, int& gridY) {
	gridX = (int)((X + (TILE_SIZE * mapWidth / 2)) / TILE_SIZE);
	gridY = -(int)((Y - (TILE_SIZE * mapHeight / 2)) / TILE_SIZE);
	return;
}

float App::tiletoWorldCoordinatesx(int gridX)
{
	float X = (gridX)*TILE_SIZE - (TILE_SIZE * mapWidth / 2);
	return(X);
}

float App::tiletoWorldCoordinatesy(int gridY)
{
	float Y = ((-gridY)*TILE_SIZE) + (TILE_SIZE * mapHeight / 2);
	return(Y);
}

bool App::TileCollisonX(Entity &entity){
	worldToTileCoordinates(entity.x, entity.y, entity.gridX, entity.gridY);
	float widthhalf = entity.width / 2;
	float heighthalf = entity.height / 2;
	float left = entity.x - widthhalf;
	float right = entity.x + widthhalf;
	if (entity.gridY < mapHeight&&entity.gridX < mapWidth){
		worldToTileCoordinates(left, entity.y, entity.gridX, entity.gridY);
		if (levelData[entity.gridY][entity.gridX] != 0){
			float x = tiletoWorldCoordinatesx(entity.gridX + 1);
			entity.x += x - left;
			entity.velocity_x = 0;
			entity.collideLeft = true;
			return true;
		}
		worldToTileCoordinates(right, entity.y, entity.gridX, entity.gridY);
		if (levelData[entity.gridY][entity.gridX] != 0){
			float x = tiletoWorldCoordinatesx(entity.gridX);
			entity.x += x - right;
			entity.velocity_x = 0;
			entity.collideRight = true;
			return true;
		}
	}


	return false;
}

bool App::TileCollisonY(Entity &entity){
	worldToTileCoordinates(entity.x, entity.y, entity.gridX, entity.gridY);
	float widthhalf = entity.width / 2;
	float heighthalf = entity.height / 2;
	float bot = entity.y - heighthalf;
	float top = entity.y + heighthalf;
	if (entity.gridY < mapHeight&&entity.gridX < mapWidth){
		worldToTileCoordinates(entity.x, bot, entity.gridX, entity.gridY);
		if (levelData[entity.gridY][entity.gridX] != 0){
			float y_pen = tiletoWorldCoordinatesy(entity.gridY + .5*TILE_SIZE);
			entity.y += y_pen - bot;
			entity.velocity_y = 0.0f;
			entity.collideBot = true;
			return true;
		}

		worldToTileCoordinates(entity.x, top, entity.gridX, entity.gridY);
		if (levelData[entity.gridY][entity.gridX] != 0){
			float y_pen = tiletoWorldCoordinatesy(entity.gridY - TILE_SIZE);
			entity.y -= y_pen - top;
			entity.velocity_y = 0.0f;
			entity.collideTop = true;
			return true;
		}
	}


	return false;
}
