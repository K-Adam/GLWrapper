#pragma once

#include "Texture.h"

#include <string>
#include <functional>

namespace GLWRAPPER_NS {

	class TextureBuilder {

		Texture& texture;

	public:

		TextureBuilder(Texture& texture) : texture(texture) {}

		bool LoadFromFile(std::string path);
		void Generate(std::function<vec4(float u, float v)> fnc, int width, int height);

	};

}