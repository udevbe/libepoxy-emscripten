# Configuration file
configure_file(output: 'config.h', configuration: conf)

# List of generated sources:
#   - name of the generated file
#   - registry source file
#   - additional sources
generated_sources = [
  [ 'gl_generated_dispatch.c', gl_registry, [ 'dispatch_common.c', 'dispatch_common.h', 'dispatch_emscripten.c' ] ],
]
generated_sources += [ [ 'egl_generated_dispatch.c', egl_registry, 'dispatch_egl.c' ] ]

gen_sources = [ ]
sources = [ ]

foreach g: generated_sources
  gen_source = g[0]
  registry = g[1]
  source = g[2]

  generated = custom_target(gen_source,
                            input: registry,
                            output: [ gen_source ],
                            command: [
                              gen_dispatch_py,
                              '--source',
                              '--no-header',
                              '--outputdir=@OUTDIR@',
                              '@INPUT@',
                            ])

  gen_sources += [ generated ]
  sources += [ source ]
endforeach

epoxy_sources = sources + gen_sources

common_ldflags = []

if host_system == 'linux' and cc.get_id() == 'gcc'
  common_ldflags += cc.get_supported_link_arguments([ '-Wl,-Bsymbolic-functions', '-Wl,-z,relro' ])
endif

# Maintain compatibility with autotools; see: https://github.com/anholt/libepoxy/issues/108
darwin_versions = [1, '1.0']

epoxy_deps = [ egl_headers_dep, ]


libepoxy = library(
  'epoxy',
  sources: epoxy_sources + epoxy_headers,
  version: '0.0.0',
  darwin_versions: darwin_versions,
  install: true,
  dependencies: epoxy_deps,
  include_directories: libepoxy_inc,
  c_args: common_cflags,
  link_args: common_ldflags,
)

epoxy_has_egl = '1'

libepoxy_dep = declare_dependency(
  link_with: libepoxy,
  include_directories: libepoxy_inc,
  dependencies: epoxy_deps,
  sources: epoxy_headers,
  variables: {
    'epoxy_has_egl': epoxy_has_egl,
  },
)
meson.override_dependency('epoxy', libepoxy_dep)

# We don't want to add these dependencies to the library, as they are
# not needed when building Epoxy; we do want to add them to the generated
# pkg-config file, for consumers of Epoxy
gl_reqs = []
gl_reqs += 'glesv2'
gl_reqs += 'egl'

pkg = import('pkgconfig')
pkg.generate(
  libraries: libepoxy,
  name: 'epoxy',
  description: 'GL dispatch library',
  version: meson.project_version(),
  variables: [
    'epoxy_has_egl=@0@'.format(epoxy_has_egl)
  ],
  filebase: 'epoxy',
  requires_private: ' '.join(gl_reqs),
)
