#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Shader.h"

int main() {  
  if (!glfwInit()) {
    std::cout << "GLFW initialization failed!" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set OpenGL version to 3.x
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set OpenGL version to x.3
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile

  // Create a GLFW window
  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);

  if (!window) {
    std::cout << "GLFW window creation failed!" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync or sync with monitor refresh rate

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW initialization failed!" << std::endl;
    return -1;
  }

  // Log OpenGL version
  std::cout << glGetString(GL_VERSION) << std::endl;

  {
    float positions[] = {
      -0.5f, -0.5f, // 0
      0.5f, -0.5f, // 1
      0.5f,  0.5f, // 2
      -0.5f,  0.5f, // 3
    };

    unsigned int indices[] = {
      0, 1, 2,
      2, 3, 0
    };

    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    
    VertexBufferLayout layout;
    layout.Push(GL_FLOAT, 2);

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

    // Unbind everything
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    
    Renderer renderer;

    float r = 0.0f;
    float increment = 0.05f;

    std::cout << "Entering main loop..." << std::endl;

    // Main loop until the window is closed
    while (!glfwWindowShouldClose(window)) {
      renderer.Clear();

      shader.Bind();
      shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

      renderer.Draw(va, ib, shader);

      if (r > 1.0f) {
        increment = -0.05f;
      } else if (r < 0.0f) {
        increment = 0.05f;
      }

      r += increment;

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  std::cout << "Terminating GLFW..." << std::endl;
  glfwTerminate();
  return 0;
}
