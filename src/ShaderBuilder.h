#pragma once

#include "Shader.h"
#include "ResourceBuilder.h"

#include <string>
#include <vector>
#include <stack>

namespace GLWRAPPER_NS {

	namespace {

		struct ShaderSourceRecord {
			std::string name;
			size_t current_line = 0;
		};

	}

	class ShaderBuilder: public ResourceBuilder<Shader> {

		GLenum shader_type;

		std::string source_text;

		bool version_set = false;
		int version;

		// <FixErrorMessage>
		// The glsl #line directive does not support source files as in C, so we need this workaround
		const uint16_t file_number_magic = 15000;
		const uint16_t first_line_magic = 25000;

		std::vector<ShaderSourceRecord> sources;
		std::stack<size_t> source_stack;
		std::vector<std::pair<size_t, size_t>> line_map; // global line nr -> source index, local line nr
		// </FixErrorMessage>

	public:

		ShaderBuilder(Shader& shader, GLenum shader_type);

		void AppendLine(std::string line);
		void AppendString(std::string text);
		void AppendString(std::string name, std::string text);
		void AppendFile(std::string path, bool with_includes = true);

		void Build() override;

	private:

		std::string DirName(std::string path);

		std::string FixErrorMessage(std::string err) const;

		void PushSource(std::string name);
		bool HasSource() const;
		bool IsGlobal() const;
		ShaderSourceRecord& GetSource();
		void PopSource();

	};

}