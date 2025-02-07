#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex_diffuse1;

void main()
{    
    FragColor = texture(tex_diffuse1, TexCoords);
}