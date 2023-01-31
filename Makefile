GX:= g++
FLAGS:= -std=c++17 -Wall
NAME:= SDL_GL
TYPE:= .exe
INCLUDES:= -Isrc/include -IC:/Dev/SDL2/include -IC:/Dev/Glad/include -Isrc/shaders
LIBS:= -LC:/Dev/SDL2/lib/x64
OUT:= build/${NAME}.exe C:/Dev/Glad/src/glad.c
FILES:= src/main.cpp
LINKS:= -lSDL2main -lSDL2

default:
	make -j4 build_exe && make run

build_exe:
	${GX} ${FLAGS} ${INCLUDES} ${LIBS} ${FILES} -o ${OUT} ${LINKS}

run:
	./${OUT}
