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
          'src/native/bindings.cc',
          'src/native/nvenc.cc',
          'src/native/webgl.cc',
      ],
      'include_dirs': [
        "<!@(node -p \"require('node-addon-api').include\")",
        '/opt/cuda/targets/x86_64-linux/include',
        "/opt/angle"
      ],
      'libraries': [
        '-lEGL',
        '-lGLESv2',
        '-lcuda',
        '-lavformat',
        '-lavutil',
        '-lavcodec',
        '-ljpeg'
      ],
      'library_dirs': [
        "/opt/angle/angle/out/Release"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
