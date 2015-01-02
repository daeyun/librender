/**
 * @file simple.v.glsl
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software under the BSD 2-Clause license.
 */
#version 330 core

in vec4 InterpColor;

out vec4 FragColor;

void main() {
    FragColor = InterpColor;
}
