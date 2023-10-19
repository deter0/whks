#pragma once

#include <stdlib.h>
#include <stdint.h>

#define ARR_SIZE(n) sizeof(n)/sizeof(n[0])

char *read_entire_file(const char *file_path);
char *get_file_directory(const char *file_path);

typedef struct float_dyn_array {
	uint64_t size;
	uint64_t allocated;
	float *data;
} float_dyn_array;


float_dyn_array new_float_dyn_array(uint64_t initial_size);
void push_float_dyn_array(float_dyn_array *dyn_array, uint64_t num_values, float *values);
void delete_float_dyn_array(float_dyn_array *dyn_array);

typedef struct int_dyn_array {
	uint64_t size;
	uint64_t allocated;
	int *data;
} int_dyn_array;


int_dyn_array new_int_dyn_array(uint64_t initial_size);
void push_int_dyn_array(int_dyn_array *dyn_array, uint64_t num_values, int *values);
void delete_int_dyn_array(int_dyn_array *dyn_array);

