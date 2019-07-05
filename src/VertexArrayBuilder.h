#pragma once

#include "VertexArray.h"

#include <vector>

namespace GLWRAPPER_NS {

	struct VertexAttribute {
		GLint size;
		GLenum type;
		GLsizei stride;
		GLsizei offset;

		GLboolean normalized = GL_FALSE;
	};

	class VertexArrayBuilder {

		VertexArray& vao;

		VertexBuffer vbo;
		IndexBuffer ibo;
		std::vector<VertexAttribute> attributes;

		bool built = false;

	public:

		VertexArrayBuilder(VertexArray& vao) : vao(vao) {}
		~VertexArrayBuilder() {
			if (!built) Build();
		}

		void SetVertexBuffer(VertexBuffer&& buffer) {
			vbo = std::move(buffer);
		}

		void SetIndexBuffer(IndexBuffer&& buffer) {
			ibo = std::move(buffer);
		}

		void AddAttribute(const VertexAttribute attribute) {
			attributes.push_back(attribute);
		}

		void Build() {

			// Don't build twice
			assert(!built);

			VertexArrayData vao_data;

			// create and bind VAO
			glGenVertexArrays(1, &vao_data.vao_id);
			glBindVertexArray(vao_data.vao_id);

			// Needs a VBO
			assert(vbo.GetId() != 0);

			// binds VBO
			glBindBuffer(GL_ARRAY_BUFFER, vbo.GetId());

			vao_data.vbo = std::move(vbo);

			// IBO ( optional )
			if (ibo.GetId() > 0) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.GetId());

				vao_data.ibo = std::move(ibo);
			}

			// Needs attributes
			assert(attributes.size() > 0);

			// Set attributes
			for (int i = 0; i < attributes.size(); i++) {
				const VertexAttribute& attribute = attributes[i];

				glEnableVertexAttribArray(i);
				glVertexAttribPointer(
					i,
					attribute.size,
					attribute.type,
					attribute.normalized,
					attribute.stride,
					(GLvoid*)(std::uintptr_t(attribute.offset))
				);
			}

			vao.Reset(std::move(vao_data));
			built = true;
		}

	};

}