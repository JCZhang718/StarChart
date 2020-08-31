#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
// The position, in world space
out vec3 WorldPos;
out vec3 VertexNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    vec4 worldPos4 = model * vec4(aPos, 1.0);
    WorldPos = vec3(worldPos4);
    gl_Position = projection * view * worldPos4;
    //VertexNormal = aNormal;
    VertexNormal = mat3(transpose(inverse(model))) * aNormal;
}