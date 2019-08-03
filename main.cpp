#define GLEW_STATIC

#include <GL/glew.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include "glfwwindow.h"

#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS


int main(int argc, char *argv[])
{
    GlfwWindow window;
    if(window.init() == -1)
    {
        std::cerr << "failed to intialise glwf window" << std::endl;
    }
    window.setEventHandling();

    // !!!
    // required to avoid crash on glGenVertexArays
    // does some magic...
    glewExperimental = GL_TRUE;
    // ¡¡¡

    GLenum err = glewInit();
    if(err != GLEW_NO_ERROR)
    {
        std::cerr << " An error occured when trying to initialize glew" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout<<glGetString(GL_VERSION)<<" | "<<glGetString(GL_VENDOR)<<std::endl;

    window.setupScene();
    window.show();

    return EXIT_SUCCESS;
}

