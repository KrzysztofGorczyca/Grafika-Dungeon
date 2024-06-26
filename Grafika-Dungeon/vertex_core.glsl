#version 440

// --------------------VERTEX DATA--------------------
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;

// --------------------UNIFORMS FOR CAMERA--------------------
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

// --------------------MAIN FUNCTION--------------------
void main()
{
	vs_position = (ModelMatrix * vec4(vertex_position, 1.0)).xyz;
    vs_color = vertex_color;
    vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.0);

    // Correct transformation of normals
    mat3 normalMatrix = transpose(inverse(mat3(ModelMatrix)));
    vs_normal = normalMatrix * vertex_normal;

    // Camera transformations
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.0);

}

