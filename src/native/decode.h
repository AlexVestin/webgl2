#ifndef DECODE_H
#define DECODE_H

#include <napi.h>

#define NAPI_METHOD(method_name) Napi::Value method_name \
   (const Napi::CallbackInfo& info);




#define USE_GLFW 0

#if USE_GLFW
    #define GLFW_INCLUDE_ES3 
    #include <GLFW/glfw3.h>
#else
    #include <GLES3/gl3.h>
#endif
extern "C" {
    #include <libavutil/hwcontext.h>
    #include <libavutil/hwcontext_cuda.h>
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/imgutils.h>

    #include <libswresample/swresample.h>
    #include <libswscale/swscale.h>
}

class VideoDecodingContext : public Napi::ObjectWrap<VideoDecodingContext> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  VideoDecodingContext(const Napi::CallbackInfo& info);
  static Napi::FunctionReference constructor;

    // encoding stuff -----------------------------------------
    size_t screenWidth;
    size_t screenHeight;

    
    GLuint program, VAO, VBO, textureLocation, texture;
    
    //
    void createWindow();
    void buildShader();

    int width; 
    int height; 

    // GLFW 
    #if USE_GLFW
    GLFWwindow* window;
    #endif

    // FFmpeg
    AVFormatContext* fmt_ctx = NULL;
    AVPacket* pkt;
    AVStream* video_stream;
    enum AVPixelFormat pix_fmt;
    struct SwsContext *conv_ctx;


    uint8_t* dst_data;
    int dst_linesize[4];

    // Video
    AVCodecContext* video_ctx;
    int video_stream_idx;
    int frame_count = 0;

    AVFrame *frame, *scaled_frame;


    NAPI_METHOD(SetBuffer);
    NAPI_METHOD(OpenVideo);
    NAPI_METHOD(Close);
    NAPI_METHOD(GetFrame);
    NAPI_METHOD(CreateWindow);
    NAPI_METHOD(Draw);
    NAPI_METHOD(InitGL);
};

#endif