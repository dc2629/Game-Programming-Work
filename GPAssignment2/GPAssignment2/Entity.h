#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_opengl.h>
#include<iostream>
#include<string>
#include<stdio.h>

using namespace std;

class Entity {
private:
	GLuint LoadTexture(const char* image_path);

public:
	void Draw();
	float x;
	float y;
	float rotation;
	GLint textureID;
	char* textureLocation;

	float width;
	float height;
	float speed;
	float direction_x;
	float direction_y;
	Entity(char* pngLoc = NULL) :textureLocation(pngLoc){ if (textureLocation) textureID = LoadTexture(textureLocation); };

};

void Entity::Draw(){
	glEnable(GL_TEXTURE_2D);//enable or disable server-side GL capabilities, in this case enables 2d textures.
	glBindTexture(GL_TEXTURE_2D, textureID);//binds texture to target. Binds an image to the texture map.
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

GLuint Entity::LoadTexture(const char* image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	cout << surface << endl;
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);

	return textureID;
}