project('libepoxy', 'c', version: '1.5.11',
        default_options: [
          'buildtype=debugoptimized',
          'c_std=gnu99',
          'warning_level=1',
        ],
        license: 'MIT',
        meson_version: '>= 0.54.0')

epoxy_version = meson.project_version().split('.')
epoxy_major_version = epoxy_version[0].to_int()
epoxy_minor_version = epoxy_version[1].to_int()
epoxy_micro_version = epoxy_version[2].to_int()

epoxy_prefix = get_option('prefix')
epoxy_libdir = join_paths(epoxy_prefix, get_option('libdir'))
epoxy_datadir = join_paths(epoxy_prefix, get_option('datadir'))
epoxy_includedir = join_paths(epoxy_prefix, get_option('includedir'))

cc = meson.get_compiler('c')
host_system = host_machine.system()

conf = configuration_data()
conf.set_quoted('PACKAGE_NAME', meson.project_name())
conf.set_quoted('PACKAGE_VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING', '@0@-@1@'.format(meson.project_name(), meson.project_version()))
conf.set_quoted('PACKAGE_DATADIR', join_paths(get_option('prefix'), get_option('datadir')))
conf.set_quoted('PACKAGE_LIBDIR', join_paths(get_option('prefix'), get_option('libdir')))
conf.set_quoted('PACKAGE_LOCALEDIR', join_paths(get_option('prefix'), get_option('datadir'), 'locale'))
conf.set_quoted('PACKAGE_LIBEXECDIR', join_paths(get_option('prefix'), get_option('libexecdir')))
conf.set('HAVE_KHRPLATFORM_H', cc.has_header('KHR/khrplatform.h'))

has_znow = true

test_cflags = [
  '-Wpointer-arith',
  '-Wmissing-declarations',
  '-Wformat=2',
  '-Wstrict-prototypes',
  '-Wmissing-prototypes',
  '-Wnested-externs',
  '-Wbad-function-cast',
  '-Wold-style-definition',
  '-Wdeclaration-after-statement',
  '-Wunused',
  '-Wuninitialized',
  '-Wshadow',
  '-Wmissing-noreturn',
  '-Wmissing-format-attribute',
  '-Wredundant-decls',
  '-Wlogical-op',
  '-Werror=implicit',
  '-Werror=nonnull',
  '-Werror=init-self',
  '-Werror=main',
  '-Werror=missing-braces',
  '-Werror=sequence-point',
  '-Werror=return-type',
  '-Werror=trigraphs',
  '-Werror=array-bounds',
  '-Werror=write-strings',
  '-Werror=address',
  '-Werror=int-to-pointer-cast',
  '-Werror=pointer-to-int-cast',
  '-fno-strict-aliasing',
  '-Wno-int-conversion',
]

common_cflags = cc.get_supported_arguments(test_cflags)
libtype = 'static'

# Dependencies
egl_dep = dependency('egl', required: true)
egl_headers_dep = egl_dep.partial_dependency(compile_args: true, includes: true)
gles2_dep = dependency('glesv2', required: true)

# Generates the dispatch tables
gen_dispatch_py = find_program('src/gen_dispatch.py')

gl_registry = files('registry/gl.xml')
egl_registry = files('registry/egl.xml')

libepoxy_inc = [
  include_directories('include'),
  include_directories('src'),
]

subdir('include/epoxy')
subdir('src')
