#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform vec3 uLightColor;
uniform float uLightIntensity;
uniform vec3 uLightPos;
uniform vec3 uViewPos;

uniform bool uUseTex;
uniform bool uUseNormalMap;
uniform sampler2D tex_diffuse1;
uniform sampler2D tex_specular1;
uniform sampler2D tex_normal1;

void main()
{
    vec3 lightColor = uLightColor * uLightIntensity;
    vec3 norm;
    
    if(uUseNormalMap)
    {
        vec3 texNormal = texture(tex_normal1, fs_in.TexCoords).rgb;
        norm = normalize(texNormal * 2.0 - 1.0);
    }
    else
    {
        norm = normalize(fs_in.Normal);
    }

    vec3 diffuseColor;
    vec3 specularColor;

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
    vec3 lightDir;
    
    if(uUseNormalMap)
    { 
        lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    }
    else
    {
        lightDir = normalize(uLightPos - fs_in.FragPos);
    }
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffuseColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir;
    if(uUseNormalMap)
    {
        viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    }
    else
    {
        viewDir = normalize(uViewPos - fs_in.FragPos);
    }
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor * specularColor;
        
    vec3 result = diffuse + specular;
    FragColor = vec4(result, 1.0);
}