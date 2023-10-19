#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <string.h>

#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

typedef struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coord;
} Vertex;

typedef struct MeshTexture {
	Texture loaded_tex;
	char *path; // Heap Alloc'd
	const char *type;
} MeshTexture;

typedef struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	std::vector<MeshTexture> textures;
	
	uint VAO, VBO, EBO;
} Mesh;

typedef struct Model {
	std::vector<Mesh> meshes;
	char *mesh_file_dir; // heap Alloc'd
} Model;

void setup_mesh(Mesh *mesh);
void mesh_delete(Mesh *mesh);
void draw_mesh(Mesh *mesh, Shader_Prog shader);

int load_model(Model *model, const char *path);
void delete_model(Model *model);
void draw_model(Model *model, Shader_Prog shader);
