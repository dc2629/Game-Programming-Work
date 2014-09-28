#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_opengl.h>
#include<string>
#include<vector>

using namespace std;

class Entity {
private:
	friend GLuint LoadTexture(const char* image_path);
	GLuint textureID;
public:
	void Draw();
	float x;
	float y;
	float rotation;

	char* textureLocation;

	float angle;
	float width;
	float height;
	float speed;
	float direction_x;
	float direction_y;

};

class SSEntity{
private:
	friend GLuint LoadTexture(const char* image_path);
	GLuint SStextureID;
public:
	void DrawSpriteSheetSprite();
	float x;
	float y;
	float rotation;

	char* textureLocation;

	float angle;
	float width;
	float height;
	float speed;
	float direction_x;
	float direction_y;

	int index;
	int spriteCountX;
	int spriteCountY;

};

class TextEntity{
private:
	friend GLuint LoadTexture(const char* image_path);
	GLuint fontTexture;
public:
	void DrawText();

	string text;
	float size;
	float spacing;
	float r;
	float g;
	float b;
	float a;
	char* textureLocation;
	float x;
	float y;
};


void Entity::Draw(){
	if (!textureID)
		textureID = LoadTexture(textureLocation);

	glEnable(GL_TEXTURE_2D);//enable or disable server-side GL capabilities, in this case enables 2d textures.
	glBindTexture(GL_TEXTURE_2D, textureID);//binds texture to target. Binds an image to the texture map.
	glMatrixMode(GL_MODELVIEW);//Modelview matrix determines location and angle of the sprites.
	glLoadIdentity();//Resets all sprites.
	glTranslatef(x, y, 0.0);//move sprites across the window.
	glRotatef(rotation, 0.0, 0.0, 1.0);//rotations on the z-view.
	GLfloat quad[] = { -width / 2, height / 2, -width / 2, -height / 2, width / 2, -height / 2, width / 2, height / 2 };//Defines a quad to place the image. REMEMBER COUNTER CLOCKWISE LISTING OF VERTICES
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




void TextEntity::DrawText() {
	if (!fontTexture)
		fontTexture = LoadTexture(textureLocation);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
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
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void SSEntity::DrawSpriteSheetSprite() {
	if (!SStextureID)
		SStextureID = LoadTexture(textureLocation);

	glEnable(GL_TEXTURE_2D);//enable or disable server-side GL capabilities, in this case enables 2d textures.
	glBindTexture(GL_TEXTURE_2D, SStextureID);//binds texture to target. Binds an image to the texture map.
	glMatrixMode(GL_MODELVIEW);//Modelview matrix determines location and angle of the sprites.
	glLoadIdentity();//Resets all sprites.
	glTranslatef(x, y, 0.0);//move sprites across the window.
	glRotatef(rotation, 0.0, 0.0, 1.0);//rotations on the z-view.

	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
	float spriteWidth = 1.0 / (float)spriteCountX;
	float spriteHeight = 1.0 / (float)spriteCountY;

	GLfloat quadUVs[] = { u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v };

	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);//Defines an array of texture coordinates 
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);//Enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Alpha blending, basically removing the background of the quad.
	glDrawArrays(GL_QUADS, 0, 4);//Drawing quads, starting from 0, and draw 4 vertices. 
	glDisable(GL_TEXTURE_2D);//Disable the texture since OpenGl won't use the same texture when redrawing other quads.

}