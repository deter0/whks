#pragma once

#include <stdint.h>
#include <vector>

#include <glm/glm.hpp>

#include "util.h"

typedef struct OBJ_vertex {
	float position[3];
	float vertex_normal[3];
	float tex_coords[2];
} OBJ_vertex;

typedef struct OBJ_Parse_Result {
	bool errored;
	
	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec3> out_vertex_normals;
	std::vector<glm::vec2> out_tex_coords;
} OBJ_Parse_Result;

OBJ_Parse_Result parse_obj_file(const char *file_path);
