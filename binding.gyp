{
  'targets': [
    {
      'target_name': 'webgl',
      'defines': [
        'VERSION=1.0.0'
        ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      'sources': [
          'src/native/decode.cc',
          'src/native/bindings.cc',
          'src/native/nvenc.cc',
          'src/native/webgl.cc',
      ],
      'include_dirs': [
        "<!@(node -p \"require('node-addon-api').include\")",
        '/opt/cuda/targets/x86_64-linux/include',
        "/usr/local/cuda-10.2/targets/x86_64-linux/include",
        "/home/alex_vestin/ffmpeg_build/include/",
        "/ffmpeg-build-static-binaries/include/",
        "/usr/local/include",
        "/home/alex/test-node/",
        "/home/alex/test-node/include_temp",
      ],
      'libraries': [
        '-lEGL',
        '-lGLESv2',
        '-lcuda',
        '-lavformat',
        '-lavutil',
        '-lavcodec',
        '-lswscale',
        '-lswresample',
        '-ljpeg',
        '-lglfw'
      ],
      'library_dirs': [
        "/opt/angle/angle/out/Release",
        "/home/alex_vestin/ffmpeg_build/lib/",
        "/ffmpeg-build-static-binaries/lib",
        "/usr/local/lib",

      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
