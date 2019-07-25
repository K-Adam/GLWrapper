#version 130

#include "common.glsl"

in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D image;

void main() {
	
	vec3 col = texture2D(image, vs_out_tex).xyz;

	fs_out_col = vec4(grayscale(col), 1.0);

}