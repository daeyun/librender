#pragma once
#include <string>
#include "shader_source.h"

// Generated from simple.v.glsl and simple.f.glsl on 2015-01-07
static const ShaderSource kSimpleShader = {
"#version 330 core\n"
"layout(location = 0) in vec3 VertexPosition;"
"layout(location = 2) in vec4 VertexColor;"
"out vec4 InterpColor;"
"uniform mat4 MVPMatrix;"
"void main() {"
"    gl_Position =  MVPMatrix * vec4(VertexPosition, 1);"
"    InterpColor = VertexColor;"
"}",
"#version 330 core\n"
"in vec4 InterpColor;"
"out vec4 FragColor;"
"void main() {"
"    FragColor = InterpColor;"
"}"
};