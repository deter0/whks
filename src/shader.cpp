#include "shader.h"

#include <glad/glad.h>
#include <stdio.h>
#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "util.h"

Shader_Prog create_shader_program(Vert_Shader vert, Frag_Shader frag, int16_t delete_shaders) {
	Shader_Prog program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	
	// Checking
	int32_t success;
	char info_log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, (GLchar*)&info_log);
		fprintf(stderr, "ERROR LINKING SHADERs! Shaders: %d, %d\nLog:\n%s\n", vert, frag, info_log);
		return 0;
	} else if (delete_shaders) {
		glDeleteShader(vert);
		glDeleteShader(frag);
	}
	
	return program;
}

void set_shader_float(Shader_Prog prog, const char *uniform_name, float value) {
	glUniform1f(glGetUniformLocation(prog, uniform_name), value);
}

void set_shader_int(Shader_Prog prog, const char *uniform_name, int value) {
	glUniform1i(glGetUniformLocation(prog, uniform_name), value);
}

void set_shader_mat4(Shader_Prog prog, const char *uniform_name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(prog, uniform_name), 1, GL_FALSE, glm::value_ptr(value));
}

Abst_Shader create_shader(const char *shader_source, GLenum shader_type, const char *shader_type_str) {
	Abst_Shader abstract_shader = glCreateShader(shader_type);
	glShaderSource(abstract_shader, 1, &shader_source, NULL);
	glCompileShader(abstract_shader);
	
	// Checking
	int32_t success;
	char info_log[512];
	glGetShaderiv(abstract_shader, GL_COMPILE_STATUS, &success);
	
	if (!success) {
		glGetShaderInfoLog(abstract_shader, 512, NULL, (GLchar*)&info_log);
		fprintf(stderr, "ERROR COMPILING SHADER! Shader Type: `%s`, Shader Source:\n===\n%s===\nLog:\n%s\n", shader_type_str, shader_source, info_log);
		return 0;
	}
	
	return abstract_shader;
}

Abst_Shader create_shader_from_file(const char *file_path, GLenum shader_type) {
	char *shader_file_source = read_entire_file(file_path);
	assert(shader_file_source != NULL);
	
	const char *shader_type_str = shader_type == GL_FRAGMENT_SHADER ? "FRAGMENT SHADER"
																																	: (shader_type == GL_VERTEX_SHADER ? "VERTEX SHADER" : "COMPUTE SHADER OR UNKNOWN");
	
	return create_shader(shader_file_source, shader_type, shader_type_str);
}
