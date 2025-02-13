#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform float uLightIntensity;
uniform vec3 uViewPos;

uniform bool uUseTex;
uniform sampler2D tex_diffuse1;
uniform sampler2D tex_specular1;

void main()
{
    vec3 lightColor = uLightColor * uLightIntensity;
    vec3 norm = normalize(fs_in.Normal);
    vec3 diffuseColor = vec3(0.0);
    vec3 specularColor = vec3(0.0);
    if(uUseTex)
    {
        vec4 texDiffuse = texture(tex_diffuse1, fs_in.TexCoords);
        diffuseColor = vec3(texDiffuse);
        vec4 texSpecular = texture(tex_specular1, fs_in.TexCoords);
        specularColor = vec3(texSpecular);
    }
    else
    {
        diffuseColor = vec3(0.5);
        specularColor = vec3(0.5);
    }

    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 lightDir = normalize(uLightPos - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 texDiffuse = texture(tex_diffuse1, fs_in.TexCoords);
    vec3 diffuse = diff * lightColor * diffuseColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 texSpecular = texture(tex_specular1, fs_in.TexCoords);
    vec3 specular = specularStrength * spec * lightColor * specularColor;  
        
    vec3 result = diffuse + specular;
    // FragColor = vec4(fs_in.FragPos, 1.0);
    FragColor = vec4(result, 1.0);
}