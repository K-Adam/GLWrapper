#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLWrapper
#include "../src/VertexArray.h"
#include "../src/ShaderProgram.h"
#include "../src/Texture.h"
#include "../src/FrameBuffer.h"

class App {

public:

	App() {}

	bool Init(int width, int height);
	void Resize(int width, int height);

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);

private:

	int canvas_width;
	int canvas_height;

	gl::VertexArray cube_geometry;

	gl::Texture crate_texture;

	gl::ShaderProgram basic_shader;
	gl::ShaderProgram postprocess_shader;

	gl::FrameBuffer fbo;

	void InitTextures();

	void InitGeometries();
	void InitCube();

	void InitShaders();

	void InitGL();

	void ResizeFBO();
};