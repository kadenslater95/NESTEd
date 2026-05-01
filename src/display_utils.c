
/**
 * @copyright  Copyright (C) 2026 Kaden Slater. All Rights Reserved.
 * @license    GNU General Public License version 2 or later; see LICENSE.txt
 */

#include "display_utils.h"
#include "file_utils.h"


#ifndef SHADER_DIR
#define SHADER_DIR "."
#endif

#define VERTEX_SHADER_PATH SHADER_DIR "/shader.vert"
#define FRAGMENT_SHADER_PATH SHADER_DIR "/shader.frag"


unsigned int vertexShader;
unsigned int fragmentShader;

unsigned int shaderProgram = 0;

unsigned int VAO, VBO, EBO;

float vertices[2*256*256];

unsigned int indices[2*256*256];


/**
 * Read the Shader Source Code from the given file path and add it to the given shader
 * 
 * @param shader unsigned int, the uint given by glCreateShader
 * @param filePath char*, the shader source file path
 * 
 * @return int, return 1 if error is caught and 0 otherwise
*/
int load_shader_from_file(unsigned int shader, const char *filePath) {
  char* fileContent;
  unsigned int fileLength;

  read_file(filePath, &fileContent, &fileLength);

  if (!fileContent) {
    printf("Failed to read file!\n");
    return 1;
  }

  GLint shaderLength = (GLint) fileLength;
  glShaderSource(shader, 1, (const GLchar * const*)&fileContent, &shaderLength);

  free(fileContent);

  GLenum glError = glGetError();
  if (glError != GL_NO_ERROR) {
    printf(
      "Failed to load shader source! GL ERROR: %s\n",
      gluErrorString(glError));
    return 1;
  }

  return 0;
}


int build_shader_program() {
  int  success;
  char infoLog[512];


  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  if (load_shader_from_file(vertexShader, VERTEX_SHADER_PATH) != 0) {
    return 1;
  }
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    return 1;
  }


  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  if (load_shader_from_file(fragmentShader, FRAGMENT_SHADER_PATH) != 0) {
    return 1;
  }
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    return 1;
  }


  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);


  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      printf("ERROR::SHADER::PROGAM::LINKAGE_FAILED\n%s\n", infoLog);
      shaderProgram = 0;
      return 1;
  }


  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return 0;
}


void on_realize(GtkGLArea *gl_area) {
  // We need to make the context current if we want to
  // call GL API
  gtk_gl_area_make_current(gl_area);

  GError *gl_area_error = gtk_gl_area_get_error(gl_area);
  if (gl_area_error != NULL) {
    printf("Failed to create gtk_gl_area: %s\n", gl_area_error->message);
    g_error_free(gl_area_error);
    return;
  }


  GdkGLContext *context = gtk_gl_area_get_context(gl_area);
  int major, minor;
  gdk_gl_context_get_version(context, &major, &minor);
  printf("Gtk GL Context Version: %d.%d\n", major, minor);

  printf("Using OpenGL Version: %s\n", glGetString(GL_VERSION));

  printf("Using GLEW Version: %s\n", glewGetString(GLEW_VERSION));
  GLenum err = glewInit();
  if (GLEW_OK != err && err != GLEW_ERROR_NO_GLX_DISPLAY) {
    // Problem: glewInit failed, something is seriously wrong
    fprintf(stderr, "GLEW Initialization Error: %s\n", glewGetErrorString(err));
    fprintf(stderr, "GLEW Error code: %d\n", err);
  }


  if (build_shader_program() != 0) {
    return;
  }


  int index;
  for(int i = 0; i < 256; i++) {
    for(int j = 0; j < 256; j++) {
      index = (256*i + j) * 2;
      vertices[index] = i;
      vertices[index + 1] = j;

      indices[index] = index;
      indices[index + 1] = index + 1;
    }
  }


  glUseProgram(shaderProgram);


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

  glVertexAttribPointer(
    0, 
    2, 
    GL_FLOAT, 
    GL_FALSE,
    2 * sizeof(float), 
    (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


  GLenum glError = glGetError();
  if (glError != GL_NO_ERROR) {
    printf("GL ERROR: %s\n", gluErrorString(glError));
    return;
  }
}



gboolean render(GtkGLArea *gl_area, GdkGLContext *context) {
  // inside this function it's safe to use GL; the given
  // GdkGLContext has been made current to the drawable
  // surface used by the `GtkGLArea` and the viewport has
  // already been set to be the size of the allocation

  // we can start by clearing the buffer
  glClear(GL_COLOR_BUFFER_BIT);

  if (shaderProgram == 0) {
    return FALSE;
  }

  glUseProgram(shaderProgram);


  int scale = gtk_widget_get_scale_factor(GTK_WIDGET(gl_area));
  int logical_width = gtk_widget_get_width(GTK_WIDGET(gl_area));
  int logical_height = gtk_widget_get_height(GTK_WIDGET(gl_area));
  float logical_PointSize = logical_width / 257.0f;

  // Set point size, but take ppi into account
  float frame_PointSize = logical_PointSize * scale;

  GLint uPointSize_loc = glGetUniformLocation(shaderProgram, "uPointSize");
  glUniform1f(uPointSize_loc, frame_PointSize);

  // Get viewport size so we can go from size X pixel to offset in NDC
  float frame_width = (float) logical_width * scale;
  float frame_height = (float) logical_height * scale;
  printf("\n\nfram_PointSize: %f\nframe_width: %f\nframe_height: %f\n\n", frame_PointSize, frame_width, frame_height);
  GLint uViewportSize_loc = glGetUniformLocation(shaderProgram, "uViewportSize");
  glUniform2f(uViewportSize_loc, frame_width, frame_height);


  glBindVertexArray(VAO);

  glDrawElements(
    GL_POINTS,
    sizeof(indices) / sizeof(indices[0]),
    GL_UNSIGNED_INT,
    0);

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
