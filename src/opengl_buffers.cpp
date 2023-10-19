#include "opengl_buffers.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <glad/glad.h>

#include "shader.h"
#include "util.h"

OpenGL_Buffers create_opengl_buffers(void) {
	OpenGL_Buffers new_buffers = { 0 };
	
	glGenVertexArrays(1, &new_buffers.VAO);
	
	// Generate others when supplying data so we don't have any buffers lying around
	
	glBindVertexArray(0);
	
	return new_buffers;
}

void supply_opengl_buffer_data(OpenGL_Buffers *buffers, float_dyn_array vertices, int_dyn_array indicies) {
	glBindVertexArray(buffers->VAO);
	
	if (buffers->VBO == 0) {
		glGenBuffers(1, &buffers->VBO);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size*sizeof(float), vertices.data, BUFFER_DATA_BUFFER_MODE);
	
	if (indicies.size > 0) {
		if (buffers->EBO == 0) {
			glGenBuffers(1, &buffers->EBO);
		}
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size*sizeof(int), indicies.data, BUFFER_DATA_BUFFER_MODE);
	}
	
	glBindVertexArray(0);
}

void bind_opengl_buffers(OpenGL_Buffers buffers) {
	glBindVertexArray(buffers.VAO);
	
	if (buffers.bound_program != 0) {
		glUseProgram(buffers.bound_program);
	}
}
