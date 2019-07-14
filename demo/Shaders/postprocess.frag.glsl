#version 130

in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D image;

void main() {
	
	vec3 col = texture2D(image, vs_out_tex).xyz;

	vec3 gray = vec3(dot(col, vec3(0.299, 0.587, 0.114)));

	fs_out_col = vec4(gray, 1.0);

}