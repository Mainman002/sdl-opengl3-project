#include <SDL.h>
#include <glad/glad.h>

#include "Vertex_Shaders.hpp"
#include "Fragment_Shaders.hpp"

// STD
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

// Globals
int gScreenWidth = 1920*0.25;
int gScreenHeight = 1080*0.25;
SDL_Window*   gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;
bool gQuit = false;

// VAO
GLuint gVertexArrayObject = 0;

// VBO
GLuint gVertexBufferObject = 0;

// Program objects for our shaders
GLuint gGraphicsPiplineShaderProgram = 0;

GLuint CompileShader(GLuint type, const std::string& source)
{
    GLuint shaderObject;
    if(type == GL_VERTEX_SHADER)
    {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    } else if (type == GL_FRAGMENT_SHADER)
    {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void CreateGraphicsPipeline()
{
    gGraphicsPiplineShaderProgram = CreateShaderProgram( Vertex_Shaders::default_shader, Fragment_Shaders::grey);
}

void GetOpenGLVersionInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void VertexSpecification()
{
    // cpu vertex points
    const std::vector<GLfloat> vertexPosition
    {
        // x,      y,    z
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f
    };

    // Setting things up for the gpu
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    // Generate VBO
    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexArrayObject);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertexPosition.size() * sizeof(GLfloat),
        vertexPosition.data(),
        GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 
        3, 
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0);
    
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}

void InitializeProgram()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL2 could not Init" << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    gGraphicsApplicationWindow = SDL_CreateWindow(
        "SDL Opengl", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        gScreenWidth, gScreenHeight, 
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_RENDERER_ACCELERATED);
    
    
    if (gGraphicsApplicationWindow==nullptr)
    {
        std::cout << "SDL2 window could not be created" << std::endl;
        exit(1);
    }

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);

    if (gOpenGLContext==nullptr)
    {
        std::cout << "OpengGL Context not available" << std::endl;
        exit(1);
    }

    // Initialize the Glad Library
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "Glad was not initialized" << std::endl;
        exit(1);
    }

    GetOpenGLVersionInfo();
}

void Input()
{
    SDL_Event e;
    while(SDL_PollEvent(&e) !=0)
    {
        if (e.type == SDL_QUIT)
        {
            std::cout<< "Quit Window" << std::endl;
            gQuit = true;
        }
    }
}

void PreDraw()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPiplineShaderProgram);
}

void Draw()
{
    glBindVertexArray(gVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MainLoop()
{
    while(!gQuit)
    {
        Input();
        PreDraw();
        Draw();

        // Update screen
        SDL_GL_SwapWindow(gGraphicsApplicationWindow);
    } 
}

void Cleanup()
{
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    InitializeProgram();
    VertexSpecification();
    CreateGraphicsPipeline();
    MainLoop();
    Cleanup();

    return 0;
}
