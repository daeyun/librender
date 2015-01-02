/**
 * @file simple.v.glsl
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software under the BSD 2-Clause license.
 */
#version 330 core

// Input
layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec4 VertexColor;

// Output for the fragment shader
out vec4 InterpColor;

uniform mat4 MVPMatrix;

void main() {
    gl_Position =  MVPMatrix * vec4(VertexPosition, 1);
    InterpColor = VertexColor;
}
