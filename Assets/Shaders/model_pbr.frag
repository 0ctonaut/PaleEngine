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

uniform vec3 uLightColor[4];
uniform vec3 uLightPos[4];
uniform float uLightIntensity[4];
uniform vec3 uViewPos;

uniform bool uUseDiffuse;
uniform bool uUseSpecular;
uniform bool uUseAlbedo; 
uniform bool uUseNormal; 
uniform bool uUseMetallic;
uniform bool uUseRoughness;
uniform bool uUseAO;

uniform sampler2D tex_diffuse1;
uniform sampler2D tex_specular1;
uniform sampler2D tex_albedo1;
uniform sampler2D tex_normal1;
uniform sampler2D tex_metallic1;
uniform sampler2D tex_roughness1;
uniform sampler2D tex_ao1;

const float PI = 3.14159265359;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(tex_normal1, fs_in.TexCoords).rgb * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.FragPos);
    vec3 Q2  = dFdy(fs_in.FragPos);
    vec2 st1 = dFdx(fs_in.TexCoords);
    vec2 st2 = dFdy(fs_in.TexCoords);

    vec3 N   = normalize(fs_in.Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;
	
    return nom / denom;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
  
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness); // Geometry Obstruction
    float ggx2 = GeometrySchlickGGX(NdotL, roughness); // Geometry Shadowing
	
    return ggx1 * ggx2;
}

void main()
{
    vec3 albedo = pow(texture(tex_albedo1, fs_in.TexCoords).rgb, vec3(2.2));
    vec3 normal = getNormalFromMap();
    float metallic = texture(tex_metallic1, fs_in.TexCoords).r;
    float roughness = texture(tex_roughness1, fs_in.TexCoords).r;
    float ao = texture(tex_ao1, fs_in.TexCoords).r;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    vec3 viewDir = normalize(uViewPos - fs_in.FragPos);

    vec3 Lo = vec3(0.0);

    for(int i = 0; i < 4; ++i) 
    {
        vec3 lightDir = uLightPos[i] - fs_in.FragPos;
        float distance = length(lightDir);
        lightDir = normalize(lightDir);
        vec3 halfDir = normalize(lightDir + viewDir);

        // radiance
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = uLightColor[i] * uLightIntensity[i] * attenuation;

        // D
        float NDF = DistributionGGX(normal, halfDir, roughness);

        // F
        vec3 F = FresnelSchlick(max(dot(halfDir, viewDir), 0.0), F0);

        // G
        float G = GeometrySmith(normal, viewDir, lightDir, roughness);

        // reflectance equation
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;
        float NdotL = max(dot(normal, lightDir), 0.0);

        // --- specular
        vec3 nominator = NDF * G * F;
        float denominator = 4 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0);
        vec3 specular = nominator / max(denominator, 0.001);

        // --- diffuse
        vec3 diffuse = albedo / PI;

        Lo += (kD * diffuse + specular) * radiance * NdotL;
    }
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
    FragColor = vec4(color, 1.0);
}