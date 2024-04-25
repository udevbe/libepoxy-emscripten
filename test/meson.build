dl_dep = cc.find_library('dl', required: false)
has_dlvsym = cc.has_function('dlvsym', dependencies: dl_dep)

has_gles1 = gles1_dep.found()
has_gles2 = gles2_dep.found()

test_cflags = common_cflags
if not has_dlvsym
test_cflags += [
  '-D_XOPEN_SOURCE',
  '-D_POSIX_C_SOURCE=200809L',
]
endif

# Unconditionally built tests
test('misc_defines',
     executable('misc defines', 'miscdefines.c',
                c_args: common_cflags,
                dependencies: libepoxy_dep,
                include_directories: libepoxy_inc))
test('khronos_typedefs',
     executable('khronos typedefs', [
                  'khronos_typedefs.c',
                  'khronos_typedefs.h',
                  'khronos_typedefs_nonepoxy.c',
                ],
                c_args: common_cflags,
                dependencies: libepoxy_dep,
                include_directories: libepoxy_inc))
