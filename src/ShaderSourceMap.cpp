/**
 * @see {@link https://github.com/K-Adam/GLWrapper}
 */

#include "ShaderSourceMap.h"

namespace GLWRAPPER_NS {

	void ShaderSourceMap::PushSource(std::string name) {
		ShaderSourceRecord file = { name };
		sources.push_back(file);
		source_stack.push(uint16_t(sources.size() - 1));
	}

	void ShaderSourceMap::PopSource() {
		assert(HasSource());
		source_stack.pop();
	}

	void ShaderSourceMap::Step() {
		GetSource().current_line++;
	}

	bool ShaderSourceMap::IsSourceRegistered() {
		return last_reg_source == source_stack.top();
	}

	void ShaderSourceMap::SetSourceRegistered() {
		last_reg_source = source_stack.top();
	}

	ShaderSourceMap::ShaderSourceState ShaderSourceMap::GetSourceState() {
		return { source_stack.top(), GetSource().current_line };
	}

	std::string ShaderSourceMap::GetSourceName(uint16_t file) {
		return sources[file].name;
	}

}