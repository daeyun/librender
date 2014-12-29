#pragma once

#include <glm/glm.hpp>

using namespace glm;

bool computeMatricesFromInputs();

mat4 getViewMatrix();
mat4 getProjectionMatrix();
vec3 getEyeDirection();
