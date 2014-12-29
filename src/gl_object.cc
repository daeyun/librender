#include "gl_object.h"

#include <exception>
#include <armadillo>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "shader.h"
#include "shaders/standard_shader.h"
#include "gl_buffer.h"

extern GLuint scry_current_shader;

GLObject::GLObject() : texture(0) {}

GLObject::GLObject(const ScryObject* scry_object, const GLuint shader_id,
                   const char* texture_filename)
    : texture(0) {
    glGenVertexArrays(1, &this->vertex_array_id);
    glBindVertexArray(this->vertex_array_id);

    if (texture_filename != NULL) {
        // TODO
        this->texture = load_texture(texture_filename);
    }

    this->shader_attribs.id = shader_id;
    this->shader_attribs.mvp = glGetUniformLocation(shader_id, "MVPMatrix");
    this->shader_attribs.mv = glGetUniformLocation(shader_id, "MVMatrix");
    this->shader_attribs.normal_mat = glGetUniformLocation(shader_id, "NormalMatrix");

    this->shader_attribs.num_lights = glGetUniformLocation(shader_id, "NumLights");
    this->shader_attribs.eye_direction = glGetUniformLocation(shader_id, "EyeDirection");
    this->shader_attribs.shininess = glGetUniformLocation(shader_id, "Shininess");
    this->shader_attribs.strength = glGetUniformLocation(shader_id, "Strength");

    for (size_t i = 0; i < kNumMaxLights; i++) {
        std::string name_prefix = "Lights";
        name_prefix += "[" + std::to_string(i) + "].";

        GLShaderLight light;
        light.isEnabled = glGetUniformLocation(shader_id, (name_prefix + "isEnabled").c_str());
        light.Ambient = glGetUniformLocation(shader_id, (name_prefix + "Ambient").c_str());
        light.LightColor = glGetUniformLocation(shader_id, (name_prefix + "LightColor").c_str());
        light.LightPosition = glGetUniformLocation(shader_id, (name_prefix + "LightPosition").c_str());
        light.ConstantAttenuation = glGetUniformLocation(shader_id, (name_prefix + "ConstantAttenuation").c_str());
        light.LinearAttenuation = glGetUniformLocation(shader_id, (name_prefix + "LinearAttenuation").c_str());
        light.QuadraticAttenuation = glGetUniformLocation(shader_id, (name_prefix + "QuadraticAttenuation").c_str());

        this->shader_attribs.lights.push_back(light);
    }

    this->scry_object = scry_object;

    attribute_buffers.push_back(GLVertexAttribBuffer(
        GL_ARRAY_BUFFER, 0, this->scry_object->v.n_rows, GL_FLOAT,
        &this->scry_object->v[0], this->scry_object->v.n_elem * sizeof(float)));

    if (!this->scry_object->vn.empty()) {
        attribute_buffers.push_back(GLVertexAttribBuffer(
            GL_ARRAY_BUFFER, 1, this->scry_object->vn.n_rows, GL_FLOAT,
            &this->scry_object->vn[0],
            this->scry_object->vn.n_elem * sizeof(float)));
    }

    if (!this->scry_object->cdata.empty()) {
        attribute_buffers.push_back(GLVertexAttribBuffer(
            GL_ARRAY_BUFFER, 2, this->scry_object->cdata.n_rows, GL_FLOAT,
            &this->scry_object->cdata[0],
            this->scry_object->cdata.n_elem * sizeof(float)));
    }

    if (texture_filename != NULL) {
        attribute_buffers.push_back(GLVertexAttribBuffer(
            GL_ARRAY_BUFFER, 3, this->scry_object->uv.n_rows, GL_FLOAT,
            &this->scry_object->uv[0],
            this->scry_object->uv.n_elem * sizeof(float)));
    }

    GLenum draw_mode = GL_TRIANGLES;
    if (scry_object->type == kLines) {
        draw_mode = GL_LINES;
    }

    index_buffer = GLIndexBuffer(
        GL_ELEMENT_ARRAY_BUFFER, draw_mode, this->scry_object->f.n_elem,
        GL_UNSIGNED_INT, &this->scry_object->f[0],
        this->scry_object->f.n_elem * sizeof(arma::uword));
}

void GLObject::clean() {
    for (GLVertexAttribBuffer buf : this->attribute_buffers) {
        buf.clean();
    }

    glDeleteVertexArrays(1, &this->vertex_array_id);
    if (this->texture != 0) {
        glDeleteTextures(1, &this->texture);
    }
}

GLuint GLObject::load_texture(const char* texture_filename) {
    std::string filename_str(texture_filename);
    std::string ext = filename_str.substr(filename_str.find_last_of(".") + 1);

    // TODO
    throw std::runtime_error("Not implemented");
}

void GLObject::draw(const ShaderProperties& shader_properties) {
    glBindVertexArray(this->vertex_array_id);

    GLuint shader_id = this->shader_attribs.id;
    if (shader_id != scry_current_shader) {
        scry_current_shader = shader_id;
        glUseProgram(shader_id);
    }

    glUniformMatrix4fv(this->shader_attribs.mvp, 1, GL_FALSE, &shader_properties.mvp[0][0]);
    glUniformMatrix4fv(this->shader_attribs.mv, 1, GL_FALSE, &shader_properties.mv[0][0]);
    glUniformMatrix4fv(this->shader_attribs.normal_mat, 1, GL_FALSE, &shader_properties.normal_mat[0][0]);
    glUniform1i(this->shader_attribs.num_lights, shader_properties.lights.size());
    glUniform3fv(this->shader_attribs.eye_direction, 1, &shader_properties.eye_direction[0]);
    glUniform1f(this->shader_attribs.shininess, shader_properties.shininess);
    glUniform1f(this->shader_attribs.strength, shader_properties.strength);

    for (size_t i = 0; i < shader_properties.lights.size(); i++) {
        glUniform1i(this->shader_attribs.lights[i].isEnabled, shader_properties.lights[i].is_enabled);
        glUniform3fv(this->shader_attribs.lights[i].Ambient, 1, &shader_properties.lights[i].ambient[0]);
        glUniform3fv(this->shader_attribs.lights[i].LightColor, 1, &shader_properties.lights[i].light_color[0]);
        glUniform3fv(this->shader_attribs.lights[i].LightPosition, 1, &shader_properties.lights[i].light_position[0]);
        glUniform1f(this->shader_attribs.lights[i].ConstantAttenuation, shader_properties.lights[i].constant_attenuation);
        glUniform1f(this->shader_attribs.lights[i].LinearAttenuation, shader_properties.lights[i].linear_attenuation);
        glUniform1f(this->shader_attribs.lights[i].QuadraticAttenuation, shader_properties.lights[i].quadratic_attenuation);
    }

    /* TODO
    if (this->texture != 0) {
        // Bind texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glUniform1i(this->shader_attribs.texture, 0);
    } */

    this->index_buffer.draw();
}
