#pragma once
#include <string>
#include "shader_source.h"

static ShaderSource kSimpleShader = {
"#version 330 core\n"
"layout(location = 0) in vec3 VertexPosition;"
"layout(location = 2) in vec4 VertexColor;"
"out vec4 vertexColor;"
"uniform mat4 MVPMatrix;"
"void main() {"
"	gl_Position =  MVPMatrix * vec4(VertexPosition, 1);"
"        vertexColor = VertexColor;"
"}",
"#version 330 core\n"
"in vec4 vertexColor;"
"out vec4 color;"
"void main() {"
"    color = vertexColor;"
"}"
};
