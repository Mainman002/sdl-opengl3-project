#pragma once
#include <string>

namespace Fragment_Shaders
{
    std::string default_shader = R"(
        #version 330 core
            
        layout(location = 0) out vec4 color;

        void main()
        {
            color = vec4(0.0, 0.5, 0.5, 1.0);
        }
    )";

    std::string grey = R"(
        #version 330 core
            
        layout(location = 0) out vec4 color;

        void main()
        {
            color = vec4(0.7, 0.7, 0.8, 1.0);
        }
    )";

    std::string red = R"(
        #version 330 core
            
        layout(location = 0) out vec4 color;

        void main()
        {
            color = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";
};
