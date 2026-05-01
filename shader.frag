#version 320 es
precision highp float;

in vec2 inPos;

out vec4 FragColor;

void main(){
  float red = inPos.x / 256.0f;
  float green = 0.0f;
  float blue = inPos.y / 256.0f;

  int intX = int(inPos.x);
  int intY = int(inPos.y);

  if(intX % 2 == 1 && intY % 2 == 1) {
    red = 0.0f;
    green = 1.0f;
    blue = 0.0f;
  }

  FragColor = vec4(red, green, blue, 1.0f);
}