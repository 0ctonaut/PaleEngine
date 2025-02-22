#version 450 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

uniform vec3 uViewPos;
uniform samplerCube tex_skybox1;

void main()
{
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(fs_in.FragPos - uViewPos);
    vec3 R = refract(I, normalize(fs_in.Normal), ratio);
    FragColor = vec4(texture(tex_skybox1, R).rgb, 1.0);
}

