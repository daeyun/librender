#pragma once
#include <string>

// Generated from trimesh_normal.glsl on 2015-01-12
namespace scry {
namespace shader {
static const std::string kTrimeshNormalShader =
"#version 330 core\n#define __SHADER_NAME__\nuniform mat4 iProjectionMatrix;uniform mat4 iModelViewMatrix;uniform mat4 iModelViewProjectionMatrix;uniform mat3 iVectorModelViewMatrix;uniform vec4 iFaceNormalColor;uniform float iFaceNormalLength;\n#ifdef VERTEX_SHADER\nlayout(location = 0) in vec3 VertexPosition;layout(location = 1) in vec3 VertexNormal;layout(location = 2) in vec4 VertexColor;layout(location = 3) in vec2 VertexTexCoord;out VS_GS_VERTEX {vec4 position;vec3 normal;} vertex_out;void main() {vertex_out.position = vec4(VertexPosition, 1);vertex_out.normal = VertexNormal;}\n#endif\n#ifdef GEOMETRY_SHADER\nlayout(triangles) in;layout(line_strip, max_vertices=2) out;in VS_GS_VERTEX {vec4 position;vec3 normal;} vertex_in[];out GS_FS_VERTEX {vec4 color;} vertex_out;void main() {vec3 face_normal = normalize(cross(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz,gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz));vec4 face_center = vec4(gl_in[0].gl_Position.xyz +gl_in[1].gl_Position.xyz +gl_in[2].gl_Position.xyz) / 3.0, 1.0);gl_Position = iProjectionMatrix * face_center;vertex_out.color = iFaceNormalColor;EmitVertex();gl_Position = iProjectionMatrix *(face_center + face_normal * iFaceNormalLength);vertex_out.color = iFaceNormalColor;EmitVertex();EndPrimitive();}\n#endif\n#ifdef FRAGMENT_SHADER\nin GS_FS_VERTEX {vec4 color;} fragment_in;layout(location=0) out vec4 FragmentColor;void main() {FragmentColor = fragment_in.color;}\n#endif";
}
}