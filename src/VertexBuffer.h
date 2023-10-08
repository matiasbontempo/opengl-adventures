#pragma once

// Create a buffer to store the vertex data (positions)
// This is the buffer that will hold the vertex data

class VertexBuffer {
  private:
    unsigned int m_RendererID;

  public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};