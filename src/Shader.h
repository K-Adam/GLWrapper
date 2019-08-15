/**
 * @see {@link https://github.com/K-Adam/GLWrapper}
 */

#pragma once

#include "SharedResourceOwner.h"

#include <string>

namespace GLWRAPPER_NS {

	struct ShaderData {
		GLuint shader_id = 0;

		ShaderData() {}
		ShaderData(GLuint shader_id) : shader_id(shader_id) {}

		void Reset() {
			if (shader_id > 0) {
				glDeleteShader(shader_id);
			}
		}
	};

	class Shader : public SharedResourceOwner<ShaderData> {

	public:

		Shader() : SharedResourceOwner() {}

		static Shader FromFile(std::string path, GLenum shader_type);

	};

}