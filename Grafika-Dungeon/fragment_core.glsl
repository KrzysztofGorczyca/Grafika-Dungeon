#version 440

// ----------------------------STRUCTS----------------------------
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D diffuseTex;
    sampler2D specularTex;
};

// ----------------------------VERTEX VARIABLES----------------------------
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

// ----------------------------UNIFORM VARIABLES---------------------------
// ADD NEW SAMPLER2D WHEN ADDING TEXTURE
uniform Material material;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec3 lightPos0;
uniform vec3 cameraPos;

//----------------------------FUNCTIONS----------------------------
vec3 calculateAmbient(Material material)
{
	return material.ambient; 
}

vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal,vec3 lightPos0)
{
    vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
    float diffuse = clamp(dot(posToLightDirVec, vs_normal),0,1);
    vec3 diffuseFinal = material.diffuse * diffuse;

    return diffuseFinal;
}

vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPos)
{
	vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos - vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0.0), 32);
	vec3 specularFinal = material.specular * specularConstant;

	return specularFinal;
}

// ----------------------------MAIN FUNCTION----------------------------
void main()
{
 // Sample textures
    //vec4 texColor0 = texture(texture0, vs_texcoord);
    //vec4 texColor1 = texture(texture1, vs_texcoord);
    //fs_color = texColor0 + texColor1;

    // Ambient light
    vec3 ambientFinal = calculateAmbient(material);

    // Diffuse light
    vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, lightPos0);

    //Specular light
    vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, lightPos0, cameraPos);

    //Attenuation

    // Final color
    fs_color=texture(material.diffuseTex, vs_texcoord) * vec4(vs_color,1.f) * (vec4(ambientFinal,1.f) + vec4(diffuseFinal, 1.0f) + vec4(specularFinal, 1.0f));


}