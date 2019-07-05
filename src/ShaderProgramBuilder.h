#pragma once

#include "ShaderProgram.h"

#include <string>

namespace GLWRAPPER_NS {

	class ShaderProgramBuilder {

		ShaderProgram& program;

		std::vector<Shader> shaders;
		std::vector<std::pair<GLuint, std::string>> attributes;

		bool built = false;

	public:

		ShaderProgramBuilder(ShaderProgram& program) : program(program) {}
		~ShaderProgramBuilder() {
			if (!built) Build();
		}

		void AttachShader(Shader& shader) {
			// Check if the shader is built
			assert(shader.Get().shader_id > 0);

			shaders.push_back(shader);
		}

		void AttachShader(Shader&& shader) {
			// Check if the shader is built
			assert(shader.Get().shader_id > 0);

			shaders.push_back(std::move(shader));
		}

		void BindAttribute(GLuint index, std::string name) {
			attributes.push_back(std::make_pair(index, name));
		}

		void Build() {

			// Don't build twice
			assert(!built);

			ShaderProgramData program_data;

			// Generate program ID
			program_data.program_id = glCreateProgram();

			// Attach shaders
			program_data.shaders = std::move(shaders);
			for (Shader& shader : program_data.shaders) {
				glAttachShader(program_data.program_id, shader.Get().shader_id);
			}

			// Bind attributes
			for (auto attrib : attributes) {
				glBindAttribLocation(program_data.program_id, attrib.first, attrib.second.c_str());
			}

			// Link program
			glLinkProgram(program_data.program_id);

			// Check for error
			GLint result = GL_FALSE;
			GLint infoLogLength = 0;

			glGetProgramiv(program_data.program_id, GL_LINK_STATUS, &result);
			glGetProgramiv(program_data.program_id, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (result == GL_FALSE)
			{

				std::vector<char> error_message(infoLogLength);
				glGetShaderInfoLog(program_data.program_id, infoLogLength, NULL, error_message.data());

				fprintf(stdout, "%s\n", error_message.data());
				assert(false);

				return;

			}

			program.Reset(std::move(program_data));
			built = true;

		}

	};

}