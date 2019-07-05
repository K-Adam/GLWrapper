#pragma once

#include "Shader.h"

#include <string>

namespace GLWRAPPER_NS {

	class ShaderBuilder {

		Shader& shader;
		GLenum shader_type;

		bool built = false;

		std::string source;

	public:

		ShaderBuilder(Shader& shader, GLenum shader_type) : shader(shader), shader_type(shader_type) {}
		~ShaderBuilder() {
			if (!built) Build();
		}

		void AppendLine(std::string line);
		void AppendString(std::string text);
		void AppendFile(std::string path, bool with_includes = true);

		void Build();

		//void LoadFromString(std::string text, GLenum shader_type);
		//void LoadFromFile(std::string path, GLenum shader_type, bool with_includes = true);

	private:

		std::string DirName(std::string path);

	};

}