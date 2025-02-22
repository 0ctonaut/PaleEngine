#version 450 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

void main()
{
    vec3 norm = normalize(fs_in.Normal);
    FragColor = vec4(norm, 1.0);
}