#include <iostream>

#include "Renderer.h"
#include "VertexArray.h"

VertexArray::VertexArray() {
  std::cout << "Creating vertex array object..." << std::endl;
  GLCall(glGenVertexArrays(1, &m_RendererID));
};

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &m_RendererID);
};

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
  Bind();
  vb.Bind();

  const auto& elements = layout.GetElements();
  unsigned int offset = 0;
  
  std::cout << "Creating vertex buffer layout..." << std::endl;
  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto& element = elements[i];
    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)(uintptr_t)offset));
    offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
  }
};

void VertexArray::Bind() const {
  GLCall(glBindVertexArray(m_RendererID));
};

void VertexArray::Unbind() const {
  GLCall(glBindVertexArray(0));
};
