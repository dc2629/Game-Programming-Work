//#pragma once //For headers usually.

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include<iostream>
#include<string>
#include<stdio.h>
#include<math.h>

SDL_Window* displayWindow;

//Loading Textures from images, use PNG file storage type. Retuns unsigned GL int.
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


//A simple sprite system:
void DrawSprite(GLint texture, float x, float y, float rotation) {

	glEnable(GL_TEXTURE_2D);//enable or disable server-side GL capabilities, in this case enables 2d textures.
	glBindTexture(GL_TEXTURE_2D, texture);//binds texture to target. Binds an image to the texture map.
	glMatrixMode(GL_MODELVIEW);//Modelview matrix determines location and angle of the sprites.
	glLoadIdentity();//Resets all sprites.
	glTranslatef(x, y, 0.0);//move sprites across the window.
	glRotatef(rotation, 0.0, 0.0, 1.0);//rotations on the z-view.
	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };//Defines a quad to place the image. REMEMBER COUNTER CLOCKWISE LISTING OF VERTICES
	glVertexPointer(2, GL_FLOAT, 0, quad);//Read 2, FLOAT VALUES, SKIP 0 values in case we put colors in the matrix, and quad is the pointer to the array.
	glEnableClientState(GL_VERTEX_ARRAY);//allows for server to access the vertex arrays and for clients to draw the arrays.
	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };//Not really sure what it does.
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);//Defines an array of texture coordinates 
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);//Enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Alpha blending, basically removing the background of the quad.
	glDrawArrays(GL_QUADS, 0, 4);//Drawing quads, starting from 0, and draw 4 vertices. 
	glDisable(GL_TEXTURE_2D);//Disable the texture since OpenGl won't use the same texture when redrawing other quads.
}

int main(int argc, char *argv[])
{
	//Main Setup
	SDL_Init(SDL_INIT_VIDEO);//Initializes SDL
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?

	bool done = false;//Running/Updating Windows requires loops.

	SDL_Event event; //Logs the I/O of the user

	glViewport(0, 0, 800, 600);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions

	char* pngLocation = "meteorBig.png";//File needs to be in cpp file location while dlls need to be in exe location.
	GLint bMeteor = LoadTexture(pngLocation);

	char* png2location = "nyan.png";
	GLint nyan = LoadTexture(png2location);

	char* png3location = "pMouse.png";
	GLint Mouse = LoadTexture(png3location);

	float lastFrameTicks = 0.0f;
	float nyanAngle = 0.0f;

	float posX = 0.0f;
	float posY = 0.0f;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {//If the Window is closed or the user quits the program, end the loop.
				done = true;
			} else if(event.type = SDL_MOUSEMOTION){
				float unitX = (((float)event.motion.x / 800.0f)*2.66f) - 1.33f;
				float unitY = (((600.0f - (float)event.motion.y) / 600.0f)*2.0f) - 1.0f;
				posX = unitX;
				posY = unitY;
			}
		}
		glClearColor(55.0f / 255.0f, 84.0f / 255.0f, 229.0f / 255.0f, 1.0f);//Determines default coloring
		glClear(GL_COLOR_BUFFER_BIT);//Makes background default color
		
		DrawSprite(bMeteor, 0, .5, 0);
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		nyanAngle += elapsed*45.0f;
		DrawSprite(nyan, -.5, -.5, nyanAngle);

		//Coloring specific sprites
		glEnableClientState(GL_COLOR_ARRAY);
		GLfloat cArray[] = { 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0 };
		glColorPointer(4, GL_FLOAT, 0, cArray);
		DrawSprite(Mouse, posX, posY, 270);
		glDisableClientState(GL_COLOR_ARRAY);

		const Uint8 *keys = SDL_GetKeyboardState(NULL);
			if (keys[SDL_SCANCODE_LEFT]) {
			
			}
			else if (keys[SDL_SCANCODE_RIGHT]) {
				// go right! 

		SDL_GL_SwapWindow(displayWindow);//Something about there being two windows, swap the one that is visible and the one that is being programmed.
	}

	SDL_Quit();

	return 0;
}