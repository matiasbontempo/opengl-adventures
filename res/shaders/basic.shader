#shader vertex
#version 410 core

// location = 0 is the same as the index in glVertexAttribPointer
// also, even though the buffer is a vec2, we can cast it to a vec4 for easier use
layout(location = 0) in vec4 position;

void main() {
  gl_Position = position;
};

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

void main() {
  color = vec4(1.0, 0.0, 0.0, 1.0);
};