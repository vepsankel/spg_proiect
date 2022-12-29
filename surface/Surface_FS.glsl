#version 330

in vec4 color;

out vec4 fragColor;  // Output color of the fragment

void main() {
  fragColor = color;// + vec4(0.5,0.5,0.5,1);
}