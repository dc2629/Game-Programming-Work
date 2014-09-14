#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;

	SDL_Event event;

	glViewport(0, 0, 800, 600);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}

		}
		glClearColor(0.8f, 0.2f, 0.4f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);
		//glMatrixMode(GL_MODELVIEW);//Run multiple times inside code
		glLoadIdentity();
		glTranslatef(.5f, 0.0f, 0.0f);
		//Drawing a triangle
		GLfloat triangle[] = { 0, .5, -.5, -.5, .5, -.5 };//REMEMBER COUNTER CLOCKWISE LISTING OF VERTICES
		glVertexPointer(2, GL_FLOAT, 0, triangle);//Read 2, FLOAT VALUES, SKIP 0 values in case we put colors in the triangle, and triangle is the pointer to the array.
		glEnableClientState(GL_VERTEX_ARRAY);

		//Giving the Triangle Color
		GLfloat ctriangle[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		glColorPointer(3, GL_FLOAT, 0, ctriangle);
		glEnableClientState(GL_COLOR_ARRAY);

		//glDisableClientState(GL_COLOR_ARRAY);

		//Drawing a quad
		GLfloat quad[] = { -.5f, .5f, -.5f, -.5f, .5f, -.5f, .5f, .5f };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
		glDrawArrays(GL_TRIANGLES, 0, 3);//Drawing triangles, starting from 0, and draw 3 vertices. 
		SDL_GL_SwapWindow(displayWindow);

	}

	SDL_Quit();
	return 0;
}