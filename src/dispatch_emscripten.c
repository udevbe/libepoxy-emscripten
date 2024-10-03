/***
 * Some functions that we need to implement, like getting pointers to
 * glGetString, etc.,
 * need to be done using the emscripten-provided OpenGL header.
 * This is not compatible with the epoxy OpenGL header, therefore it
 * must be done in a separate compilation unit.
 */

#include <epoxy/common.h>
#include <GL/gl.h>
#include <string.h>
#include <stdlib.h>
#include <EGL/egl.h>

EPOXY_PUBLIC void *
epoxy_em_bootstrap(const char * str) {
  if (0 == strcmp("glGetString", str)) {
    return &glGetString;
  }
  return &glGetIntegerv;
}

// maps to https://github.com/emscripten-core/emscripten/blob/4c34a0dbe3d0c2be1faa4bd6e287f46e4f479653/src/library_egl.js
EPOXY_PUBLIC void *
epoxy_em_eglGetProcAddress(const char * str) {
    if (strcmp(str, "eglGetDisplay") == 0) { return eglGetDisplay; }
    else if (strcmp(str, "eglInitialize") == 0) { return eglInitialize; }
    else if (strcmp(str, "eglTerminate") == 0) { return eglTerminate; }
    else if (strcmp(str, "eglGetConfigs") == 0) { return eglGetConfigs; }
    else if (strcmp(str, "eglChooseConfig") == 0) { return eglChooseConfig; }
    else if (strcmp(str, "eglGetConfigAttrib") == 0) { return eglGetConfigAttrib; }
    else if (strcmp(str, "eglCreateWindowSurface") == 0) { return eglCreateWindowSurface; }
    else if (strcmp(str, "eglDestroySurface") == 0) { return eglDestroySurface; }
    else if (strcmp(str, "eglCreateContext") == 0) { return eglCreateContext; }
    else if (strcmp(str, "eglDestroyContext") == 0) { return eglDestroyContext; }
    else if (strcmp(str, "eglQuerySurface") == 0) { return eglQuerySurface; }
    else if (strcmp(str, "eglQueryContext") == 0) { return eglQueryContext; }
    else if (strcmp(str, "eglGetError") == 0) { return eglGetError; }
    else if (strcmp(str, "eglQueryString") == 0) { return eglQueryString; }
    else if (strcmp(str, "eglBindAPI") == 0) { return eglBindAPI; }
    else if (strcmp(str, "eglQueryAPI") == 0) { return eglQueryAPI; }
    else if (strcmp(str, "eglWaitClient") == 0) { return eglWaitClient; }
    else if (strcmp(str, "eglWaitNative") == 0) { return eglWaitNative; }
    else if (strcmp(str, "eglWaitGL") == 0) { return eglWaitGL; }
    else if (strcmp(str, "eglSwapInterval") == 0) { return eglSwapInterval; }
    else if (strcmp(str, "eglMakeCurrent") == 0) { return eglMakeCurrent; }
    else if (strcmp(str, "eglGetCurrentContext") == 0) { return eglGetCurrentContext; }
    else if (strcmp(str, "eglGetCurrentSurface") == 0) { return eglGetCurrentSurface; }
    else if (strcmp(str, "eglGetCurrentDisplay") == 0) { return eglGetCurrentDisplay; }
    else if (strcmp(str, "eglSwapBuffers") == 0) { return eglSwapBuffers; }
    else if (strcmp(str, "eglReleaseThread") == 0) { return eglReleaseThread; }
    abort();
}
