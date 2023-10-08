#pragma once

// Create a buffer to store the index data, this way we don't have to send the same vertex data multiple times
// This is the buffer that will hold the index data pointing to the vertex data

class IndexBuffer {
  private:
    unsigned int m_RendererID;
    unsigned int m_Count;

  public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return m_Count; };
};