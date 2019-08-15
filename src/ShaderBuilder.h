/**
 * @see {@link https://github.com/K-Adam/GLWrapper}
 */

#pragma once

#include "Shader.h"
#include "ResourceBuilder.h"
#include "ShaderErrorParser.h"
#include "ShaderSourceMap.h"

#include <string>

namespace GLWRAPPER_NS {

	class ShaderBuilder: public ResourceBuilder<Shader> {

		GLenum shader_type;

		std::string source_text;

		bool version_set = false;
		int version;

		ShaderSourceMap source_map;

		void AddSourceLine(std::string line);

	public:

		ShaderBuilder(Shader& shader, GLenum shader_type);

		void AppendLine(std::string line);
		void AppendString(std::string text);
		void AppendString(std::string name, std::string text);
		void AppendFile(std::string path, bool with_includes = true);

		void Build() override;

	private:

		std::string DirName(std::string path);

	};

}