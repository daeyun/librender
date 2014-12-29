#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "shader.h"
#include "controls.h"
#include "scry_object.h"
#include "annotation.h"
#include "shader.h"
#include "shaders/blinn_shader.h"
#include "gl_object.h"
#include "config.h"

GLFWwindow* window;

using namespace glm;

int render_object(const ScryObject& mesh) {
    // Initialise GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 4x antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // for OS X
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(config::window_width, config::window_height,
                              config::window_title.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to open GLEW." << std::endl;
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    auto bg = config::background;
    glClearColor(bg.r, bg.g, bg.b, bg.a);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    std::vector<LightProperties> lights;
    LightProperties light;
    light.light_position = config::light_position;
    light.ambient = config::ambient;
    light.constant_attenuation = config::constant_attenuation;
    light.linear_attenuation = config::linear_attenuation;
    light.quadratic_attenuation = config::quadratic_attenuation;
    lights.push_back(light);

    ShaderProperties shader_properties;
    shader_properties.lights = lights;
    shader_properties.shininess = config::shininess;
    shader_properties.strength = config::strength;

    GLuint shader_id = Shader::Shader(kBlinnShader);

    GLObject object(&mesh, shader_id, NULL);
    Annotation annotation(arma::min(mesh.v.row(2)));

    do {
        // Compute the MVP matrix from keyboard and mouse input
        if (computeMatricesFromInputs()) {
            glm::mat4 projection = getProjectionMatrix();
            glm::mat4 view = getViewMatrix();
            glm::mat4 model = glm::mat4(1.0);
            glm::vec3 eye_dir = getEyeDirection();

            shader_properties.mv = view * model;
            shader_properties.mvp = projection * shader_properties.mv;
            shader_properties.eye_direction = eye_dir;
            shader_properties.normal_mat = glm::transpose(glm::inverse(shader_properties.mv));

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            annotation.draw(projection, view, model);
            object.draw(shader_properties);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }  // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    glDeleteProgram(shader_id);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
