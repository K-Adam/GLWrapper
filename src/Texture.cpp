#include "Texture.h"
#include "TextureBuilder.h"

namespace GLWRAPPER_NS {

	Texture Texture::FromFile(std::string path) {
		Texture img;

		TextureBuilder(img).LoadFromFile(path);

		return img;
	}

}