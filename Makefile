#OBJS specifies which files to compile as part of the project
OBJS := $(wildcard ./Source/*.c)

INCLUDES = -I../../_Dev_Libs/Emscripten -I./Include $(pathsubst %, -I%, $(wildcard ../../_Dev_Libs/SDL2_MinGW/*/Include))

LIBS = $(wildcard -L../../_Dev_Libs/SDL2_MinGW/*/x64)

CC = emcc

ASSETS = ./Assets

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wno-narrowing ${INCLUDES} ${LIBS}

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Asteroids.html

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) -O3 $(OBJS) $(COMPILER_FLAGS) -s USE_SDL=2 -s USE_WEBGL2=1 -o ${OBJ_NAME}

$(OBJS) : ./Include/globals.h