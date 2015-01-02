/**
 * @file blinn.v.glsl
 * @brief Shader based on the Blinn-Phong model.
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software under the BSD 2-Clause license.
 */
#version 330 core

// Input
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec4 VertexColor;
layout(location = 3) in vec2 VertexTexCoord;

// Output for the fragment shader
out vec4 Color;
out vec3 Normal;
out vec4 Position;

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat4 NormalMatrix;

void main() {
    Color = VertexColor;
    Normal = VertexNormal;
    Position = vec4(VertexPosition, 1);
    gl_Position = MVPMatrix * vec4(VertexPosition, 1);
}
