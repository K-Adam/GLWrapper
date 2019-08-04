#include "ShaderErrorParser.h"

#include <regex>

namespace GLWRAPPER_NS {

	bool ShaderErrorParser::Parse(ShaderError& out) {

		return (
			ParseVendor<LOG_INTEL>(out) ||
			ParseVendor<LOG_NVIDIA>(out)
		);

	}

	template<>
	bool ShaderErrorParser::ParseVendor<ShaderErrorParser::LOG_INTEL>(ShaderError& out) {

		std::regex reg_error("(ERROR|WARNING):\\s+([0-9]+):([0-9]+):\\s+(.*)");
		std::smatch reg_match;

		if (std::regex_match(source, reg_match, reg_error)) {

			auto err_type = reg_match[1].str();
			out.type = (err_type == "ERROR") ? ShaderError::ERROR : ShaderError::WARNING;

			out.file = std::stoi(reg_match[2].str());
			out.line = std::stoi(reg_match[3].str());

			out.message = reg_match[4].str();

			return true;
		}

		return false;

	}

	template<>
	bool ShaderErrorParser::ParseVendor<ShaderErrorParser::LOG_NVIDIA>(ShaderError& out) {

		std::regex reg_error("([0-9]+)\\(([0-9]+)\\)\\s:\\s(error|warning)\\s[A-Z0-9]+:\\s+(.*)");
		std::smatch reg_match;

		if (std::regex_match(source, reg_match, reg_error)) {

			auto err_type = reg_match[3].str();
			out.type = (err_type == "error") ? ShaderError::ERROR : ShaderError::WARNING;

			out.file = std::stoi(reg_match[1].str());
			out.line = std::stoi(reg_match[2].str());

			out.message = reg_match[4].str();

			return true;
		}

		return false;
	}

}