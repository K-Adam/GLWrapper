/**
 * @see {@link https://github.com/K-Adam/GLWrapper}
 */

#include <iostream>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// App
#include "app.h"

/*
extern "C"
{
	__declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 1;
	__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
}
*/

void program_exit() {
	SDL_Quit();

	std::cout << "Press any key to exit" << std::endl;
	std::cin.get();
}

int main(int argc, char* args[]) {

	atexit(program_exit);

	// Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	// Set GL params
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// antialiasing - off
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

	// create window
	SDL_Window *win = nullptr;
	win = SDL_CreateWindow(
		"GL Demo",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	if (win == nullptr)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return 1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(win);
	if (context == nullptr)
	{
		SDL_Log("Unable to create OpenGL context: %s", SDL_GetError());
		return 1;
	}

	// wait for vsync
	SDL_GL_SetSwapInterval(1);

	// Start GLEW
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		SDL_Log("Unable to initialize GLEW!");
		return 1;
	}

	// Check OpenGL version
	std::pair<int, int> glVersion = { -1, -1 };
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion.first);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion.second);
	std::cout << "Running OpenGL " << glVersion.first << "." << glVersion.second << std::endl;
	std::cout << glGetString(GL_VENDOR) << " - " << glGetString(GL_RENDERER) << "\n" << std::endl;

	if (glVersion.first == -1 && glVersion.second == -1)
	{
		SDL_Log("Unable to create OpenGL context!");
		return 1;
	}

	{
		App app;

		bool quit = false;
		SDL_Event ev;

		// https://discourse.libsdl.org/t/wip-windows-high-dpi-support/22446
		int canvas_width, canvas_height;
		SDL_GL_GetDrawableSize(win, &canvas_width, &canvas_height);

		if (!app.Init(canvas_width, canvas_height))
		{
			SDL_Log("Error during app initialization!");
			quit = true;
		}

		while (!quit)
		{
			// amíg van feldolgozandó üzenet dolgozzuk fel mindet:
			while (SDL_PollEvent(&ev))
			{
				switch (ev.type)
				{
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_WINDOWEVENT:
					if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
						app.Resize(ev.window.data1, ev.window.data2);
					break;
				case SDL_KEYDOWN:
					if (ev.key.keysym.sym == SDLK_ESCAPE)
						quit = true;
					app.KeyboardDown(ev.key);
					break;
				case SDL_KEYUP:
					app.KeyboardUp(ev.key);
					break;
				case SDL_MOUSEBUTTONDOWN:
					app.MouseDown(ev.button);
					break;
				case SDL_MOUSEBUTTONUP:
					app.MouseUp(ev.button);
					break;
				case SDL_MOUSEWHEEL:
					app.MouseWheel(ev.wheel);
					break;
				case SDL_MOUSEMOTION:
					app.MouseMove(ev.motion);
					break;
				}
			}

			app.Update();
			app.Render();

			SDL_GL_SwapWindow(win);
		}
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);

	return 0;
}