
#include "ShaderProgram.h"

namespace GLWRAPPER_NS {

	void ShaderProgram::SetUniform(std::string uniform, vec2& vec)
	{
		GLint loc = Get().GetLocation(uniform);
		glUniform2fv(loc, 1, &vec[0]);
	}

	void ShaderProgram::SetUniform(std::string uniform, vec3& vec)
	{
		GLint loc = Get().GetLocation(uniform);
		glUniform3fv(loc, 1, &vec[0]);
	}

	void ShaderProgram::SetUniform(std::string uniform, vec4& vec)
	{
		GLint loc = Get().GetLocation(uniform);
		glUniform4fv(loc, 1, &vec[0]);
	}

	void ShaderProgram::SetUniform(std::string uniform, mat4& mat)
	{
		GLint loc = Get().GetLocation(uniform);
		glUniformMatrix4fv(loc, 1, GL_FALSE, &(mat[0][0]));
	}

	void ShaderProgram::SetUniform(std::string uniform, int ival)
	{
		GLint loc = Get().GetLocation(uniform);
		glUniform1i(loc, ival);
	}

	void ShaderProgram::SetUniform(std::string uniform, float fval)
	{
		GLint loc = Get().GetLocation(uniform);
		glUniform1f(loc, fval);
	}

	void ShaderProgram::SetTexture2D(std::string uniform, GLuint textureID, int sampler) {
		glActiveTexture(GL_TEXTURE0 + sampler);
		glBindTexture(GL_TEXTURE_2D, textureID);
		SetUniform(uniform, sampler);
	}

	void ShaderProgram::SetTexture3D(std::string uniform, GLuint textureID, int sampler) {
		glActiveTexture(GL_TEXTURE0 + sampler);
		glBindTexture(GL_TEXTURE_3D, textureID);
		SetUniform(uniform, sampler);
	}

	void ShaderProgram::SetTexture2D(std::string uniform, Texture& texture, int sampler) {
		SetTexture2D(uniform, texture.GetId(), sampler);
	}

	void ShaderProgram::SetTexture3D(std::string uniform, Texture& texture, int sampler) {
		SetTexture3D(uniform, texture.GetId(), sampler);
	}

}