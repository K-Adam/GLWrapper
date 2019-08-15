/**
 * @see {@link https://github.com/K-Adam/GLWrapper}
 */

#pragma once

#include "ShaderError.h"

namespace GLWRAPPER_NS {

	class ShaderErrorParser {

		const std::string& source;

		enum ShaderErrorLogType : uint8_t {
			LOG_INTEL, LOG_NVIDIA
		};

	public:

		ShaderErrorParser(const std::string& source) : source(source) {}

		bool Parse(ShaderError& out);

	private:

		template<ShaderErrorLogType>
		bool ParseVendor(ShaderError& out) = delete;

	};

	template<>
	bool ShaderErrorParser::ParseVendor<ShaderErrorParser::LOG_INTEL>(ShaderError& out);

	template<>
	bool ShaderErrorParser::ParseVendor<ShaderErrorParser::LOG_NVIDIA>(ShaderError& out);

}