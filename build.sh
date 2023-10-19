#!/bin/sh

DEPS="glfw3 cglm assimp"

LIBS=`pkg-config --libs $DEPS`
CFLAGS="-Wall -O2 -ggdb" `pkg-config --cflags $DEPS`
INC="-I./glad/include -I./include"

set -xe

SRC="src/main.cpp src/opengl_buffers.cpp src/texture.cpp src/util.cpp src/shader.cpp src/mesh.cpp 	glad/src/glad.c"

g++ $CFLAGS $INC $SRC -o whks $LIBS -lm
