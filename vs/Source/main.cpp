#include "stdafx.h"

#include "Renderer.h"
#include "Window.h"

#include <windows.h>						// DWORD is undefined otherwise

// Laptop support. Use NVIDIA graphic card instead of Intel
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main()
{
    Tet::Window* window = Tet::Window::getInstance();
    Tet::Renderer* renderer = Tet::Renderer::getInstance();

    try
    {
        window->init("Tetrahedral meshing");
        window->loop();
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
    }

    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Finishing application..." << std::endl;

    // - Esta llamada es para impedir que la consola se cierre inmediatamente tras la
    // ejecución y poder leer los mensajes. Se puede usar también getChar();
    system("pause");
}


