#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint32_t Texture;
Texture create_texture(const char *file_path);
void delete_texture(Texture texture);