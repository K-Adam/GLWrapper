/**
 * @see {@link https://github.com/K-Adam/GLWrapper}
 */

#include "app.h"

// glm
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform2.hpp>

// offsetof
#include <cstddef>

#include "Vertex.h"

#include "../src/VertexArrayBuilder.h"
#include "../src/ShaderProgramBuilder.h"
#include "../src/FrameBufferBuilder.h"

bool App::Init(int width, int height) {
	
	InitTextures();

	InitShaders();

	InitGeometries();

	InitGL();
	
	Resize(width, height);

	return true;
}

void App::InitGL() {
	// Set clear color to blue
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	// Enable backface culling and depth test
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void App::Resize(int width, int height) {
	canvas_width = width;
	canvas_height = height;

	ResizeFBO();

	glViewport(0, 0, canvas_width, canvas_height);
}

void App::InitGeometries() {

	InitCube();

}


void App::InitTextures() {

	crate_texture = gl::Texture::FromFile("Asset/crate.jpg");

}

void App::InitCube() {
	
	// Define vertices
	std::vector<Vertex> vertices;
	vertices.push_back({ glm::vec3(-1, -1,  1), glm::vec3(0, 0, 1), glm::vec2(0, 0) });	// 0
	vertices.push_back({ glm::vec3(1, -1,  1), glm::vec3(0, 0, 1), glm::vec2(1, 0) });	// 1
	vertices.push_back({ glm::vec3(-1,  1,  1), glm::vec3(0, 0, 1), glm::vec2(0, 1) });	// 2
	vertices.push_back({ glm::vec3(1,  1,  1), glm::vec3(0, 0, 1), glm::vec2(1, 1) });	// 3

	vertices.push_back({ glm::vec3(-1, -1, -1), glm::vec3(0, 0,-1), glm::vec2(1, 0) }); // 4
	vertices.push_back({ glm::vec3(1, -1, -1), glm::vec3(0, 0,-1), glm::vec2(0, 0) }); // 5
	vertices.push_back({ glm::vec3(-1,  1, -1), glm::vec3(0, 0,-1), glm::vec2(1, 1) }); // 6
	vertices.push_back({ glm::vec3(1,  1, -1), glm::vec3(0, 0,-1), glm::vec2(0, 1) }); // 7

	vertices.push_back({ glm::vec3(1, -1, -1), glm::vec3(1, 0, 0), glm::vec2(0, 0) });	// 8
	vertices.push_back({ glm::vec3(1,  1, -1), glm::vec3(1, 0, 0), glm::vec2(1, 0) });	// 9
	vertices.push_back({ glm::vec3(1, -1,  1), glm::vec3(1, 0, 0), glm::vec2(0, 1) });	// 10
	vertices.push_back({ glm::vec3(1,  1,  1), glm::vec3(1, 0, 0), glm::vec2(1, 1) });	// 11

	vertices.push_back({ glm::vec3(-1, -1, -1), glm::vec3(-1, 0, 0), glm::vec2(1, 0) }); // 12
	vertices.push_back({ glm::vec3(-1,  1, -1), glm::vec3(-1, 0, 0), glm::vec2(0, 0) }); // 13
	vertices.push_back({ glm::vec3(-1, -1,  1), glm::vec3(-1, 0, 0), glm::vec2(1, 1) }); // 14
	vertices.push_back({ glm::vec3(-1,  1,  1), glm::vec3(-1, 0, 0), glm::vec2(0, 1) }); // 15

	vertices.push_back({ glm::vec3(-1, 1,  1), glm::vec3(0, 1, 0), glm::vec2(0, 0) });	// 16
	vertices.push_back({ glm::vec3(1, 1,  1), glm::vec3(0, 1, 0), glm::vec2(1, 0) });	// 17
	vertices.push_back({ glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0), glm::vec2(0, 1) });	// 18
	vertices.push_back({ glm::vec3(1, 1, -1), glm::vec3(0, 1, 0), glm::vec2(1, 1) });	// 19

	vertices.push_back({ glm::vec3(-1, -1,  1), glm::vec3(0, -1, 0), glm::vec2(1, 0) }); // 20
	vertices.push_back({ glm::vec3(1, -1,  1), glm::vec3(0, -1, 0), glm::vec2(0, 0) }); // 21
	vertices.push_back({ glm::vec3(-1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(1, 1) }); // 22
	vertices.push_back({ glm::vec3(1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(0, 1) }); // 23

	// Define indices
	std::vector<GLuint> indices =
	{
		// elõlap
		0,1,2,
		2,1,3,

		// hátlap
		4,6,5,
		6,7,5,

		// jobb
		8,9,10,
		10,9,11,

		// bal
		12,14,13,
		14,15,13,

		// fent
		16,17,18,
		18,17,19,

		// lent
		20,22,21,
		22,23,21
	};

	// Create VAO builder
	gl::VertexArrayBuilder builder(cube_geometry);

	// Set VBO/IBO
	builder.SetVertexBuffer(gl::VertexBuffer::FromVector(vertices));
	builder.SetIndexBuffer(gl::IndexBuffer::FromVector(indices));

	// Set attributes
	builder.AddAttribute({
		3,                         // size
		GL_FLOAT,                  // type
		sizeof(Vertex),            // stride
		offsetof(Vertex, position) // offset
		});

	builder.AddAttribute({
		3,                         // size
		GL_FLOAT,                  // type
		sizeof(Vertex),            // stride
		offsetof(Vertex, normal)   // offset
		});

	builder.AddAttribute({
		2,                         // size
		GL_FLOAT,                  // type
		sizeof(Vertex),            // stride
		offsetof(Vertex, texture)  // offset
		});

	// Construct VAO
	builder.Build();
}


void App::InitShaders() {

	{
		gl::ShaderProgramBuilder shader_builder(basic_shader);

		shader_builder.AttachShader(gl::Shader::FromFile("Shaders/basic.vert.glsl", GL_VERTEX_SHADER));
		shader_builder.AttachShader(gl::Shader::FromFile("Shaders/basic.frag.glsl", GL_FRAGMENT_SHADER));

		shader_builder.BindAttribute(0, "vs_in_pos");
		shader_builder.BindAttribute(1, "vs_in_col");
		shader_builder.BindAttribute(2, "vs_in_tex");

		shader_builder.Build();
	}

	{
		gl::ShaderProgramBuilder shader_builder(postprocess_shader);

		shader_builder.AttachShader(gl::Shader::FromFile("Shaders/postprocess.vert.glsl", GL_VERTEX_SHADER));
		shader_builder.AttachShader(gl::Shader::FromFile("Shaders/postprocess.frag.glsl", GL_FRAGMENT_SHADER));

		shader_builder.Build();
	}

}

void App::Update() {}

void App::Render() {

	fbo.Bind([&](gl::FrameBuffer&) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 P = glm::perspective(45.0f, float(canvas_width)/float(canvas_height), 1.0f, 1000.0f);
		glm::mat4 V = glm::lookAt(
			glm::vec3(0, 5, 8),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0)
		);

		basic_shader.Use([&](gl::ShaderProgram& program) {
		
			cube_geometry.Bind([&](gl::VertexArray& vao) {

				glm::mat4 MVP;

				// Rotations
				float angle = SDL_GetTicks() / 1000.f;
				glm::mat4 rot =
					glm::rotate(angle, glm::vec3(1, 0, 0))*
					glm::rotate(angle, glm::vec3(0, 1, 0))*
					glm::rotate(angle, glm::vec3(0, 0, 1))
				;

				// Draw cubes
				program.SetTexture("myTexture", crate_texture, 0);

				{
					// Cube 1
					glm::mat4 M = glm::translate(glm::vec3(-2, 0, 0))*rot;
					MVP = P * V*M;
					program.SetUniform("MVP", MVP);

					vao.DrawIndexed();
				}

				{
					// Cube 2
					glm::mat4 M = glm::translate(glm::vec3(2, 0, 0))*rot;
					MVP = P * V*M;
					program.SetUniform("MVP", MVP);

					vao.DrawIndexed();
				}

			});

		});

	});

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	postprocess_shader.Use([&](gl::ShaderProgram& program) {
		
		program.SetTexture("image", fbo.GetColorBuffer(0), 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	});

}

void App::ResizeFBO() {
	gl::FrameBufferBuilder builder(fbo);

	builder.SetSize(canvas_width, canvas_height);

	//builder.AddAttachment(gl::FrameBufferAttachment::Float4());
	builder.AddAttachment(gl::FrameBufferAttachment::Byte4());

	builder.Build();
}

void App::KeyboardDown(SDL_KeyboardEvent&) {}
void App::KeyboardUp(SDL_KeyboardEvent&) {}
void App::MouseMove(SDL_MouseMotionEvent&) {}
void App::MouseDown(SDL_MouseButtonEvent&) {}
void App::MouseUp(SDL_MouseButtonEvent&) {}
void App::MouseWheel(SDL_MouseWheelEvent&) {}