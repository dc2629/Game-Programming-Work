#include"Platformer.h"

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
		glLoadIdentity();//Resets all sprites.
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
	
	return x_pen;
	



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

	SpriteSheetTextureID = LoadTexture("SpriteSheet.png");
	lastFrameTicks = 0.0f;
	gravity_y = -0.1f;
	timeLeftOver = 0.0f;

	glMatrixMode(GL_MODELVIEW);
	
	genEnemy.textureID = SpriteSheetTextureID;
	genEnemy.spriteCountX = 16;
	genEnemy.spriteCountY = 8;
	genEnemy.index = 81;
	genEnemy.height = 0.1f;
	genEnemy.width = 0.1f;
	genEnemy.health = 3;
	genEnemy.visible = true;
	genEnemy.enableCollisions = true;
	
	for (int i = 0; i < MAX_ENEMIES; i++){
		genEnemy.x = -0.5f + RANDOM_NUMBER;
		if (RANDOM_NUMBER>.5)
			genEnemy.velocity_x = 0.2;
		else{
			genEnemy.velocity_x = -0.2;
			genEnemy.flipX = true;
		}
			
		genEnemy.y = 0.9f;
		enemies.push_back(genEnemy);
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
	player.x = -1.0f;
	player.y = 0.6f;

	tile.textureID = SpriteSheetTextureID;
	tile.spriteCountX = 16;
	tile.spriteCountY = 8;
	tile.visible = true;
	tile.enableCollisions = true;
	tile.isStatic = true;
	tile.mass = 999999;
	tile.friction_x = 5;
	tile.friction_y = .5;
	tile.width = 0.1f;
	tile.height = .025;
	tile.y = -0.8f;
	for (int i = 0; i < 25; i++){
		tile.x = -1.2f + ((float)i / 10.0f);
		tile.index = 16 + i % 4;
		floor.push_back(tile);
	}

	tile.index = 4;
	tile.height = 0.1f;

	tile.x = -1.2f;
	for (int j = 0; j < 10; j++){
		tile.y = -0.75f + ((float)j / 10.0f);
		wall.push_back(tile);
	}

	tile.x = 1.2f;
	for (int j = 0; j < 10; j++){
		tile.y = -0.75f + ((float)j / 10.0f);
		wall.push_back(tile);
	}

	tile.index = 89;
	tile.y = -0.6f;
	for (int i = 0; i < 20; i++){
		if (i % 5 == 0){
			tile.x = -0.75f + ((float)i / 10.0f);
			floor.push_back(tile);
		}
		
	}

}

DemoApp::DemoApp(){
	Init();
}

DemoApp::~DemoApp(){
	SDL_Quit();
}

void DemoApp::GameRender(){
	player.Draw();
	for (int i = 0; i < floor.size(); i++){
		floor[i].Draw();
	}
	for (int i = 0; i < wall.size(); i++){
		wall[i].Draw();
	}
	for (int i = 0; i < MAX_ENEMIES; i++){
		enemies[i].Draw();
	}
	

}

void DemoApp::Render(){

	glClear(GL_COLOR_BUFFER_BIT);

	if (State == STATE_GAME_LEVEL){
		GameRender();
	}



	SDL_GL_SwapWindow(displayWindow);
}

void DemoApp::FixedUpdate(){

	for (int i = 0; i < MAX_ENEMIES; i++){
		for (int j = 0; j < floor.size(); j++){
			if (enemies[i].checkCollision(floor[j])){
				enemies[i].collideBot = true;
			}
		}
		for (int k = 0; k < wall.size(); k++){
			if (enemies[i].checkCollision(wall[k])){
				if (k < 10){
					enemies[i].collideLeft = true;
				}
				else{
					enemies[i].collideRight = true;
				}
			}
		}
		if (player.checkCollision(enemies[i])){
			float x_pen = player.CalculateX_Pen(enemies[i]);
			float y_pen = player.CalculateY_Pen(enemies[i]);
			if (x_pen > y_pen){
				cout << player.health << endl;
				player.health--;
			}
			else{
				enemies[i].visible = 0;
			}


		}

	}


	for (int i = 0; i < MAX_ENEMIES; i++){
		if (enemies[i].collideRight){
			enemies[i].velocity_x = -.2f;
			enemies[i].flipX = true;
		}
		if (enemies[i].collideLeft){
			enemies[i].velocity_x = .2f;			
			enemies[i].flipX = false;
		}
		if (enemies[i].collideTop){
			enemies[i].enableCollisions = false;
		}
		if (enemies[i].collideBot){
			enemies[i].velocity_y = 0;
			enemies[i].acceleration_y = 0;
		}

	}
	for (int i = 0; i < floor.size(); i++){
		if (player.checkCollision(floor[i])){
			player.collideBot = true;
			player.acceleration_y = 0;
			player.y += player.CalculateY_Pen(floor[i]);
		}
	}

	for (int j = 0; j < wall.size(); j++){
		if (player.checkCollision(wall[j])){
			if (j < 10){
				player.collideLeft = true;
				player.x += player.CalculateX_Pen(wall[j]);
			}
			else{
				player.collideRight = true;
				player.x -= player.CalculateX_Pen(wall[j]);
			}
		}
	}

}

void DemoApp::Update(float elapsed){
	for (int i = 0; i < MAX_ENEMIES; i++){
		enemies[i].velocity_x += enemies[i].acceleration_x * elapsed;
		enemies[i].velocity_y += enemies[i].acceleration_y * elapsed;
		enemies[i].y += enemies[i].velocity_y * elapsed;
		enemies[i].x += enemies[i].velocity_x * elapsed;
		enemies[i].velocity_y += gravity_y*elapsed;
		enemies[i].resetCollisions();
	}
	if (!player.collideBot){
		player.velocity_y += gravity_y*elapsed;
		player.y += player.velocity_y*elapsed;
	}

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_RIGHT]){
		if (!player.collideRight){
			player.velocity_x += 0.02f;
			player.acceleration_x += 0.01f*elapsed;
			player.flipX = true;
		}
	}
	if (keys[SDL_SCANCODE_LEFT]){
		if (!player.collideLeft){
			player.velocity_x -= 0.02f;
			player.acceleration_x -= 0.01f*elapsed;
			player.flipX = false;
		}

	}
	if (keys[SDL_SCANCODE_UP]){
		if (player.collideBot){
			player.acceleration_y = 0;
			player.velocity_y = .3f;
			player.y += player.velocity_y*elapsed;
		}
	}
	player.velocity_x += player.acceleration_x*elapsed;
	player.x += player.velocity_x*elapsed;
	player.velocity_y = lerp(player.velocity_y, 0.0f, elapsed*floor[0].friction_y);
	player.velocity_x = lerp(player.velocity_x, 0.0f, elapsed*floor[0].friction_x);
	
	player.resetCollisions();

	if (player.health == 0){
		State = STATE_GAME_OVER;
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

	Update(fixedElapsed);
	Render();
}