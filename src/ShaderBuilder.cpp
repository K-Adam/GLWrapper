/**
 * @see {@link https://github.com/K-Adam/GLWrapper}
 */

#include "ShaderBuilder.h"

#include <fstream>
#include <sstream>
#include <regex>

namespace GLWRAPPER_NS {

	ShaderBuilder::ShaderBuilder(Shader& shader, GLenum shader_type) :
		ResourceBuilder(shader),
		shader_type(shader_type)
	{}

	std::string ShaderBuilder::DirName(std::string path) {
		std::size_t found;
		found = path.find_last_of("/\\");
		return path.substr(0, found);
	}

	void ShaderBuilder::AddSourceLine(std::string line) {

		if (version_set) {

			if (!source_map.IsSourceRegistered()) {
				auto state = source_map.GetSourceState();
				source_text.append("#line " + std::to_string(state.line-1) + ' ' + std::to_string(state.file) + '\n');
				source_map.SetSourceRegistered();
			}

		} else {
			std::regex reg_version("\\s*#version\\s+([0-9]+)\\s*");
			std::smatch reg_match;

			if (std::regex_match(line, reg_match, reg_version)) {
				version = std::stoi(reg_match[1].str());
				version_set = true;
			}
		}

		source_text.append(line + '\n');

	}

	void ShaderBuilder::AppendLine(std::string line) {
		source_map.Step();
		AddSourceLine(line);
	}

	void ShaderBuilder::AppendString(std::string text) {
		// Create source record
		auto startpos = text.find_first_not_of(" \t\n");
		std::string short_name = text.substr(startpos, 16);
		if ((text.size() - startpos) > 16) {
			short_name += "...";
		}
		std::replace(short_name.begin(), short_name.end(), '\n', ' ');

		AppendString("String(" + short_name + ")", text);
	}

	void ShaderBuilder::AppendString(std::string name, std::string text) {
		source_map.PushSource(name);

		// read line by line
		std::istringstream source(text);
		std::string line;
		while (std::getline(source, line)) {
			source_map.Step();
			AddSourceLine(line);
		}

		source_map.PopSource();
	}
	
	void ShaderBuilder::AppendFile(std::string path, bool with_includes) {
		
		std::string dir = DirName(path);
		std::ifstream file(path);

		source_map.PushSource(path);

		std::regex reg_include("#include\\s+\"(.*)\"");
		std::smatch reg_match;

		std::string str;
		while (std::getline(file, str))
		{
			source_map.Step();

			if (
				with_includes &&
				std::regex_match(str, reg_match, reg_include) &&
				reg_match.size() == 2
			) {
				AppendFile(dir + '/' + reg_match[1].str(), true);
			} else {
				AddSourceLine(str);
			}
		}

		source_map.PopSource();
	}

	void ShaderBuilder::Build() {

		ResourceBuilder::Build();

		// Set the shader version
		assert(version_set);

		ShaderData shader_data;

		// Generate shader id
		shader_data.shader_id = glCreateShader(shader_type);

		// Check for error
		if (shader_data.shader_id == 0)
		{
			assert(false);
			return;
		}
		
		// Load shader from source string
		const char* source_prt = source_text.c_str();
		glShaderSource(shader_data.shader_id, 1, &source_prt, NULL);

		// Compile the loaded shader
		glCompileShader(shader_data.shader_id);

		// Check for error
		GLint result = GL_FALSE;

		glGetShaderiv(shader_data.shader_id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int infoLogLength;
			glGetShaderiv(shader_data.shader_id, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Pring error message
			std::string error_message;
			error_message.resize(infoLogLength);

			glGetShaderInfoLog(shader_data.shader_id, infoLogLength, NULL, &error_message[0]);

			// read line by line
			std::istringstream source(error_message);
			std::string line;
			ShaderError error;
			while (std::getline(source, line)) {

				if (ShaderErrorParser(line).Parse(error)) {
					error.file_name = source_map.GetSourceName(error.file);
					fprintf(stdout, "Error( Line: %d, Source: %s ): %s\n", error.line, error.file_name.c_str(), error.message.c_str());
				} else {
					// print original
					fprintf(stdout, "%s\n", line.c_str());
				}
			}

			assert(false);
			return;
		}

		object.Reset(std::move(shader_data));

	}

}

