#pragma once

#include "common.h"

namespace GLWRAPPER_NS {

	/* TODO */
	template<typename T>
	class AbstractBuilder {

		bool built = false;

	protected:

		T& object;

	public:

		AbstractBuilder(T& object) : object(object) {}
		virtual ~AbstractBuilder() {
			if (!built) Build();
		}

		virtual Build() {
			built = true;
		}

	};

}