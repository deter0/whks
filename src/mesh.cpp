#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <string.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "util.h"
#include "texture.h"

#include "mesh.h"

static std::vector<MeshTexture> loaded_textures;

void setup_mesh(Mesh *mesh) {
	glGenVertexArrays(1, &mesh->VAO);
	
	glGenBuffers(1, &mesh->VBO);
	glGenBuffers(1, &mesh->EBO);
	
	glBindVertexArray(mesh->VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(uint), &mesh->indices[0], GL_STATIC_DRAW);
	
	// Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	
	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	
	// Tex Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
	
	glBindVertexArray(0);
}

void mesh_delete(Mesh *mesh) {
	mesh->vertices.clear();
	mesh->indices.clear();
	
	for (uint i = 0; i < mesh->textures.size(); i++) {
		// TODO(kay): (BUG) If another mesh is using the same texture, the texture will be deleted. !! For now we are not since we do not optimize this
		delete_texture(mesh->textures[i].loaded_tex);
		free(mesh->textures[i].path);
	}
	mesh->textures.clear();
	
	glDeleteVertexArrays(1, &mesh->VAO);
	glDeleteBuffers(1, &mesh->VBO);
	glDeleteBuffers(1, &mesh->EBO);
}

void draw_mesh(Mesh *mesh, Shader_Prog shader) {
	uint num_diffuse = 1;
	uint num_spec = 1;
	
	for (uint i = 0; i < mesh->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		
		char number_str[16] = { 0 };
		const char *type_name = mesh->textures[i].type;
		
		if (strcmp(type_name, "texture_diffuse") == 0) {
			snprintf(number_str, sizeof(number_str), "%d", num_diffuse++);
		} else if (strcmp(type_name, "texture_specular") == 0) {
			snprintf(number_str, sizeof(number_str), "%d", num_spec++);
		}
		
		char final_name[64] = "";
		strlcat(final_name, type_name, sizeof(final_name));
		strlcat(final_name, number_str, sizeof(final_name));
		
		//printf("[DEBUG] Final Shader Texture Uniform Name: %s\n", final_name);
		
		set_shader_int(shader, final_name, i);
		glBindTexture(GL_TEXTURE_2D, mesh->textures[i].loaded_tex);
	}	
	glActiveTexture(GL_TEXTURE0);
	
	glBindVertexArray(mesh->VAO);
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void draw_model(Model *model, Shader_Prog shader) {
	for (uint i = 0; i < model->meshes.size(); i++) {
		draw_mesh(&model->meshes[i], shader);
	}
}

static std::vector<MeshTexture> load_aimaterial_textures(Model *model, aiMaterial *mat, aiTextureType type, const char *type_name) {
	std::vector<MeshTexture> textures;
	
	for (uint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString texture_file_name;
		mat->GetTexture(type, i, &texture_file_name);
		
		size_t texture_path_len = strlen(model->mesh_file_dir) + strlen(texture_file_name.C_Str()) + 2;
		char *texture_path = (char*)calloc(1, texture_path_len);
		strlcat(texture_path, model->mesh_file_dir, texture_path_len);
		strlcat(texture_path, "/", texture_path_len);
		strlcat(texture_path, texture_file_name.C_Str(), texture_path_len);
		
		bool skip = false;
		for (uint j = 0; j < loaded_textures.size(); j++) {
			if (strcmp(loaded_textures[j].path, texture_path) == 0) {
				textures.push_back(loaded_textures[j]);
				skip = true;
				break;
			}
		} 
		
		printf("[DEBUG]: (%s) Texture Path for name (%s, Cached: %d): `%s`\n", texture_file_name.C_Str(), type_name, skip, texture_path);
		
		if (!skip) {
			MeshTexture texture;
			texture.loaded_tex = create_texture(texture_path);
			texture.type = type_name;
			texture.path = strdup(texture_path);
			textures.push_back(texture);
			loaded_textures.push_back(texture);
		}
		
		free(texture_path);
	}
	
	return textures;
}

static Mesh process_aimesh(Model *model, aiMesh *ai_mesh, const aiScene *scene) {
	std::vector <MeshTexture> textures;
	std::vector <Vertex> vertices;
	std::vector <uint> indicies;
	
	for (uint i = 0; i < ai_mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 data_puller;
		
		data_puller.x = ai_mesh->mVertices[i].x;
		data_puller.y = ai_mesh->mVertices[i].y;
		data_puller.z = ai_mesh->mVertices[i].z;
		vertex.position = data_puller;
		
		data_puller.x = ai_mesh->mNormals[i].x;
		data_puller.y = ai_mesh->mNormals[i].y;
		data_puller.z = ai_mesh->mNormals[i].z;
		vertex.normal = data_puller;
		
		vertex.tex_coord = glm::vec2();
		if (ai_mesh->mTextureCoords[0]) { // Contains UVs? else 0
			vertex.tex_coord.x = ai_mesh->mTextureCoords[0][i].x;
			vertex.tex_coord.y = ai_mesh->mTextureCoords[0][i].y;
		}
		
		vertices.push_back(vertex);
	}
	
	for (uint i = 0; i < ai_mesh->mNumFaces; i++) {
		aiFace face = ai_mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++) {
			indicies.push_back(face.mIndices[j]);
		}
	}
	
	if (ai_mesh->mMaterialIndex >= 0) { // Has Materials?
		aiMaterial *material = scene->mMaterials[ai_mesh->mMaterialIndex];
		
		std::vector<MeshTexture> diffuse_textures = load_aimaterial_textures(model, material, aiTextureType_DIFFUSE, "texture_diffuse");
		for (uint i = 0; i < diffuse_textures.size(); i++) {
			textures.push_back(diffuse_textures[i]);
		}
		
		std::vector<MeshTexture> specular_textures = load_aimaterial_textures(model, material, aiTextureType_SPECULAR, "specular_diffuse");
		for (uint i = 0; i < specular_textures.size(); i++) {
			textures.push_back(specular_textures[i]);
		}
	}
	
	Mesh final_product;
	final_product.vertices = vertices;
	final_product.indices = indicies;
	final_product.textures = textures;
	final_product.VAO = 0;
	final_product.VBO = 0;
	final_product.EBO = 0;
	
	return final_product;
}

static void process_ai_node(Model *model, aiNode *node, const aiScene *scene) {
	for (uint i = 0; i < node->mNumMeshes; i++) {
		aiMesh *ai_mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh our_mesh = process_aimesh(model, ai_mesh, scene);
		setup_mesh(&our_mesh);
		model->meshes.push_back(our_mesh);
	}
	
	for (uint i = 0; i < node->mNumChildren; i++) {
		printf("Child: %d\n", i);
		process_ai_node(model, node->mChildren[i], scene);
	}
}

int load_model(Model *model, const char *path) {
	// TODO(kay): (PERF) Look into aiProcess_OptimizeMeshes & aiProcess_SplitLargeMeshes
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	
	if (!scene) {
		fprintf(stderr, "[ERROR] ASSIMP ERROR! Path: %s, Error: %s\n", path, importer.GetErrorString());
		return -1;
	}
	
	model->mesh_file_dir = get_file_directory(path);
	
	process_ai_node(model, scene->mRootNode, scene);
	
	return 0;
}

void delete_model(Model *model) {
	free(model->mesh_file_dir);
}
