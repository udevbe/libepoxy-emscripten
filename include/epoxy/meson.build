headers = [ 'common.h' ]

generated_headers = [ [ 'gl.h', 'gl_generated.h', gl_registry ] ]
generated_headers += [ [ 'egl.h', 'egl_generated.h', egl_registry ] ]


gen_headers = []

foreach g: generated_headers
  header = g[0]
  gen_header = g[1]
  registry = g[2]
  generated = custom_target(gen_header,
                            input: registry,
                            output: [ gen_header ],
                            command: [
                              gen_dispatch_py,
                              '--header',
                              '--no-source',
                              '--outputdir=@OUTDIR@',
                              '@INPUT@',
                            ],
                            install: true,
                            install_dir: join_paths(epoxy_includedir, 'epoxy'))

  gen_headers += [ generated ]
  headers += [ header ]
endforeach

epoxy_headers = files(headers) + gen_headers

install_headers(headers, subdir: 'epoxy')
