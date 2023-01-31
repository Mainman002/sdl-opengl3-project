#include <SDL.h>
#include <glad/glad.h>

#include "Vertex_Shaders.hpp"
#include "Fragment_Shaders.hpp"

// STD
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

// DeltaTime STD
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <cstdint>
#include <thread>

// DeltaTime variables
static std::condition_variable cv;
static std::mutex             mut;
// static const int dt_max;

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

GLuint shaderObject;

GLuint CompileShader(GLuint type, const std::string& source)
{
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
    gGraphicsPiplineShaderProgram = CreateShaderProgram( Vertex_Shaders::vertex, Fragment_Shaders::vertex);
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
    // cpu vertex positions
    const std::vector<GLfloat> vertexData
    {
        // x,      y,     z
        -0.8f, -0.8f,  0.0f, // Left Position
         1.0f,  0.0f,  0.0f, // Color
         0.8f, -0.8f,  0.0f, // Right Position
         0.0f,  1.0f,  0.0f, // Color
         0.0f,  0.8f,  0.0f, // Top Position
         0.0f,  0.0f,  1.0f, // Color
    };

    // Setting things up for the gpu
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    // Generate VBO
    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertexData.size() * sizeof(GLfloat),
        vertexData.data(),
        GL_STATIC_DRAW);
    
    // Vertex Position Information
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 
        3, 
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT)*6,
        (GLvoid*)0);

    // Vertex Color Information
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 
        3, 
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT)*6,
        (GLvoid*)(sizeof(GL_FLOAT)*3));
    
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
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
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    
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
    using delta = std::chrono::duration<std::int64_t, std::ratio<1, 40>>;
    auto next = std::chrono::steady_clock::now() + delta{1};
    std::unique_lock<std::mutex> lk(mut);
    while (!gQuit)
    {
        mut.unlock();

        // Loop Game Code Start
        Input();
        PreDraw();
        Draw();

        // Update screen
        SDL_GL_SwapWindow(gGraphicsApplicationWindow);
        // Loop Game Code End

        // Wait for the next 1/40 sec
        mut.lock();
        cv.wait_until(lk, next, []{return false;});
        next += delta{1};
    }
}

void Cleanup()
{
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    InitializeProgram();
    VertexSpecification();
    CreateGraphicsPipeline();
    MainLoop();
    Cleanup();

    return 0;
}
