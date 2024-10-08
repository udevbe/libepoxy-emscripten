/*
 * Copyright © 2013 Intel Corporation
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

#include "config.h"
#include "epoxy/gl.h"
#include "epoxy/egl.h"


#define PACKED __attribute__((__packed__))
#define ENDPACKED

#define UNWRAPPED_PROTO(x) (GLAPIENTRY *x)
#define WRAPPER_VISIBILITY(type) static type GLAPIENTRY
#define WRAPPER(x) x ## _wrapped

#define GEN_GLOBAL_REWRITE_PTR(name, args, passthrough)          \
    static void EPOXY_CALLSPEC                                   \
    name##_global_rewrite_ptr args                               \
    {                                                            \
        if (name == (void *)name##_global_rewrite_ptr) {         \
            name = (void *)name##_resolver();                    \
        }                                                        \
        name passthrough;                                        \
    }

#define GEN_GLOBAL_REWRITE_PTR_RET(ret, name, args, passthrough) \
    static ret EPOXY_CALLSPEC                                    \
    name##_global_rewrite_ptr args                               \
    {                                                            \
        if (name == (void *)name##_global_rewrite_ptr) {         \
            name = (void *)name##_resolver();                    \
        }                                                        \
        return name passthrough;                                 \
    }


#define GEN_DISPATCH_TABLE_REWRITE_PTR(name, args, passthrough)
#define GEN_DISPATCH_TABLE_REWRITE_PTR_RET(ret, name, args, passthrough)
#define GEN_DISPATCH_TABLE_THUNK(name, args, passthrough)
#define GEN_DISPATCH_TABLE_THUNK_RET(ret, name, args, passthrough)

#define GEN_THUNKS(name, args, passthrough)                          \
    GEN_GLOBAL_REWRITE_PTR(name, args, passthrough)                  \
    GEN_DISPATCH_TABLE_REWRITE_PTR(name, args, passthrough)          \
    GEN_DISPATCH_TABLE_THUNK(name, args, passthrough)

#define GEN_THUNKS_RET(ret, name, args, passthrough)                 \
    GEN_GLOBAL_REWRITE_PTR_RET(ret, name, args, passthrough)         \
    GEN_DISPATCH_TABLE_REWRITE_PTR_RET(ret, name, args, passthrough) \
    GEN_DISPATCH_TABLE_THUNK_RET(ret, name, args, passthrough)

// "bootstrap" functions
void * epoxy_em_bootstrap(const char * name);
void * epoxy_em_eglGetProcAddress(const char * str);
