#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec4 VertexColor;
layout(location = 3) in vec2 VertexTexCoord;

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat4 NormalMatrix;

// now need the transform, minus perspective
out vec4 Color;
out vec3 Normal;
out vec4 Position; // adding position, so we know where we are

void main() {
    Color = VertexColor;
    Normal = normalize(NormalMatrix * vec4(VertexNormal, 1)).xyz;
    Position = MVMatrix * vec4(VertexPosition, 1);     // pre-perspective space
    gl_Position = MVPMatrix * vec4(VertexPosition, 1); // includes perspective
}
