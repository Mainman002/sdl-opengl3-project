GX:= g++
FLAGS:= -std=c++17 -Wall -O3 -W -s -mwindows
NAME:= SDL_GL
TYPE:= .exe
INCLUDES:= -Isrc/include -IC:/Dev/SDL2/include -IC:/Dev/Glad/include -Isrc/shaders
LIBS:= -LC:/Dev/SDL2/lib/x64
OUT:= build/${NAME}
FILES:= src/main.cpp C:/Dev/Glad/src/glad.c
LINKS:= -lSDL2main -lSDL2

default:
	make build && make run

build:
	${GX} ${FLAGS} ${INCLUDES} ${LIBS} ${FILES} -o ${OUT} ${LINKS}

run:
	./${OUT}
