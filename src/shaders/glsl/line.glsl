/**
 * @file line.glsl
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright librender is free software released under the BSD 2-Clause
 * license
 */
#version 330 core
#define __SHADER_NAME__
//=============================================================================
// Globals
//=============================================================================
uniform mat4 iModelViewMatrix;
uniform mat4 iProjectionMatrix;
uniform mat4 iModelViewProjectionMatrix;
uniform mat3 iVectorModelViewMatrix;

#ifdef VERTEX_SHADER
//=============================================================================
// Vertex Shader
//=============================================================================
layout(location=0) in vec3 VertexPosition;
layout(location=2) in vec4 VertexColor;

out VS_FS_VERTEX {
    vec4 color;
} vertex_out;

void main() {
    gl_Position = iProjectionMatrix * vec4(VertexPosition, 1);
    vertex_out.color = VertexColor;
}
#endif
#ifdef FRAGMENT_SHADER
//=============================================================================
// Fragment Shader
//=============================================================================
// Interpolated
in VS_FS_VERTEX {
    vec4 color;
} fragment_in;

layout(location=0) out vec4 FragmentColor;

void main() {
    FragmentColor = fragment_in.color;
}
#endif
