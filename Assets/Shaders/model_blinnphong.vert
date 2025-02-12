#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vs_out.FragPos = vec3(uModel * vec4(aPosition, 1.0));
    vs_out.Normal = mat3(transpose(inverse(uModel))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}