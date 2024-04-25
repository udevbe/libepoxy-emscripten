[![License: MIT](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT)

Epoxy-emscripoten is a library for handling OpenGL function pointer management for
you. It simplified to only support GLES2/3 and EGL for use with Emscripten.

Features
--------

  * Automatically initializes as new GL functions are used.
  * GLES 1/2/3 context support.
  * Knows about function aliases so (e.g.) `glBufferData()` can be
    used with `GL_ARB_vertex_buffer_object` implementations, along
    with GL 1.5+ implementations.
  * EGL support.
  * Can be mixed with non-epoxy GL usage.

Building
--------

```sh
mkdir _build && cd _build
meson
ninja
sudo ninja install
```

Dependencies for Debian:

  * meson
  * libegl1-mesa-dev

Dependencies for macOS (using MacPorts): 

  * pkgconfig
  * meson

The test suite has additional dependencies depending on the platform.
(EGL, a running X Server).

Switching your code to using epoxy
----------------------------------

It should be as easy as replacing:

```cpp
#include <GL/gl.h>
#include <GL/glext.h>
```

with:

```cpp
#include <epoxy/gl.h>
```

As long as epoxy's headers appear first, you should be ready to go.
Additionally, some new helpers become available, so you don't have to
write them:

`int epoxy_gl_version()` returns the GL version:

  * 12 for GL 1.2
  * 20 for GL 2.0
  * 44 for GL 4.4

`bool epoxy_has_gl_extension()` returns whether a GL extension is
available (`GL_ARB_texture_buffer_object`, for example).

Note that this is not terribly fast, so keep it out of your hot paths,
ok?
