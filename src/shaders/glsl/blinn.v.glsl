#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec4 VertexColor;
layout(location = 3) in vec2 VertexTexCoord;

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat4 NormalMatrix;

out vec4 Color;
out vec3 Normal;
out vec4 Position;

void main() {
    Color = VertexColor;
    Normal = VertexNormal;
    Position = vec4(VertexPosition, 1);
    gl_Position = MVPMatrix * vec4(VertexPosition, 1);
}
