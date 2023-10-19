#pragma once

#include <stdlib.h>
#include <stdint.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

typedef int32_t Abst_Shader;
typedef int32_t Vert_Shader;
typedef int32_t Frag_Shader;
typedef int32_t Shader_Prog;

Shader_Prog create_shader_program(Vert_Shader vert, Frag_Shader frag, int16_t delete_shaders);
Abst_Shader create_shader(const char *shader_source, GLenum shader_type, const char *shader_type_str);
Abst_Shader create_shader_from_file(const char *file_path, GLenum shader_type);

void set_shader_float(Shader_Prog prog, const char *uniform_name, float value);
void set_shader_int(Shader_Prog prog, const char *uniform_name, int value);
void set_shader_mat4(Shader_Prog prog, const char *uniform_name, glm::mat4 value);
