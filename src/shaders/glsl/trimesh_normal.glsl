/**
 * @file trimesh_normal.glsl
 * @brief OpenGL shader for triangle face normal visualization.
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software under the BSD 2-Clause license.
 */
#version 330 core
#define __SHADER_NAME__
//=============================================================================
// Globals
//=============================================================================

uniform mat4 iProjectionMatrix;
// ModelView = [world to view] x [model to world]
uniform mat4 iModelViewMatrix;
// ModelViewProjection = 
// [view to projection] x [world to view] x [model to world]
uniform mat4 iModelViewProjectionMatrix;
// VectorModelView = mat3(inv(ModelView)')
// Transform vectors to view space. Preserves angles and lengths.
// Also called "normal matrix".
uniform mat3 iVectorModelViewMatrix;

uniform vec4 iFaceNormalColor;
uniform float iFaceNormalLength;

#ifdef VERTEX_SHADER
//=============================================================================
// Vertex Shader
//=============================================================================

// Input from the vertex array object
//-----------------------------------------------------------------------------
// in view space
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec4 VertexColor;
layout(location = 3) in vec2 VertexTexCoord;

// Output to the geometry shader
//-----------------------------------------------------------------------------
out VS_GS_VERTEX {
    vec4 position;
    vec3 normal;
} vertex_out;

//-----------------------------------------------------------------------------
void main() {
    vertex_out.position = vec4(VertexPosition, 1);
    vertex_out.normal   = VertexNormal;
}
#endif
#ifdef GEOMETRY_SHADER
//=============================================================================
// Geometry Shader
//=============================================================================

// Primitive in/out specification
//-----------------------------------------------------------------------------
layout(triangles) in;
layout(line_strip, max_vertices=2) out;

// Input from the vertex shader
//-----------------------------------------------------------------------------
// View space
in VS_GS_VERTEX {
    vec4 position;
    vec3 normal;
} vertex_in[];

/** Built-in
in gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[]; */

// Output to the fragment shader
//-----------------------------------------------------------------------------
out GS_FS_VERTEX {
    vec4 color;
} vertex_out;

//-----------------------------------------------------------------------------
void main() {
    // Compute face normal. Counter-clockwise winding. cross(P1-P0, P2-P0)
    vec3 face_normal = normalize(cross(
        gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz,
        gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz
    ));

    // Center of the triangle. (P0+P1+P2)/3
    vec4 face_center = vec4(gl_in[0].gl_Position.xyz +
                            gl_in[1].gl_Position.xyz +
                            gl_in[2].gl_Position.xyz) / 3.0, 1.0);

    // Compute both positions in view space and apply projection transform
    gl_Position = iProjectionMatrix * face_center;
    vertex_out.color = iFaceNormalColor;
    EmitVertex();

    gl_Position = iProjectionMatrix *
        (face_center + face_normal * iFaceNormalLength);
    vertex_out.color = iFaceNormalColor;
    EmitVertex();

    EndPrimitive();
}
#endif
#ifdef FRAGMENT_SHADER
//=============================================================================
// Fragment Shader
//=============================================================================

// Input from the geometry shader
//-----------------------------------------------------------------------------
// Interpolated
in GS_FS_VERTEX {
    vec4 color;
} fragment_in;

/** Built-in
in vec4 gl_FragCoord;
in bool gl_FrontFacing;
in vec2 gl_PointCoord; */

// Output to the draw buffer
//-----------------------------------------------------------------------------
layout(location=0) out vec4 FragmentColor;

//-----------------------------------------------------------------------------
void main() {
    FragmentColor = fragment_in.color;
}
#endif
