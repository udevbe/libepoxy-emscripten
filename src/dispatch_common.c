/*
 * Copyright © 2013-2014 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/**
 * \mainpage Epoxy
 *
 * \section intro_sec Introduction
 *
 * Epoxy is a library for handling OpenGL function pointer management for
 * you.
 *
 * It hides the complexity of `dlopen()`, `dlsym()`, `glXGetProcAddress()`,
 * `eglGetProcAddress()`, etc. from the app developer, with very little
 * knowledge needed on their part.  They get to read GL specs and write
 * code using undecorated function names like `glCompileShader()`.
 *
 * Don't forget to check for your extensions or versions being present
 * before you use them, just like before!  We'll tell you what you forgot
 * to check for instead of just segfaulting, though.
 *
 * \section features_sec Features
 *
 *   - Automatically initializes as new GL functions are used.
 *   - GLES 1/2/3 context support.
 *   - Knows about function aliases so (e.g.) `glBufferData()` can be
 *     used with `GL_ARB_vertex_buffer_object` implementations, along
 *     with GL 1.5+ implementations.
 *   - EGL support.
 *   - Can be mixed with non-epoxy GL usage.
 *
 * \section using_sec Using Epoxy
 *
 * Using Epoxy should be as easy as replacing:
 *
 * ```cpp
 * #include <GL/gl.h>
 * #include <GL/glext.h>
 * ```
 *
 * with:
 *
 * ```cpp
 * #include <epoxy/gl.h>
 * ```
 *
 * \subsection using_include_sec Headers
 *
 * Epoxy comes with the following public headers:
 *
 *  - `epoxy/gl.h`  - For GL API
 *  - `epoxy/egl.h` - For EGL API
 *
 * \section links_sec Additional links
 *
 * The latest version of the Epoxy code is available on [GitHub](https://github.com/anholt/libepoxy).
 *
 * For bug reports and enhancements, please use the [Issues](https://github.com/anholt/libepoxy/issues)
 * link.
 *
 * The scope of this API reference does not include the documentation for
 * OpenGL and OpenGL ES. For more information on those programming interfaces
 * please visit:
 *
 *  - [Khronos](https://www.khronos.org/)
 *  - [OpenGL page on Khronos.org](https://www.khronos.org/opengl/)
 *  - [OpenGL ES page on Khronos.org](https://www.khronos.org/opengles/)
 *  - [docs.GL](http://docs.gl/)
 */

/**
 * @file dispatch_common.c
 *
 * @brief Implements common code shared by the generated GL/EGL dispatch code.
 *
 * A collection of some important specs on getting GL function pointers.
 *
 * From the linux GL ABI (http://www.opengl.org/registry/ABI/):
 *
 *     "3.4. The libraries must export all OpenGL 1.2, GLU 1.3, GLX 1.3, and
 *           ARB_multitexture entry points statically.
 *
 *      3.5. Because non-ARB extensions vary so widely and are constantly
 *           increasing in number, it's infeasible to require that they all be
 *           supported, and extensions can always be added to hardware drivers
 *           after the base link libraries are released. These drivers are
 *           dynamically loaded by libGL, so extensions not in the base
 *           library must also be obtained dynamically.
 *
 *      3.6. To perform the dynamic query, libGL also must export an entry
 *           point called
 *
 *           void (*glXGetProcAddressARB(const GLubyte *))(); 
 *
 *      The full specification of this function is available separately. It
 *      takes the string name of a GL or GLX entry point and returns a pointer
 *      to a function implementing that entry point. It is functionally
 *      identical to the wglGetProcAddress query defined by the Windows OpenGL
 *      library, except that the function pointers returned are context
 *      independent, unlike the WGL query."
 *
 * From the EGL 1.4 spec:
 *
 *    "Client API function pointers returned by eglGetProcAddress are
 *     independent of the display and the currently bound client API context,
 *     and may be used by any client API context which supports the extension.
 *
 *     eglGetProcAddress may be queried for all of the following functions:
 *
 *     • All EGL and client API extension functions supported by the
 *       implementation (whether those extensions are supported by the current
 *       client API context or not). This includes any mandatory OpenGL ES
 *       extensions.
 *
 *     eglGetProcAddress may not be queried for core (non-extension) functions
 *     in EGL or client APIs 20 .
 *
 *     For functions that are queryable with eglGetProcAddress,
 *     implementations may choose to also export those functions statically
 *     from the object libraries im- plementing those functions. However,
 *     portable clients cannot rely on this behavior.
 *
 * From the GLX 1.4 spec:
 *
 *     "glXGetProcAddress may be queried for all of the following functions:
 *
 *      • All GL and GLX extension functions supported by the implementation
 *        (whether those extensions are supported by the current context or
 *        not).
 *
 *      • All core (non-extension) functions in GL and GLX from version 1.0 up
 *        to and including the versions of those specifications supported by
 *        the implementation, as determined by glGetString(GL VERSION) and
 *        glXQueryVersion queries."
 */

#include <assert.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <err.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "dispatch_common.h"

/**
 * @brief Checks whether we're using OpenGL or OpenGL ES
 *
 * @return `true` if we're using OpenGL
 */
bool
epoxy_is_desktop_gl(void)
{
    return false;
}

/**
 * @brief Returns the version of OpenGL we are using
 *
 * The version is encoded as:
 *
 * ```
 *
 *   version = major * 10 + minor
 *
 * ```
 *
 * So it can be easily used for version comparisons.
 *
 * @return The encoded version of OpenGL we are using
 */
int
epoxy_gl_version(void)
{
    return 20;
}

/**
 * @brief Returns the version of the GL Shading Language we are using
 *
 * The version is encoded as:
 *
 * ```
 *
 *   version = major * 100 + minor
 *
 * ```
 *
 * So it can be easily used for version comparisons.
 *
 * @return The encoded version of the GL Shading Language we are using
 */
int
epoxy_glsl_version(void)
{
    return 20;
}

/**
 * @brief Checks for the presence of an extension in an OpenGL extension string
 *
 * @param extension_list The string containing the list of extensions to check
 * @param ext The name of the GL extension
 * @return `true` if the extension is available'
 *
 * @note If you are looking to check whether a normal GL, EGL or GLX extension
 * is supported by the client, this probably isn't the function you want.
 *
 * Some parts of the spec for OpenGL and friends will return an OpenGL formatted
 * extension string that is separate from the usual extension strings for the
 * spec. This function provides easy parsing of those strings.
 *
 * @see epoxy_has_gl_extension()
 * @see epoxy_has_egl_extension()
 */
bool
epoxy_extension_in_string(const char *extension_list, const char *ext)
{
    const char *ptr = extension_list;
    int len;

    if (!ext)
        return false;

    len = strlen(ext);

    if (extension_list == NULL || *extension_list == '\0')
        return false;

    /* Make sure that don't just find an extension with our name as a prefix. */
    while (true) {
        ptr = strstr(ptr, ext);
        if (!ptr)
            return false;

        if (ptr[len] == ' ' || ptr[len] == 0)
            return true;
        ptr += len;
    }
}

static bool
epoxy_internal_has_gl_extension(const char *ext, bool invalid_op_mode)
{
    if (epoxy_gl_version() < 30) {
        const char *exts = (const char *)glGetString(GL_EXTENSIONS);
        if (!exts)
            return invalid_op_mode;
        return epoxy_extension_in_string(exts, ext);
    } else {
        int num_extensions;
        int i;

        glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
        if (num_extensions == 0)
            return invalid_op_mode;

        for (i = 0; i < num_extensions; i++) {
            const char *gl_ext = (const char *)glGetStringi(GL_EXTENSIONS, i);
            if (!gl_ext)
                return false;
            if (strcmp(ext, gl_ext) == 0)
                return true;
        }

        return false;
    }
}

/**
 * @brief Returns true if the given GL extension is supported in the current context.
 *
 * @param ext The name of the GL extension
 * @return `true` if the extension is available
 *
 * @note that this function can't be called from within `glBegin()` and `glEnd()`.
 *
 * @see epoxy_has_egl_extension()
 */
bool
epoxy_has_gl_extension(const char *ext)
{
    return epoxy_internal_has_gl_extension(ext, false);
}
