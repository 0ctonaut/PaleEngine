#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 uLightColor;
uniform float uIntensity;

void main()
{    
    FragColor = vec4(uLightColor * uIntensity, 1.0f);
}