
/**
 * @copyright  Copyright (C) 2026 Kaden Slater. All Rights Reserved.
 * @license    GNU General Public License version 2 or later; see LICENSE.txt
 */

#include "display_utils.h"
#include "file_utils.h"


// TODO(kadenslater95): Brought this from my CHIP 8 emulator, need to update.
// X and Y go from -1 to 1 (so lengths of 2.0f) in OpenGL and the dimensions
// of Chip 8 screen are 64x32
#define PIXEL_W 2.0f / 64.0f
#define PIXEL_H 2.0f / 32.0f

// The point size I will give to OpenGL
#define POINT_S 10.0f

// This is for a block of bytes that will be held as a "display buffer"
// which I will iterate through and use to build up the Vertex Buffer.
// The display is bitmapped so divide 8 to go from bits to bytes
#define DISPLAY_SIZE 64 * 32 / 8


unsigned char displayBuffer[DISPLAY_SIZE];

unsigned int vertexShader;
unsigned int fragmentShader;

unsigned int shaderProgram;

unsigned int VAO, VBO, EBO;

float vertices[] = {
  -1.0f + PIXEL_W*0.5f + 0*PIXEL_W, 1.0f - PIXEL_H*0.5f - 0*PIXEL_W,
  -1.0f + PIXEL_W*0.5f + 1*PIXEL_W, 1.0f - PIXEL_H*0.5f - 1*PIXEL_H,

  -1.0f + PIXEL_W*0.5f + 63*PIXEL_W, 1.0f - PIXEL_H*0.5f - 0*PIXEL_H,

  -1.0f + PIXEL_W*0.5f + 63*PIXEL_W, 1.0f - PIXEL_H*0.5f - 31*PIXEL_H,

  -1.0f + PIXEL_W*0.5f + 0*PIXEL_W, 1.0f - PIXEL_H*0.5f - 31*PIXEL_H,
};

unsigned int indices[] = {
  0, 1, 2, 3, 4
};


void clear_display_buffer() {
  for (int i = 0; i < DISPLAY_SIZE; i++) {
    displayBuffer[i] = 0;
  }
}


// void read_display_buffer() {
  // Go through the display bitmap and BitwiseAND to check if I include this
  // position in the vertex buffer or not
  // Keep a count as I go so that I can know the correct size to allocate later

  // unsigned int onStateCount;
  // int positionState[64 * 32];

  // for(int i = 0; i < 64; i++) {
  //   for(int j = 0; j < 32; j++) {
  //     if(displayBuffer[64*i + 32*j / 8] & )
  //   }
  // }
// }


/**
 * Read the Shader Source Code from the given file path and add it to the given shader
 * 
 * @param shader unsigned int, the uint given by glCreateShader
 * @param filePath char*, the shader source file path
 * 
 * @return int, return 1 if error is caught and 0 otherwise
*/
int load_shader_from_file(unsigned int shader, char *filePath) {
  char* fileContent;
  unsigned int fileLength;

  read_file(filePath, &fileContent, &fileLength);

  if (!fileContent) {
    printf("Failed to read file!\n");
    return 1;
  }

  glShaderSource(shader, 1, (const GLchar * const*)&fileContent, NULL);

  free(fileContent);

  GLenum glError = glGetError();
  if (glError != GL_NO_ERROR) {
    printf("Failed to compile shader! GL ERROR: %s\n", gluErrorString(glError));
    return FALSE;
  }

  return 0;
}


void build_shader_program() {
  int  success;
  char infoLog[512];


  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  load_shader_from_file(vertexShader, "./shader.vert");
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    return;
  }


  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  load_shader_from_file(fragmentShader, "./shader.frag");
  glCompileShader(fragmentShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    return;
  }


  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);


  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      printf("ERROR::SHADER::PROGAM::LINKAGE_FAILED\n%s\n", infoLog);
      return;
  }


  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}


void on_realize(GtkGLArea *area) {
  // We need to make the context current if we want to
  // call GL API
  gtk_gl_area_make_current(area);

  GError *gl_area_error = gtk_gl_area_get_error(area);
  if (gl_area_error != NULL) {
    printf("Failed to create gtk_gl_area: %s\n", gl_area_error->message);
    g_error_free(gl_area_error);
    return;
  }


  GLenum err = glewInit();
  if (GLEW_OK != err) {
    // Problem: glewInit failed, something is seriously wrong
    fprintf(stderr, "GLEW Initialization Error: %s\n", glewGetErrorString(err));
  }


  GdkGLContext *context = gtk_gl_area_get_context(area);
  int major, minor;
  gdk_gl_context_get_version(context, &major, &minor);
  printf("Gtk GL Context Version: %d.%d\n", major, minor);

  printf("Using OpenGL Version: %s\n", glGetString(GL_VERSION));


  clear_display_buffer();

  build_shader_program();


  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    sizeof(indices),
    indices,
    GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glPointSize(POINT_S);


  GLenum glError = glGetError();
  if (glError != GL_NO_ERROR) {
    printf("GL ERROR: %s\n", gluErrorString(glError));
    return;
  }
}



gboolean render(GtkGLArea *area, GdkGLContext *context) {
  // inside this function it's safe to use GL; the given
  // GdkGLContext has been made current to the drawable
  // surface used by the `GtkGLArea` and the viewport has
  // already been set to be the size of the allocation

  // we can start by clearing the buffer
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(shaderProgram);

  glBindVertexArray(VAO);

  glDrawElements(GL_POINTS, 5, GL_UNSIGNED_INT, 0);

  glFlush();

  GLenum glError = glGetError();
  if (glError != GL_NO_ERROR) {
    printf("GL ERROR: %s\n", gluErrorString(glError));
    return FALSE;
  }

  // we completed our drawing; the draw commands will be
  // flushed at the end of the signal emission chain, and
  // the buffers will be drawn on the window
  return TRUE;
}
