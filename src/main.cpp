#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() {  
  if (!glfwInit()) {
    std::cout << "GLFW initialization failed!" << std::endl;
    return -1;
  }

  // Create a GLFW window
  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);

  if (!window) {
    std::cout << "GLFW window creation failed!" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW initialization failed!" << std::endl;
    return -1;
  }

  // Actual OpenGL code starts here
  std::cout << glGetString(GL_VERSION) << std::endl;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Render your OpenGL content here
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f( 0.0f,  0.5f);
    glVertex2f( 0.5f, -0.5f);
    glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
