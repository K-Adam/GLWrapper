/**
 * @see {@link https://github.com/K-Adam/GLWrapper}
 */

#pragma once

#include "common.h"

#include <string>

namespace GLWRAPPER_NS {

	struct ShaderError {

		enum ErrorType {
			WARNING, ERROR
		};

		uint16_t line;
		uint16_t file;
		ErrorType type;
		std::string message;

		std::string file_name;
	};

}