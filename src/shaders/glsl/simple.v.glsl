#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec4 VertexColor;

// Output data ; will be interpolated for each fragment.
out vec4 vertexColor;

// Values that stay constant for the whole mesh.
uniform mat4 MVPMatrix;

void main() {
	gl_Position =  MVPMatrix * vec4(VertexPosition, 1);
        vertexColor = VertexColor;
}

