#pragma once
#include <string>

// Generated from line.glsl on 2015-01-13
namespace librender {
namespace shader {
static const std::string kLineShader =
"#version 330 core\n#define __SHADER_NAME__\nuniform mat4 iModelViewMatrix;uniform mat4 iProjectionMatrix;uniform mat4 iModelViewProjectionMatrix;uniform mat3 iVectorModelViewMatrix;\n#ifdef VERTEX_SHADER\nlayout(location=0) in vec3 VertexPosition;layout(location=2) in vec4 VertexColor;out VS_FS_VERTEX {vec4 color;} vertex_out;void main() {gl_Position = iProjectionMatrix * vec4(VertexPosition, 1);vertex_out.color = VertexColor;}\n#endif\n#ifdef FRAGMENT_SHADER\nin VS_FS_VERTEX {vec4 color;} fragment_in;layout(location=0) out vec4 FragmentColor;void main() {FragmentColor = fragment_in.color;}\n#endif";
}
}