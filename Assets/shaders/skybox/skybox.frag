#version 450 core
out vec4 FragColor;
in vec3 TexCoords;
uniform samplerCube skybox;
void main()
{
    vec3 color = textureLod(skybox, TexCoords, 0.0).rgb;
    FragColor = vec4(color, 1.0);
}
