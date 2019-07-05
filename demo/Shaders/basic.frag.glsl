#version 130

in vec3 vs_out_col;
in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D myTexture;

void main()
{
	fs_out_col = texture(myTexture, vs_out_tex);
}
