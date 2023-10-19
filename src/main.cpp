#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "texture.h"
#include "opengl_buffers.h"
#include "util.h"
#include "mesh.h"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void framebuffer_size_callback(GLFWwindow *window, int new_width, int new_height) {
	glViewport(0, 0, new_width, new_height);
}

int main() {
	if (glfwInit() == GLFW_FALSE) {
		fprintf(stderr, "Error initializing GLFW.\n");
		exit(-1);
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow *window = glfwCreateWindow(640, 480, "gayum", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Error creating GLFW window!\n");
		glfwTerminate();
		exit(-1);
	}
	
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD");
		exit(-1);
	}
	
	glViewport(0, 0, 640, 480);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	Model backpack;
	assert(load_model(&backpack, "./backpack_model/backpack.obj") == 0);
	
	Vert_Shader vert_shader = create_shader_from_file("./src/shaders/main.vert", GL_VERTEX_SHADER);
	Frag_Shader frag_shader = create_shader_from_file("./src/shaders/main.frag", GL_FRAGMENT_SHADER);
	
	Shader_Prog program = create_shader_program(vert_shader, frag_shader, 1);

	while (!glfwWindowShouldClose(window)) {
		int window_width = 0, window_height = 0;
		glfwGetWindowSize(window, &window_width, &window_height);
		
		glfwSwapBuffers(window);
		
		glClearColor(0.25f, 0.4f, 0.6f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(program);
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(.71f, .71f, .71f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.f, 1.f, 0.f));
		
		glm::mat4 view = glm::mat4(1.f);
		view = glm::translate(view, glm::vec3(0.f, 0.f, -5.f));
		
		glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)window_width/(float)window_height, 0.1f, 100.f);
		
		set_shader_mat4(program, "model", model);
		set_shader_mat4(program, "view", view);
		set_shader_mat4(program, "projection", projection);
		
		draw_model(&backpack, program);
		
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}
