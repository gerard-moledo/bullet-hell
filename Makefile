#OBJS specifies which files to compile as part of the project
OBJS := $(patsubst ./Source/%.c, ./obj/%.o, $(wildcard ./Source/*.c))

INCLUDES = -I../../_Dev_Libs/Emscripten -I./Include $(patsubst %, -I%, $(wildcard ../../_Dev_Libs/SDL2_Emscripten/*/Include))

LIBS = $(patsubst %, -L%, $(wildcard ../../_Dev_Libs/SDL2_Emscripten/*/lib))

CC = emcc

ASSETS = ./Assets

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -O3 -DGPU_USE_INT_BOOL ${INCLUDES} 

LINKER_FLAGS = --profiling -O3 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH -s USE_SDL=2 -lSDL2_gpu  #-lmingw32 -lSDL2main -lSDL2 -lSDL2_gpu

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Bullet_Hell.html

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(LINKER_FLAGS) -o $(OBJ_NAME)

./obj/%.o : ./Source/%.c ./Include/globals.h
	$(CC) $< $(COMPILER_FLAGS) -c -o $@