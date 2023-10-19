#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "shader.h"
#include "util.h"

#define BUFFER_DATA_BUFFER_MODE GL_STATIC_DRAW

typedef struct OpenGL_Buffers {
	uint32_t VAO, VBO, EBO;
	Shader_Prog bound_program;
} OpenGL_Buffers;

OpenGL_Buffers create_opengl_buffers(void);
void supply_opengl_buffer_data(OpenGL_Buffers *buffers, float_dyn_array vertices, int_dyn_array indicies);
void bind_opengl_buffers(OpenGL_Buffers buffers);
