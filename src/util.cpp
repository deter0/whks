#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

char *read_entire_file(const char *file_path) {
	FILE *fp = fopen(file_path, "rb");
	if (!fp) {
		fprintf(stderr, "Something went wrong opening file (`%s`): %s\n", file_path, strerror(errno));
		return NULL;
	}	
	
	fseek(fp, 0L, SEEK_END);
	long file_size = ftell(fp);
	rewind(fp);
	
	char *file_buffer = (char*)malloc(file_size + 1);
	if (!file_buffer) {
		fprintf(stderr, "Error allocating buffer to read entire file at once. `%s` Size: %ld\n", file_path, file_size);
		fclose(fp);
		return 0;
	}
	
	if (fread(file_buffer, file_size, 1, fp) != 1) {
		fprintf(stderr, "Error reading file (`%s`): %s\n", file_path, strerror(errno));
		fclose(fp);
		free(file_buffer);
		return 0;
	}
	
	file_buffer[file_size] = 0;
	
	return file_buffer;
}

char *get_file_directory(const char *file_path) {
	uint path_last_slash_index = 0;
	for (size_t i = 0; i < strlen(file_path); i++) {
		if (file_path[i] == '/') {
			path_last_slash_index = i;
		}
	}
	char *directory = (char*)calloc(1, path_last_slash_index + 1);
	assert(directory != NULL);
	memcpy(directory, file_path, path_last_slash_index);
	
	return directory;
}


// Float Dynamic Array

float_dyn_array new_float_dyn_array(uint64_t initial_size) {
	float_dyn_array new_array = { 0 };
	
	float *float_buffer = (float*)calloc(initial_size > 0 ? initial_size : 512, sizeof(float));
	if (!float_buffer) {
		fprintf(stderr, "Error allocating dynamic float array of size: %zu. Crashing.\n", initial_size);
		assert(0);
	}
	
	new_array.data = float_buffer;
	new_array.allocated = initial_size > 0 ? initial_size : 512;
	
	return new_array;
}

void push_float_dyn_array(float_dyn_array *dyn_array, uint64_t num_values, float *values) {
	if (dyn_array->size + num_values > dyn_array->allocated) {
		uint64_t new_allocated = dyn_array->allocated * 2;
		
		dyn_array->data = (float*)realloc(dyn_array->data, new_allocated*sizeof(float));
		if (!dyn_array->data)  {
			fprintf(stderr, "Error resizing dynamic float array to size: %zu\n. Crashing.\n", new_allocated);
			assert(0);
		}
		dyn_array->allocated = new_allocated;
	}
	
	for (uint64_t i = 0; i < num_values; i++) {
		dyn_array->data[dyn_array->size + i] = values[i];
	}
	dyn_array->size += num_values;
}

void delete_float_dyn_array(float_dyn_array *dyn_array) {
	free(dyn_array->data);
	dyn_array->data = 0;
	dyn_array->size = 0;
	dyn_array->allocated = 0;
}


// Int Dynamic Array

int_dyn_array new_int_dyn_array(uint64_t initial_size) {
	int_dyn_array new_array = { 0 };
	
	int *int_buffer = (int*)calloc(initial_size > 0 ? initial_size : 512, sizeof(int));
	if (!int_buffer) {
		fprintf(stderr, "Error allocating dynamic int array of size: %zu. Crashing.\n", initial_size);
		assert(0);
	}
	
	new_array.data = int_buffer;
	new_array.allocated = initial_size > 0 ? initial_size : 512;
	
	return new_array;
}

void push_int_dyn_array(int_dyn_array *dyn_array, uint64_t num_values, int *values) {
	if (dyn_array->size + num_values > dyn_array->allocated) {
		uint64_t new_allocated = dyn_array->allocated * 2;
		
		dyn_array->data = (int*)realloc(dyn_array->data, new_allocated*sizeof(int));
		if (!dyn_array->data)  {
			fprintf(stderr, "Error resizing dynamic int array to size: %zu\n. Crashing.\n", new_allocated);
			assert(0);
		}
		dyn_array->allocated = new_allocated;
	}
	
	for (uint64_t i = 0; i < num_values; i++) {
		dyn_array->data[dyn_array->size++] = values[i];
	}
}

void delete_int_dyn_array(int_dyn_array *dyn_array) {
	free(dyn_array->data);
	dyn_array->data = 0;
	dyn_array->size = 0;
	dyn_array->allocated = 0;
}
