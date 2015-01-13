/**
 * @file trimesh_normal.glsl
 * @brief OpenGL shader for triangle face normal visualization.
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright librender is free software released under the BSD 2-Clause
 * license.
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
// out VS_GS_VERTEX { // Currently unused
//     vec4 position;
//     vec3 normal;
// } vertex_out;

//-----------------------------------------------------------------------------
void main() {
    gl_Position = vec4(VertexPosition, 1);
}
#endif
#ifdef GEOMETRY_SHADER
//=============================================================================
// Geometry Shader
//=============================================================================

// Primitive in/out specification
//-----------------------------------------------------------------------------
const int kNumArrowHeadVertices = 200;
layout(triangles) in;
layout(line_strip, max_vertices=202) out;
// 2 for the arrow body. 200 more vertices for the arrow tip. This can be
// optimized.

// Input from the vertex shader
//-----------------------------------------------------------------------------
// in VS_GS_VERTEX {  // Unused
//     vec4 position;
//     vec3 normal;
// } vertex_in[];

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

mat4 RotationMatrix(vec3 axis, float angle) {
// Thanks to https://gist.github.com/neilmendoza/4512992
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,
            oc * axis.x * axis.y - axis.z * s,
            oc * axis.z * axis.x + axis.y * s,  0.0,
            oc * axis.x * axis.y + axis.z * s,
            oc * axis.y * axis.y + c,
            oc * axis.y * axis.z - axis.x * s,  0.0,
            oc * axis.z * axis.x - axis.y * s,
            oc * axis.y * axis.z + axis.x * s,
            oc * axis.z * axis.z + c,           0.0,
            0.0, 0.0, 0.0, 1.0);
}

//-----------------------------------------------------------------------------
void main() {
    // Compute face normal. Counter-clockwise winding. cross(P1-P0, P2-P0)
    vec3 face_normal = normalize(cross(
        gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz,
        gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz
    ));

    // Center of the triangle. (P0+P1+P2)/3
    vec4 face_center = vec4((gl_in[0].gl_Position.xyz +
                            gl_in[1].gl_Position.xyz +
                            gl_in[2].gl_Position.xyz) / 3.0, 1.0);

    // Arrow base
    gl_Position = iModelViewProjectionMatrix *  face_center;
    vertex_out.color = iFaceNormalColor;
    EmitVertex();

    // End point
    vec4 tip = (face_center + iFaceNormalLength * vec4(face_normal, 0));
    gl_Position = iModelViewProjectionMatrix * tip;
    EmitVertex();

    // Arrow direction vector
    vec4 v = tip-face_center;
    mat4 rot = RotationMatrix(v.xyz, 3.1415/(kNumArrowHeadVertices/4));  
    const float arrow_head_width = 0.001;
    // An arbitrary vector parellel to the arrow direction
    vec4 p = vec4(normalize(vec3(-1/v.x, -1/v.y, 2/v.z))*arrow_head_width, 1);

    for (int i=0; i<kNumArrowHeadVertices/2; ++i){
        gl_Position = iModelViewProjectionMatrix * tip;
        EmitVertex();
        vec4 d = vec4(face_center.xyz+v.xyz*0.65 + p.xyz, 1);
        gl_Position = iModelViewProjectionMatrix * d;
        EmitVertex();
        EndPrimitive();
        p = rot*p;
    }
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
