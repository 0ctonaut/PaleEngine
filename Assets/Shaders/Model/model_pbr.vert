#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform mat3 uNormalMatrix;

void main()
{
    vs_out.FragPos = vec3(uModel * vec4(aPosition, 1.0));
    vs_out.Normal = uNormalMatrix * aNormal;
    vs_out.TexCoords = aTexCoords;
    gl_Position = uProjection * uView * vec4(vs_out.FragPos, 1.0);
}