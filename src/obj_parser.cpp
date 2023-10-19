#include "obj_parser.hpp"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

OBJ_Parse_Result parse_obj_file(const char *file_path) {
	FILE *fp = fopen(file_path, "rb");
	
	if (!fp) {
		fprintf(stderr, "Error opening OBJ File '%s': %s.\n", file_path, strerror(errno));
	}

	std::vector<int> vertex_indices, uv_indices, normal_indices;
	std::vector<glm::vec3> temp_verts;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> temp_uvs;
	
	OBJ_Parse_Result result;

	char *line = 0;
	size_t len = 0;
	ssize_t read;

	while ((read = getline(&line, &len, fp)) != -1) {
		char obj_command[128] = { 0 };
		int res = sscanf(line, "%127s", (char*)&obj_command);
		
		if (res == EOF) {
			perror("Unexpected EOF in OBJ parsing.\n");
			exit(EXIT_FAILURE);
		}
		
		// TODO(kay): Optimization: Check if switch'ing the first character is faster than strcmp.
		if (strcmp(obj_command, "#") == 0) {
			printf("OBJ File Comment.\n");
			goto LineClear;
		} else if (strcmp(obj_command, "v") == 0) {
			glm::vec3 vertex;
			sscanf(line, "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);			
#ifdef OBJ_DEBUG
			printf("::VERTEX:: Line: %s, Read: %f %f %f\n", line, vertex.x, vertex.y, vertex.z);
#endif
			temp_verts.push_back(vertex);
		} else if (strcmp(obj_command, "vt") == 0) {
			glm::vec2 uv;
			sscanf(line, "vt %f %f\n", &uv.x, &uv.y);	
#ifdef OBJ_DEBUG
			printf("::UVs:: Line: %s, Read: %f %f\n", line, uv.x, uv.y);
#endif
			temp_uvs.push_back(uv);
		} else if (strcmp(obj_command, "vn") == 0) {
			glm::vec3 vertex_norm;
			sscanf(line, "vn %f %f %f\n", &vertex_norm.x, &vertex_norm.y, &vertex_norm.z);
#ifdef OBJ_DEBUG
			printf("::VERTEX NRMS:: Line: %s, Read: %f %f %f\n", line, vertex_norm.x, vertex_norm.y, vertex_norm.z);
#endif
			temp_normals.push_back(vertex_norm);
		} else if (strcmp(obj_command, "f") == 0) {
			int vertex_index[3], uv_index[3], normal_index[3];
			int matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex_index[0], &vertex_index[1], &vertex_index[2], &uv_index[0], &uv_index[1], &uv_index[2], &normal_index[0], &normal_index[1], &normal_index[2]);
			
			if (matches != 9) {
				fprintf(stderr, "Face entry too not supported. Try other export options. Line: '%s'\n", line);
				
				result.errored = 1;
				return result;
			}
			
			vertex_indices.push_back(vertex_index[0]);
			vertex_indices.push_back(vertex_index[1]);
			vertex_indices.push_back(vertex_index[2]);
			uv_indices.push_back(uv_index[0]);
			uv_indices.push_back(uv_index[1]);
			uv_indices.push_back(uv_index[2]);
			normal_indices.push_back(normal_index[0]);
			normal_indices.push_back(normal_index[1]);
			normal_indices.push_back(normal_index[2]);
		}
		
		LineClear:
		if (line) {
			free(line);
			line = 0;
		}
	}
	
	for (uint32_t i = 0; i < vertex_indices.size(); i++) {
		int vertex_index = vertex_indices[i];
		
		glm::vec3 vertex = temp_verts[vertex_index - 1];
		result.out_vertices.push_back(vertex);
	}
	for (uint32_t i = 0; i < uv_indices.size(); i++) {
		int uv_index = uv_indices[i];
		
		glm::vec2 uv = temp_uvs[uv_index - 1];
		result.out_tex_coords.push_back(uv);
	}
	for (uint32_t i = 0; i < uv_indices.size(); i++) {
		int vn_index = normal_indices[i];
		
		glm::vec3 vertex_normal = temp_normals[vn_index - 1];
		result.out_vertex_normals.push_back(vertex_normal);
	}

	return result;
}
