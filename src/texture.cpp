#include "texture.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <glad/glad.h>
#include <stb/stb_image.h>

Texture create_texture(const char *file_path) {
	stbi_set_flip_vertically_on_load(1);
	
	int32_t width, height, number_chs;
	uint8_t *image_data = stbi_load(file_path, &width, &height, &number_chs, 0);
	
	if (!image_data) { 	
		fprintf(stderr, "ERROR Loading Image `%s`: %s\n", file_path, strerror(errno));
		return 0;
	}
	
	Texture texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	switch (number_chs) {
		case 3: {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
		} break;
		case 4: {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		} break;
		
		default: {
			fprintf(stderr, "Unknown channels for image. Path: `%s`\n", image_data);
			glDeleteTextures(1, &texture_id);
			
			return 0;
		}
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	
	stbi_image_free(image_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return texture_id;
}

void delete_texture(Texture texture) {
	glDeleteTextures(1, &texture);
}
