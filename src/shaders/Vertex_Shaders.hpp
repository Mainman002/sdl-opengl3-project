#pragma once
#include <string>

namespace Vertex_Shaders
{
    std::string default_shader = R"(
        #version 330 core
            
        layout(location = 0) in vec4 position;

        void main()
        {
            gl_Position = vec4(position);
        }
    )";
};

