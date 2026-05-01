#version 320 es
precision highp float;

layout (location = 0) in vec2 aPos;

uniform float uPointSize;
uniform vec2 uViewportSize;

void main() {
  vec2 normalized_pSize = vec2(uPointSize, uPointSize) / uViewportSize;

  float normalized_x = -1.0f + normalized_pSize.x + aPos.x*(2.0f * normalized_pSize.x);
  float normalized_y = 1.0f - normalized_pSize.y - aPos.y*(2.0f * normalized_pSize.y);

  gl_Position = vec4(normalized_x, normalized_y, 0.0f, 1.0f);
  gl_PointSize = uPointSize;
}