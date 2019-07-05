#pragma once

#include "ResourceOwner.h"

#include <string>

namespace GLWRAPPER_NS {

	struct TextureData {
		GLuint texture_id;

		TextureData() : texture_id(0) {}
		TextureData(GLuint texture_id) : texture_id(texture_id) {}

		void Reset() {
			if (texture_id > 0) {
				glDeleteTextures(1, &texture_id);
			}
		}
	};

	class Texture : public ResourceOwner<TextureData> {

	public:

		Texture() : ResourceOwner() {}

		GLuint GetId() const {
			return Get().texture_id;
		}

		static Texture FromFile(std::string path);

	};

}