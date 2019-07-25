
#include "ShaderBuilder.h"

#include <fstream>
#include <sstream>
#include <regex>

namespace GLWRAPPER_NS {

	ShaderBuilder::ShaderBuilder(Shader& shader, GLenum shader_type) : ResourceBuilder(shader), shader_type(shader_type) {
		PushSource("Global");
	}

	std::string ShaderBuilder::DirName(std::string path) {
		std::size_t found;
		found = path.find_last_of("/\\");
		return path.substr(0, found);
	}

	void ShaderBuilder::AppendLine(std::string line) {

		if (version_set) {

			if (IsGlobal()) {
				GetSource().current_line++;
			}

			source_text += line + '\n';

			if (HasSource()) {
				line_map.push_back({ source_stack.top(), GetSource().current_line });
			}
		} else {
			std::regex reg_version("\\s*#version\\s+([0-9]+)\\s*");
			std::smatch reg_match;

			if (std::regex_match(line, reg_match, reg_version)) {
				version = std::stoi(reg_match[1].str());
				version_set = true;

				std::string vv = line + '\n' + "#line " + std::to_string(first_line_magic) + ' ' + std::to_string(file_number_magic) + '\n';
				source_text += vv;

			}
		}

		
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
		PushSource(name);

		// read line by line
		std::istringstream source(text);
		std::string line;
		while (std::getline(source, line)) {
			GetSource().current_line++;
			AppendLine(line);
		}

		PopSource();
	}

	void ShaderBuilder::AppendFile(std::string path, bool with_includes) {
		
		std::string dir = DirName(path);
		std::ifstream file(path);

		PushSource(path);

		std::regex reg_include("#include\\s+\"(.*)\"");
		std::smatch reg_match;

		std::string str;
		while (std::getline(file, str))
		{
			GetSource().current_line++;

			if (
				with_includes &&
				std::regex_match(str, reg_match, reg_include) &&
				reg_match.size() == 2
			) {
				AppendFile(dir + '/' + reg_match[1].str(), true);
			} else {
				AppendLine(str);
			}
		}

		PopSource();
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
		int infoLogLength;

		glGetShaderiv(shader_data.shader_id, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shader_data.shader_id, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (result == GL_FALSE)
		{
			// Pring error message
			std::string error_message;
			error_message.resize(infoLogLength);

			glGetShaderInfoLog(shader_data.shader_id, infoLogLength, NULL, &error_message[0]);

			error_message = FixErrorMessage(error_message);

			fprintf(stdout, "%s\n", error_message.c_str());
			assert(false);
			return;
		}

		object.Reset(std::move(shader_data));

	}

	std::string ShaderBuilder::FixErrorMessage(std::string err) const {
		// TODO: Rework?

		std::string output_text;
		// Intel/AMD FNUM:LNUM
		// Nvidia FNUM(LNUM)
		std::regex line_num_regex("([0-9]+)[:\\(]([0-9]+)\\)?");

		auto callback = [&](std::string const& str) {
			std::smatch reg_match;
			if (std::regex_match(str, reg_match, line_num_regex)) {

				int filenum = std::stoi(reg_match[1].str());
				int linenum = std::stoi(reg_match[2].str());

				int global_line_num = linenum - first_line_magic -1;
				if (filenum != file_number_magic || global_line_num < 0 || global_line_num >= line_map.size()) {
					output_text += str;
					return;
				}

				auto lookup = line_map[global_line_num];

				auto file_name = sources[lookup.first].name;
				auto file_line = lookup.second;

				output_text += "( Line: " + std::to_string(file_line) + ", Source: " + file_name + " )";
			}
			else {
				output_text += str;
			}
		};

		std::sregex_token_iterator
			begin(err.begin(), err.end(), line_num_regex, { -1,0 }), end;
		std::for_each(begin, end, callback);

		return output_text;
	}

	void ShaderBuilder::PushSource(std::string name) {
		ShaderSourceRecord file = {name};
		sources.push_back(file);
		source_stack.push(size_t(sources.size()-1));
	}

	bool ShaderBuilder::HasSource() const {
		return !source_stack.empty();
	}

	bool ShaderBuilder::IsGlobal() const {
		return source_stack.size() == 1;
	}

	ShaderSourceRecord& ShaderBuilder::GetSource() {
		assert(HasSource());

		return sources[source_stack.top()];
	}

	void ShaderBuilder::PopSource() {
		assert(HasSource());
		source_stack.pop();
	}

}

