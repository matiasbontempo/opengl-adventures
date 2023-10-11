#shader vertex
#version 410 core

// location = 0 is the same as the index in glVertexAttribPointer
// also, even though the buffer is a vec2, we can cast it to a vec4 for easier use
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main() {
  gl_Position = position;
  v_TexCoord = texCoord;
};

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {
  vec4 texColor = texture(u_Texture, v_TexCoord);
  color = texColor;
};