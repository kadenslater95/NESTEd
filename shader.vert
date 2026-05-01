#version 320 es
precision highp float;

layout (location = 0) in vec2 aPos;

uniform float uPointSize;
uniform vec2 uViewportSize;

out vec2 inPos;

void main() {
  inPos = aPos;

  vec2 normalized_pSize = vec2(uPointSize, uPointSize) / uViewportSize;

  vec2 normalized_position = vec2(
    -1.0f + normalized_pSize.x + aPos.x*(2.0f * normalized_pSize.x),
    1.0f - normalized_pSize.y - aPos.y*(2.0f * normalized_pSize.y)
  );

  gl_Position = vec4(normalized_position.x, normalized_position.y, 0.0f, 1.0f);
  gl_PointSize = uPointSize;
}