#version 330 core
out vec4 FragColor;

// -----------------------------------------------------Structs-----------------------------------------------------
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    // -----------Light variables-------------
    vec3 position;
   
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // ---------------------------------------


    // ---------Point Light variables---------
    float constant;
    float linear;
    float quadratic;
    // ---------------------------------------


    // -------Directional Light variables-----
    //vec3 lightVector;
    // ---------------------------------------


    // ---------Spot Light variables----------
    vec3 direction;
    float cutOff;
    float outerCutOff;
    // ---------------------------------------
};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

// -------------------------------------------------Main Function-------------------------------------------------
void main()
{
    //-------Ambient-----------
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    //-------Diffuse----------
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    
    //-------Specular---------
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // --------Directional Light--------
    //vec3 lightDir = normalize(-light.lightVector);
    // ---------------------------------

    // ---------Spot Light Soft Edge-----------
    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
    diffuse  *= intensity;
    specular *= intensity;

    // ---------Point Light----------
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    // -------------------------------

    //-------Attenuation From Spot Light--------
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
