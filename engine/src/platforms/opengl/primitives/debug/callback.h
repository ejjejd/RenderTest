#pragma once
#include "GL/glew.h"

void GLAPIENTRY MessageCallbackOGL(GLenum source, GLenum type, GLuint id, GLenum severity,
                                    GLsizei length, const GLchar* message, const void* userParam);