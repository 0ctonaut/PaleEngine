#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

uniform vec3 uLightPos[4];
uniform vec3 uLightColor[4];
uniform float uLightIntensity[4];
uniform vec3 uViewPos;

uniform bool uUseTex;

void main()
{
    vec3 norm = normalize(fs_in.Normal);
    vec3 diffuseColor = vec3(0.5);
    vec3 specularColor = vec3(0.5);
    
    // ambient
    float ambientStrength = 0.02;
  	vec3 result = vec3(0.0);
    for(int i = 0; i < 4; ++i) 
    {
        vec3 lightColor = uLightColor[i] * uLightIntensity[i] / 200;
        vec3 ambient = ambientStrength * lightColor;
        // diffuse 
        vec3 lightDir = normalize(uLightPos[i] - fs_in.FragPos);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * diffuseColor;
        
        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(uViewPos - fs_in.FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor * specularColor;
        result += (ambient + diffuse + specular);
    }
        
    FragColor = vec4(result, 1.0);
}