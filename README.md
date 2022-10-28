# Canvas

Small library to display 2D shapes based on [GLFW](https://github.com/glfw/glfw) and [ImGui](https://github.com/ocornut/imgui).

## Usage

*See [example/main.c](example/main.c) and [example/app.c](example/app.c) for a complete demonstration.*

```c
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <canvas.h>

#include "app.h"

int main()
{
    // Init glfw
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1000, 800, "Canvas", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Init canvas library
    CanvasConfig config = cvGetDefaultConfig();
    cvInit(window, config);

    // Main loop
    while (glfwWindowShouldClose(window) == GLFW_FALSE)
    {
        // Begin
        glfwPollEvents();
        cvNewFrame();

        cvSetCoordinateSystemFromScreenSpace(500, 400, 100, 0, 0, -100);
        // Draw
        cvAddLine(-1.f,-1.f, 1.f, 1.f, CV_COL32_WHITE);
        cvAddLine(-1.f, 1.f, 1.f,-1.f, CV_COL32_WHITE);
        
        // End
        cvEndFrame();
        glfwSwapBuffers(window);
    }

    // Shutdown
    cvShutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

```

## Integration

Copy **include** and **libs** directory inside your project. And link with `-lcanvas` and `-lglfw3`.

## Build example command

For Linux:
```bash
gcc example/app.c example/main.c -Iinclude -Llibs/x86_64-linux-gnu -lglfw3 -lcanvas -lstdc++ -ldl -lm -lpthread -o app
```

For Windows/cygwin:
```bash
gcc example/app.c example/main.c -Iinclude -Llibs/x86_64-pc-cygwin -lglfw3 -lcanvas -lstdc++ -lgdi32 -o app
```

For Windows/mingw:
```bash
gcc example/app.c example/main.c -Iinclude -Llibs/x86_64-w64-mingw32 -lglfw3 -lcanvas -lstdc++ -lgdi32 -o app
```

## Dependencies
- [GLFW](https://www.glfw.org/glfw3) (pre-built library provided)