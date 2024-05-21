#version 440

// ----------------------------VERTEX VARIABLES----------------------------
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

// ----------------------------UNIFORM VARIABLES---------------------------
// ADD NEW SAMPLER2D WHEN ADDING TEXTURE
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec3 lightPos0;


// ----------------------------MAIN FUNCTION----------------------------
void main()
{
	 // Sample textures
    //vec4 texColor0 = texture(texture0, vs_texcoord);
    //vec4 texColor1 = texture(texture1, vs_texcoord);
    //fs_color = texColor0 + texColor1;

    // Ambient light
    vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);

    // Diffuse light
    vec3 posToLightDirVec = normalize(vs_position - lightPos0);
    vec3 diffuseColor = vec3(1.0f, 1.0f, 1.0f);
    float diffuse = clamp(dot(posToLightDirVec, vs_normal),0,1);
    vec3 diffuseFinal = diffuseColor * diffuse;

    // Final color
    fs_color=texture(texture0, vs_texcoord) * vec4(vs_color,1.f) * (vec4(ambientLight,1.f) + vec4(diffuseFinal, 1.0f));


}