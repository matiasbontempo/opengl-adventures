#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "Renderer.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

#include "Shader.h"
#include "Texture.h"

const int WIDTH = 800;
const int HEIGHT = 600;

int main() {  
  if (!glfwInit()) {
    std::cout << "GLFW initialization failed!" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set OpenGL version to 3.x
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set OpenGL version to x.3
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile

  // Create a GLFW window
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", nullptr, nullptr);

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
      100.0f, 100.0f, 0.0f, 0.0f, // 0
      200.0f, 100.0f, 1.0f, 0.0f, // 1
      200.0f, 200.0f, 1.0f, 1.0f, // 2
      100.0f, 200.0f, 0.0f, 1.0f // 3
    };

    unsigned int indices[] = {
      0, 1, 2,
      2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    
    VertexBufferLayout layout;
    layout.Push(GL_FLOAT, 2); // position
    layout.Push(GL_FLOAT, 2); // texture coordinates

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

    glm::mat4 mvp = proj * view * model;

    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
    shader.SetUniformMat4f("u_MVP", mvp);

    Texture texture("res/textures/test.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    // Unbind everything
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    
    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    float r = 0.0f;
    float increment = 0.05f;

    std::cout << "Entering main loop..." << std::endl;

    // Main loop until the window is closed
    while (!glfwWindowShouldClose(window)) {
      renderer.Clear();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      shader.Bind();
      shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

      renderer.Draw(va, ib, shader);

      if (r > 1.0f) {
        increment = -0.05f;
      } else if (r < 0.0f) {
        increment = 0.05f;
      }

      r += increment;

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  std::cout << "Terminating GLFW..." << std::endl;
  glfwTerminate();
  return 0;
}
