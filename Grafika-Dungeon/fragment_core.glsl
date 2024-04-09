#version 440

// ----------------------------VERTEX VARIABLES----------------------------
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;

out vec4 fs_color;

// ----------------------------UNIFORM VARIABLES---------------------------
// ADD NEW SAMPLER2D WHEN ADDING TEXTURE
uniform sampler2D texture0;
uniform sampler2D texture1;

// ----------------------------MAIN FUNCTION----------------------------
void main()
{
	//fs_color = vec4(vs_color, 1.f);
	fs_color = texture(texture0, vs_texcoord) + texture(texture1, vs_texcoord);
}