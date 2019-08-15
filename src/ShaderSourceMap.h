/**
 * @see {@link https://github.com/K-Adam/GLWrapper}
 */

#pragma once

#include "common.h"

#include <string>
#include <vector>
#include <stack>

namespace GLWRAPPER_NS {

	class ShaderSourceMap {

		struct ShaderSourceRecord {
			std::string name;
			uint16_t current_line = 0;
		};

		std::vector<ShaderSourceRecord> sources;
		std::stack<uint16_t> source_stack;

		uint16_t last_reg_source;

	public:

		ShaderSourceMap() {
			PushSource("Global");
			last_reg_source = 0;
		}

		struct ShaderSourceState {
			uint16_t file;
			uint16_t line;
		};

		void PushSource(std::string name);
		void PopSource();

		void Step();

		bool IsSourceRegistered();
		void SetSourceRegistered();

		ShaderSourceState GetSourceState();
		std::string GetSourceName(uint16_t file);

	private:

		ShaderSourceRecord& GetSource() {
			assert(HasSource());

			return sources[source_stack.top()];
		}

		bool HasSource() const {
			return !source_stack.empty();
		}

	};

}