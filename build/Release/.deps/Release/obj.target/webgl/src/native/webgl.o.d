cmd_Release/obj.target/webgl/src/native/webgl.o := g++ '-DNODE_GYP_MODULE_NAME=webgl' '-DUSING_UV_SHARED=1' '-DUSING_V8_SHARED=1' '-DV8_DEPRECATION_WARNINGS=1' '-DV8_DEPRECATION_WARNINGS' '-DV8_IMMINENT_DEPRECATION_WARNINGS' '-D_LARGEFILE_SOURCE' '-D_FILE_OFFSET_BITS=64' '-D__STDC_FORMAT_MACROS' '-DOPENSSL_NO_PINSHARED' '-DOPENSSL_THREADS' '-DNAPI_DISABLE_CPP_EXCEPTIONS' '-DBUILDING_NODE_EXTENSION' -I/home/alex_vestin/.cache/node-gyp/12.16.1/include/node -I/home/alex_vestin/.cache/node-gyp/12.16.1/src -I/home/alex_vestin/.cache/node-gyp/12.16.1/deps/openssl/config -I/home/alex_vestin/.cache/node-gyp/12.16.1/deps/openssl/openssl/include -I/home/alex_vestin/.cache/node-gyp/12.16.1/deps/uv/include -I/home/alex_vestin/.cache/node-gyp/12.16.1/deps/zlib -I/home/alex_vestin/.cache/node-gyp/12.16.1/deps/v8/include -I/home/alex_vestin/coding-server/webgl2/node_modules/node-addon-api -I/opt/cuda/targets/x86_64-linux/include -I/opt/angle -I/opt/angle/angle/include -I/usr/local/cuda-10.2/targets/x86_64-linux/include -I/home/alex_vestin/ffmpeg_build/include  -fPIC -pthread -Wall -Wextra -Wno-unused-parameter -m64 -O3 -fno-omit-frame-pointer -fno-rtti -std=gnu++1y -MMD -MF ./Release/.deps/Release/obj.target/webgl/src/native/webgl.o.d.raw   -c -o Release/obj.target/webgl/src/native/webgl.o ../src/native/webgl.cc
Release/obj.target/webgl/src/native/webgl.o: ../src/native/webgl.cc \
 ../src/native/webgl.h \
 /home/alex_vestin/coding-server/webgl2/node_modules/node-addon-api/napi.h \
 /home/alex_vestin/.cache/node-gyp/12.16.1/include/node/node_api.h \
 /home/alex_vestin/.cache/node-gyp/12.16.1/include/node/js_native_api.h \
 /home/alex_vestin/.cache/node-gyp/12.16.1/include/node/js_native_api_types.h \
 /home/alex_vestin/.cache/node-gyp/12.16.1/include/node/node_api_types.h \
 /home/alex_vestin/coding-server/webgl2/node_modules/node-addon-api/napi-inl.h \
 /home/alex_vestin/coding-server/webgl2/node_modules/node-addon-api/napi-inl.deprecated.h \
 /opt/angle/angle/include/EGL/egl.h \
 /opt/angle/angle/include/EGL/eglplatform.h \
 /opt/angle/angle/include/KHR/khrplatform.h \
 /opt/angle/angle/include/EGL/eglext.h \
 /opt/angle/angle/include/EGL/eglext_angle.h \
 /opt/angle/angle/include/GLES2/gl2.h \
 /opt/angle/angle/include/GLES2/gl2platform.h \
 /opt/angle/angle/include/GLES2/gl2ext.h \
 /opt/angle/angle/include/GLES2/gl2ext_angle.h \
 /opt/angle/angle/include/GLES2/gl2ext_explicit_context_autogen.inc \
 /opt/angle/angle/include/GLES2/../GLES3/gl3ext_explicit_context_autogen.inc \
 /opt/angle/angle/include/GLES2/../GLES3/gl31ext_explicit_context_autogen.inc \
 /opt/angle/angle/include/GLES3/gl32.h \
 /opt/angle/angle/include/GLES3/gl3platform.h \
 /usr/local/cuda-10.2/targets/x86_64-linux/include/cuda.h \
 /usr/local/cuda-10.2/targets/x86_64-linux/include/cudaGL.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/hwcontext.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/buffer.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/frame.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/avutil.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/common.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/attributes.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/macros.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/version.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/avconfig.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/mem.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/error.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/rational.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/mathematics.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/intfloat.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/log.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/pixfmt.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/dict.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/samplefmt.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/hwcontext_cuda.h \
 /home/alex_vestin/ffmpeg_build/include/libavformat/avformat.h \
 /home/alex_vestin/ffmpeg_build/include/libavcodec/avcodec.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/samplefmt.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/attributes.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/avutil.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/buffer.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/cpu.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/channel_layout.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/dict.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/frame.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/log.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/pixfmt.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/rational.h \
 /home/alex_vestin/ffmpeg_build/include/libavcodec/version.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/version.h \
 /home/alex_vestin/ffmpeg_build/include/libavformat/avio.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/common.h \
 /home/alex_vestin/ffmpeg_build/include/libavformat/version.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/error.h \
 /home/alex_vestin/ffmpeg_build/include/libavutil/opt.h \
 /home/alex_vestin/ffmpeg_build/include/libswresample/swresample.h \
 /home/alex_vestin/ffmpeg_build/include/libswresample/version.h \
 /home/alex_vestin/ffmpeg_build/include/libswscale/swscale.h \
 /home/alex_vestin/ffmpeg_build/include/libswscale/version.h \
 ../src/native/extra.h
../src/native/webgl.cc:
../src/native/webgl.h:
/home/alex_vestin/coding-server/webgl2/node_modules/node-addon-api/napi.h:
/home/alex_vestin/.cache/node-gyp/12.16.1/include/node/node_api.h:
/home/alex_vestin/.cache/node-gyp/12.16.1/include/node/js_native_api.h:
/home/alex_vestin/.cache/node-gyp/12.16.1/include/node/js_native_api_types.h:
/home/alex_vestin/.cache/node-gyp/12.16.1/include/node/node_api_types.h:
/home/alex_vestin/coding-server/webgl2/node_modules/node-addon-api/napi-inl.h:
/home/alex_vestin/coding-server/webgl2/node_modules/node-addon-api/napi-inl.deprecated.h:
/opt/angle/angle/include/EGL/egl.h:
/opt/angle/angle/include/EGL/eglplatform.h:
/opt/angle/angle/include/KHR/khrplatform.h:
/opt/angle/angle/include/EGL/eglext.h:
/opt/angle/angle/include/EGL/eglext_angle.h:
/opt/angle/angle/include/GLES2/gl2.h:
/opt/angle/angle/include/GLES2/gl2platform.h:
/opt/angle/angle/include/GLES2/gl2ext.h:
/opt/angle/angle/include/GLES2/gl2ext_angle.h:
/opt/angle/angle/include/GLES2/gl2ext_explicit_context_autogen.inc:
/opt/angle/angle/include/GLES2/../GLES3/gl3ext_explicit_context_autogen.inc:
/opt/angle/angle/include/GLES2/../GLES3/gl31ext_explicit_context_autogen.inc:
/opt/angle/angle/include/GLES3/gl32.h:
/opt/angle/angle/include/GLES3/gl3platform.h:
/usr/local/cuda-10.2/targets/x86_64-linux/include/cuda.h:
/usr/local/cuda-10.2/targets/x86_64-linux/include/cudaGL.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/hwcontext.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/buffer.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/frame.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/avutil.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/common.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/attributes.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/macros.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/version.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/avconfig.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/mem.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/error.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/rational.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/mathematics.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/intfloat.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/log.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/pixfmt.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/dict.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/samplefmt.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/hwcontext_cuda.h:
/home/alex_vestin/ffmpeg_build/include/libavformat/avformat.h:
/home/alex_vestin/ffmpeg_build/include/libavcodec/avcodec.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/samplefmt.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/attributes.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/avutil.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/buffer.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/cpu.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/channel_layout.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/dict.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/frame.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/log.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/pixfmt.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/rational.h:
/home/alex_vestin/ffmpeg_build/include/libavcodec/version.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/version.h:
/home/alex_vestin/ffmpeg_build/include/libavformat/avio.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/common.h:
/home/alex_vestin/ffmpeg_build/include/libavformat/version.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/error.h:
/home/alex_vestin/ffmpeg_build/include/libavutil/opt.h:
/home/alex_vestin/ffmpeg_build/include/libswresample/swresample.h:
/home/alex_vestin/ffmpeg_build/include/libswresample/version.h:
/home/alex_vestin/ffmpeg_build/include/libswscale/swscale.h:
/home/alex_vestin/ffmpeg_build/include/libswscale/version.h:
../src/native/extra.h:
