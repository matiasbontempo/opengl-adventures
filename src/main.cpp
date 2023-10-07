#include <iostream>
#include <GLFW/glfw3.h>

int main() {
  std::cout << "Hello, World!" << std::endl;

  if (!glfwInit()) {
    // Handle initialization error
    return -1;
  }

  // Create a GLFW window
  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);

  if (!window) {
    // Handle window creation error
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

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
