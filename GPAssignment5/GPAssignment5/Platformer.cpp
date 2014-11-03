#include "Platformer.h"


float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

void Entity::resetCollisions(){
	collideBot = false;
	collideLeft = false;
	collideRight = false;
	collideTop = false;
}

void TextEntity::DrawText() {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//Resets all sprites.
	glTranslatef(x, y, 0.0);//move sprites across the window.

	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size, ((size + spacing) * i) + (-0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), 0.5f* size });
		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x + texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Entity::Draw() {
	if (visible){
		glEnable(GL_TEXTURE_2D);//enable or disable server-side GL capabilities, in this case enables 2d textures.
		glBindTexture(GL_TEXTURE_2D, textureID);//binds texture to target. Binds an image to the texture map.
		glMatrixMode(GL_MODELVIEW);//Modelview matrix determines location and angle of the sprites.
		glPushMatrix();
		glTranslatef(x, y, 0.0);//move sprites across the window.
		glRotatef(rotation, 0.0, 0.0, 1.0);//rotations on the z-view.
		if (flipX){
			glScalef(-1, 1, 1);
		}

		float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
		float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
		float spriteWidth = 1.0f / (float)spriteCountX;
		float spriteHeight = 1.0f / (float)spriteCountY;

		GLfloat quadUVs[] = { u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v };

		GLfloat quad[] = { -width / 2, height / 2, -width / 2, -height / 2, width / 2, -height / 2, width / 2, height / 2 };//Defines a quad to place the image. REMEMBER COUNTER CLOCKWISE LISTING OF VERTICES
		glVertexPointer(2, GL_FLOAT, 0, quad);//Read 2, FLOAT VALUES, SKIP 0 values in case we put colors in the matrix, and quad is the pointer to the array.
		glEnableClientState(GL_VERTEX_ARRAY);//allows for server to access the vertex arrays and for clients to draw the arrays.
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);//Defines an array of texture coordinates 
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_BLEND);//Enable blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Alpha blending, basically removing the background of the quad.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glDrawArrays(GL_QUADS, 0, 4);//Drawing quads, starting from 0, and draw 4 vertices. 
		glDisable(GL_TEXTURE_2D);//Disable the texture since OpenGl won't use the same texture when redrawing other quads.
		glDisable(GL_BLEND);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glPopMatrix();
	}
}

bool Entity::checkCollision(Entity A){
	float projTop = A.y + A.height / 2;
	float projBot = A.y - A.height / 2;
	float projRight = A.x + A.width / 2;
	float projLeft = A.x - A.width / 2;

	float Top = y + height / 2;
	float Bot = y - height / 2;
	float Right = x + width / 2;
	float Left = x - width / 2;

	if ((projLeft <= Right && projRight >= Left) || (projRight >= Left && projLeft <= Right))
	{
		if (!(projBot >= Top || projTop <= Bot))
		{
			return true;
		}
	}
	return false;
};

float Entity::CalculateY_Pen(Entity A){
	float projTop = A.y + A.height / 2;
	float projBot = A.y - A.height / 2;
	float projRight = A.x + A.width / 2;
	float projLeft = A.x - A.width / 2;

	float Top = y + height / 2;
	float Bot = y - height / 2;
	float Right = x + width / 2;
	float Left = x - width / 2;

	float y_distance = fabs(y - A.y);
	float y_pen = fabs(y_distance - A.height / 2 - height / 2);

	return (y_pen);


}

float Entity::CalculateX_Pen(Entity A){
	float projTop = A.y + A.height / 2;
	float projBot = A.y - A.height / 2;
	float projRight = A.x + A.width / 2;
	float projLeft = A.x - A.width / 2;

	float Top = y + height / 2;
	float Bot = y - height / 2;
	float Right = x + width / 2;
	float Left = x - width / 2;

	float x_distance = fabs(x - A.x);
	float x_pen = fabs(x_distance - A.width / 2 - width / 2);

	return (x_pen);




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

bool DemoApp::ProcessEvents(){
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {
			return(true);
		}
		////HOW TO DEBUG EVERYTHING !
		//else if (EVENT.type == SDL_MOUSEMOTION){
		//	float unitX = (((float)EVENT.motion.x / 800.0f)*2.66f) - 1.33f;
		//	float unitY = (((float)(600 - EVENT.motion.y) / 600.0f) * 2.0f) - 1.0f;
		//	player.x = unitX;
		//	player.y = unitY;
		//}
		//else if (EVENT.type == SDL_MOUSEBUTTONDOWN){
		//	worldToTileCoordinates(player.x, player.y, player.gridX, player.gridY);
		//	cout << "This is x value: " << player.x << " and y value: " << player.y << endl;
		//	cout << "This is the grid_X value: " << player.gridX << "and the grid_Y value: " << player.gridY << endl;
		//	TileCollisonY(player);
		//	TileCollisonX(player);
		//}
		else if (EVENT.type == SDL_KEYDOWN){
			if (EVENT.key.keysym.scancode == SDL_SCANCODE_SPACE){
				if (bullet.visible == false){
					bullet.x = player.x;
					bullet.y = player.y;
					bullet.flipX = player.flipX;
					bullet.visible = true;
					Mix_PlayChannel(-1, gunsblazing, 0);
				}
			}
		}
	}
	return false;
}

void DemoApp::Init(){
	State = STATE_GAME_LEVEL;
	SDL_Event EVENT;

	SDL_Init(SDL_INIT_VIDEO);//Initializes SDL
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?
	glViewport(0, 0, 800, 600);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions

	//Initialize music
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	music = Mix_LoadMUS("vivy.mp3");
	Mix_PlayMusic(music, -1);
	gunsblazing = Mix_LoadWAV("raygun.wav");
	mariojump = Mix_LoadWAV("jump.wav");



	lastFrameTicks = 0.0f;
	gravity_y = -0.1f;
	timeLeftOver = 0.0f;
	TILE_SIZE = 0.06f;

	glMatrixMode(GL_MODELVIEW);
	SpriteSheetTextureID = LoadTexture("SpriteSheet.png");
	mapWidth = -1;
	mapHeight = -1;
	ifstream infile("myfirstmap.txt");
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}

	}

	player.textureID = SpriteSheetTextureID;
	player.spriteCountX = 16;
	player.spriteCountY = 8;
	player.index = 6;
	player.visible = true;
	player.enableCollisions = true;
	player.health = 3;
	player.height = 0.1f;
	player.width = 0.1f;
	player.x = 0.0f;
	player.y = 0.0f;

	bullet.textureID = SpriteSheetTextureID;
	bullet.height = .15f;
	bullet.width = .5f;
	bullet.spriteCountX = 16;
	bullet.spriteCountY = 8;
	bullet.index = 24;
	bullet.velocity_x = 1.0f;
	bullet.visible = false;
}

bool DemoApp::readHeader(ifstream &stream) {
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

bool DemoApp::readLayerData(ifstream &stream) {
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

void DemoApp::buildLevel() {

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

DemoApp::DemoApp(){
	Init();
}

DemoApp::~DemoApp(){
	Mix_FreeMusic(music);
	SDL_Quit();
}

void DemoApp::GameRender(){
	player.Draw();
	bullet.Draw();
	buildLevel();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, SpriteSheetTextureID);
	glPushMatrix();
	glTranslatef(-TILE_SIZE * mapWidth / 2, TILE_SIZE * mapHeight / 2, 0.0f);

	glVertexPointer(2, GL_FLOAT, 0, levelvertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, leveltexCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, levelvertexData.size() / 2);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();



}

void DemoApp::Render(){
	glClearColor(0.7f, 0.5f, 0.9f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (State == STATE_GAME_LEVEL){

		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		float translateX = -player.x;
		float translateY = -player.y;
		if (translateY > 0)
			translateY = 0.0;
		if (translateX < -2.50){
			translateX = -2.50;
		}
		if (translateX > 2.50){
			translateX = 2.50;
		}

		glTranslatef(translateX, translateY, 0.0f);

		GameRender();
		glPopMatrix();
	}

	SDL_GL_SwapWindow(displayWindow);
}

void DemoApp::FixedUpdate(){

	//for (int i = 0; i < MAX_ENEMIES; i++){
	//	for (int j = 0; j < floor.size(); j++){
	//		if (enemies[i].checkCollision(floor[j])){
	//			enemies[i].collideBot = true;
	//		}
	//	}
	//	for (int k = 0; k < wall.size(); k++){
	//		if (enemies[i].checkCollision(wall[k])){
	//			if (k < 10){
	//				enemies[i].collideLeft = true;
	//			}
	//			else{
	//				enemies[i].collideRight = true;
	//			}
	//		}
	//	}


	//for (int i = 0; i < MAX_ENEMIES; i++){
	//	if (enemies[i].collideRight){
	//		enemies[i].velocity_x = -.2f;
	//		enemies[i].flipX = true;
	//	}
	//	if (enemies[i].collideLeft){
	//		enemies[i].velocity_x = .2f;
	//		enemies[i].flipX = false;
	//	}
	//	if (enemies[i].collideTop){
	//		enemies[i].enableCollisions = false;
	//	}
	//	if (enemies[i].collideBot){
	//		enemies[i].velocity_y = 0;
	//		enemies[i].acceleration_y = 0;
	//	}







	player.velocity_y += player.acceleration_y*FIXED_TIMESTEP;
	player.velocity_y += gravity_y*FIXED_TIMESTEP;
	player.y += player.velocity_y*FIXED_TIMESTEP;
	TileCollisonY(player);


	player.velocity_x += player.acceleration_x*FIXED_TIMESTEP;
	player.x += player.velocity_x*FIXED_TIMESTEP;
	TileCollisonX(player);
	if (bullet.flipX)
		bullet.x += .04;
	else
		bullet.x -= .04;



	//for (int i = 0; i < enemies.size(); i++){
	//	if (player.checkCollision(enemies[i])){
	//		if (enemies[i].visible)
	//			State = STATE_GAME_OVER;
	//	}
	//}

	player.acceleration_x = 0.0f;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_RIGHT]){
		player.acceleration_x = 0.2f;
		player.flipX = true;
	}
	if (keys[SDL_SCANCODE_LEFT]){
		player.acceleration_x = -0.2f;
		player.flipX = false;

	}
	if (keys[SDL_SCANCODE_UP]){
		if (player.collideBot){
			player.velocity_y = .3f;
			Mix_PlayChannel(-1, mariojump, 0);
		}
	}

	player.resetCollisions();

	player.velocity_y = lerp(player.velocity_y, 0.0f, FIXED_TIMESTEP*0.5f);
	player.velocity_x = lerp(player.velocity_x, 0.0f, FIXED_TIMESTEP*0.5f);


}

void DemoApp::Update(float elapsed){
	if (TileCollisonX(bullet)){
		bullet.visible = false;
	}
}

void DemoApp::UpdateandRender(){
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

	Update(elapsed);
	Render();
}

void DemoApp::worldToTileCoordinates(float X, float Y, int& gridX, int& gridY) {
	gridX = (int)((X + (TILE_SIZE * mapWidth / 2)) / TILE_SIZE);
	gridY = -(int)((Y - (TILE_SIZE * mapHeight / 2)) / TILE_SIZE);
	return;
}

float DemoApp::tiletoWorldCoordinatesx(int gridX)
{
	float X = (gridX)*TILE_SIZE - (TILE_SIZE * mapWidth / 2);
	return(X);
}

float DemoApp::tiletoWorldCoordinatesy(int gridY)
{
	float Y = ((-gridY)*TILE_SIZE) + (TILE_SIZE * mapHeight / 2);
	return(Y);
}

bool DemoApp::TileCollisonX(Entity &entity){
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

bool DemoApp::TileCollisonY(Entity &entity){
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

