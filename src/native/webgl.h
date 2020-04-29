#ifndef WEBGL_H
#define WEBGL_H

#include <napi.h>

#define NAPI_METHOD(method_name) Napi::Value method_name \
   (const Napi::CallbackInfo& info);

#if USE_GLEW
	#include <GL/glew.h>
#else 
	#define EGL_EGL_PROTOTYPES 1
	#define EGL_EGLEXT_PROTOTYPES 1
	#include <EGL/egl.h>
	#include <EGL/eglext.h>
	#include <GLES3/gl32.h>

    #include <cuda.h>
    #include <cudaGL.h>
    
    extern "C" {
        #include <include_temp/libavcodec/avcodec.h>
        #include <include_temp/libavutil/hwcontext.h>
        #include <include_temp/libavutil/hwcontext_cuda.h>
        #include <include_temp/libavformat/avformat.h>
        #include <include_temp/libavutil/error.h>
        #include <include_temp/libavutil/opt.h>
        #include <include_temp/libswresample/swresample.h>
        #include <include_temp/libswscale/swscale.h>
    }
#endif


static const GLchar* vertexShaderSource = "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 position;\n"
    "const vec2 madd=vec2(0.5,0.5);\n"
    "out vec2 textureCoord;\n"
    "void main()\n"
    "{\n"
      "textureCoord = position.xy*madd+madd;\n"
      "gl_Position = vec4(position.xy, 0.0, 1.0);\n"
    "}\0";

static const GLchar* fragmentShaderSource =
    "#version 300 es\n"
    "precision highp float;\n"
    "out vec4 fragColor;\n"
    "uniform sampler2D tex;\n"
    "in vec2 textureCoord;\n"
    "void main () {\n"
      "fragColor = texture(tex, vec2(textureCoord.x, 1.0 - textureCoord.y ));\n"
    "}\n";

// fix temporary array error in c++1x
#ifdef av_err2str
#undef av_err2str
av_always_inline char* av_err2str(int errnum)
{
    static char str[AV_ERROR_MAX_STRING_SIZE];
    memset(str, 0, sizeof(str));
    return av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, errnum);
}
#endif

class WebGLRenderingContext : public Napi::ObjectWrap<WebGLRenderingContext> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  WebGLRenderingContext(const Napi::CallbackInfo& info);

    inline GLuint ToGLuint(const void* ptr) {
		return static_cast<GLuint>(reinterpret_cast<size_t>(ptr));
	}

    unsigned char* unpackPixels(GLenum type, GLenum format, GLint width, GLint height, unsigned char* pixels);

    // Initialization stuff
    bool alpha = true;
	bool stencil = true;
	bool depth = true;
	bool antialias = false;
	bool premultipliedAlpha = false;
	bool preserveDrawingBuffer = false;
	bool preferLowPowerToHighPerformance = false;
	bool failIfMajorPerformanceCaveat = false;

    // Webgl stuff
    bool  unpack_flip_y;
    bool  unpack_premultiply_alpha;
    GLint unpack_colorspace_conversion;
    GLint unpack_alignment;

    // EGL stuff
    EGLDisplay display;
	EGLContext context;
	EGLConfig  config;
	EGLSurface surface;


    // encoding stuff -----------------------------------------

    size_t screenWidth;
    size_t screenHeight;
    CUcontext* m_cuContext;
    CUgraphicsResource* cuInpTexRes;
    CUDA_MEMCPY2D_st* m_memCpyStruct;

    // FFmpeg
    AVFormatContext* ofmt_ctx = NULL;
    AVPacket* pkt;

    // Video
    AVCodecContext* c;
    AVFrame* frame;
    AVBufferRef* m_avBufferRefFrame;
    AVBufferRef* m_avBufferRefDevice;
    AVHWDeviceContext* hwDevContext;
    AVCUDADeviceContext* cudaDevCtx;
    AVHWFramesContext* frameCtxPtr;
    AVStream* video_stream = NULL;
    int have_video = 1;
    int frame_index = 0;

    int counter = 0;

    // Audio
    AVCodecContext* audio_ctx;
    int getFrameSize();

    int frame_bytes, audio_idx, dst_nb_samples;
    int src_sample_rate, src_bit_rate, src_nr_channels, src_size;
    int have_audio = 0;
    AVFrame* audio_frame;
    struct SwrContext* audio_swr_ctx = NULL;
    SwsContext * video_sws_ctx;
    AVStream* audio_stream = NULL;

    // OpenGL
    GLint native_texture_id;
    GLuint texture;
    GLuint fbo;
    GLuint fboMsaaId;
    GLuint rboColorId;
    GLuint rboDepthId;
    GLuint rboId;

    // flipping stuff
    GLuint flipTexture;
    GLuint fboFlip;
    GLuint bufferPosition;
    GLuint shaderProgram;
    GLuint vertexPosition;
    GLuint vao;
    GLuint VAO;
    GLuint VBO;
    GLuint textureLocation;
    GLuint timeLocation;

    bool has_hardware_support = true;
    void setUpFramebuffers();
    AVFrame* alloc_audio_frame();
    void encode(AVFrame* frame, AVCodecContext* cod, AVStream* out, AVPacket* p);

 private:
    NAPI_METHOD(NvencInitAudio);
    NAPI_METHOD(NvencInitVideo);
    NAPI_METHOD(NvencOpen);
    NAPI_METHOD(NvencEncode);
    NAPI_METHOD(NvencAddAudioFrame);
    NAPI_METHOD(NvencBindBuffer);
    NAPI_METHOD(NvencClose);
    NAPI_METHOD(NvencGetFrameSize);
    NAPI_METHOD(NvencWriteImage);




    static Napi::FunctionReference constructor;
    NAPI_METHOD(BindAttribLocation);
    NAPI_METHOD(DisableVertexAttribArray);
    NAPI_METHOD(EnableVertexAttribArray);
    NAPI_METHOD(GetActiveAttrib);
    NAPI_METHOD(GetAttribLocation);
    NAPI_METHOD(GetVertexAttrib);
    NAPI_METHOD(GetVertexAttribOffset);
    NAPI_METHOD(VertexAttrib1f);
    NAPI_METHOD(VertexAttrib1fv);
    NAPI_METHOD(VertexAttrib2f);
    NAPI_METHOD(VertexAttrib2fv);
    NAPI_METHOD(VertexAttrib3f);
    NAPI_METHOD(VertexAttrib3fv);
    NAPI_METHOD(VertexAttrib4f);
    NAPI_METHOD(VertexAttrib4fv);
    NAPI_METHOD(VertexAttribPointer);
    NAPI_METHOD(VertexAttribIPointer);


    // Blend

    NAPI_METHOD(BlendColor);
    NAPI_METHOD(BlendEquation);
    NAPI_METHOD(BlendEquationSeparate);
    NAPI_METHOD(BlendFunc);
    NAPI_METHOD(BlendFuncSeparate);


    // VBO

    NAPI_METHOD(CreateBuffer);
    NAPI_METHOD(DeleteBuffer);
    NAPI_METHOD(IsBuffer);
    NAPI_METHOD(BindBuffer);
    NAPI_METHOD(BindBufferBase);
    NAPI_METHOD(BindBufferRange);
    NAPI_METHOD(BufferData);
    NAPI_METHOD(BufferSubData);
    NAPI_METHOD(CopyBufferSubData);
    NAPI_METHOD(GetBufferSubData);
    NAPI_METHOD(GetBufferParameter);


    // FBO

    NAPI_METHOD(CreateFramebuffer);
    NAPI_METHOD(DeleteFramebuffer);
    NAPI_METHOD(IsFramebuffer);
    NAPI_METHOD(BindFramebuffer);
    NAPI_METHOD(BindFrameBuffer);
    NAPI_METHOD(BlitFrameBuffer);
    NAPI_METHOD(CheckFramebufferStatus);
    NAPI_METHOD(FramebufferRenderbuffer);
    NAPI_METHOD(FramebufferTexture2D);
    NAPI_METHOD(GetFramebufferAttachmentParameter);


    // Program

    NAPI_METHOD(CreateProgram);
    NAPI_METHOD(DeleteProgram);
    NAPI_METHOD(IsProgram);
    NAPI_METHOD(GetProgramInfoLog);
    NAPI_METHOD(GetProgramParameter);
    NAPI_METHOD(LinkProgram);
    NAPI_METHOD(UseProgram);
    NAPI_METHOD(ValidateProgram);


    // RBO

    NAPI_METHOD(CreateRenderbuffer);
    NAPI_METHOD(DeleteRenderbuffer);
    NAPI_METHOD(IsRenderbuffer);
    NAPI_METHOD(BindRenderbuffer);
    NAPI_METHOD(GetRenderbufferParameter);
    NAPI_METHOD(RenderbufferStorage);



    // Shader


    NAPI_METHOD(DeleteShader);
    NAPI_METHOD(CreateShader);
    NAPI_METHOD(IsShader);
    NAPI_METHOD(AttachShader);
    NAPI_METHOD(CompileShader);
    NAPI_METHOD(DetachShader);
    NAPI_METHOD(GetAttachedShaders);
    NAPI_METHOD(GetShaderInfoLog);
    NAPI_METHOD(GetShaderParameter);
    NAPI_METHOD(GetShaderSource);
    NAPI_METHOD(ShaderSource);
    NAPI_METHOD(GetShaderPrecisionFormat);



    // Stencil

    NAPI_METHOD(ClearStencil);
    NAPI_METHOD(StencilFunc);
    NAPI_METHOD(StencilFuncSeparate);
    NAPI_METHOD(StencilMask);
    NAPI_METHOD(StencilMaskSeparate);
    NAPI_METHOD(StencilOp);
    NAPI_METHOD(StencilOpSeparate);


    // Texture

    NAPI_METHOD(CreateTexture);
    NAPI_METHOD(DeleteTexture);
    NAPI_METHOD(IsTexture);
    NAPI_METHOD(BindTexture);
    NAPI_METHOD(ActiveTexture);
    NAPI_METHOD(CopyTexImage2D);
    NAPI_METHOD(CopyTexSubImage2D);
    NAPI_METHOD(GenerateMipmap);
    NAPI_METHOD(GetTexParameter);
    NAPI_METHOD(TexImage2D);
    NAPI_METHOD(TexParameterf);
    NAPI_METHOD(TexParameteri);
    NAPI_METHOD(TexSubImage2D);

    NAPI_METHOD(TexStorage2D);
    NAPI_METHOD(CreateSampler);
    NAPI_METHOD(SamplerParameteri);
    NAPI_METHOD(BindSampler);

    // Uniform

    NAPI_METHOD(GetActiveUniform);
    NAPI_METHOD(GetUniform);
    NAPI_METHOD(GetUniformLocation);
    NAPI_METHOD(Uniform1f);
    NAPI_METHOD(Uniform1fv);
    NAPI_METHOD(Uniform1i);
    NAPI_METHOD(Uniform1iv);
    NAPI_METHOD(Uniform2f);
    NAPI_METHOD(Uniform2fv);
    NAPI_METHOD(Uniform2i);
    NAPI_METHOD(Uniform2iv);
    NAPI_METHOD(Uniform3f);
    NAPI_METHOD(Uniform3fv);
    NAPI_METHOD(Uniform3i);
    NAPI_METHOD(Uniform3iv);
    NAPI_METHOD(Uniform4f);
    NAPI_METHOD(Uniform4fv);
    NAPI_METHOD(Uniform4i);
    NAPI_METHOD(Uniform4iv);
    NAPI_METHOD(UniformMatrix2fv);
    NAPI_METHOD(UniformMatrix3fv);
    NAPI_METHOD(UniformMatrix4fv);


    // VAO

    NAPI_METHOD(CreateVertexArray);
    NAPI_METHOD(DeleteVertexArray);
    NAPI_METHOD(IsVertexArray);
    NAPI_METHOD(BindVertexArray);


    // Instances

    NAPI_METHOD(DrawArraysInstanced);
    NAPI_METHOD(DrawElementsInstanced);
    NAPI_METHOD(VertexAttribDivisor);


    // Transform feedback
    NAPI_METHOD(CreateTransformFeedback);
    NAPI_METHOD(DeleteTransformFeedback);
    NAPI_METHOD(IsTransformFeedback);
    NAPI_METHOD(BindTransformFeedback);
    NAPI_METHOD(BeginTransformFeedback);
    NAPI_METHOD(EndTransformFeedback);
    NAPI_METHOD(PauseTransformFeedback);
    NAPI_METHOD(ResumeTransformFeedback);
    NAPI_METHOD(TransformFeedbackVaryings);
    NAPI_METHOD(GetTransformFeedbackVarying);

    // Misc OpenGL Functions

    NAPI_METHOD(Clear);
    NAPI_METHOD(ClearColor);
    NAPI_METHOD(ClearDepth);
    NAPI_METHOD(ColorMask);
    NAPI_METHOD(CullFace);
    NAPI_METHOD(DepthFunc);
    NAPI_METHOD(DepthMask);
    NAPI_METHOD(DepthRange);
    NAPI_METHOD(Disable);
    NAPI_METHOD(DrawArrays);
    NAPI_METHOD(DrawElements);
    NAPI_METHOD(Enable);
    NAPI_METHOD(Finish);
    NAPI_METHOD(Flush);
    NAPI_METHOD(FrontFace);
    NAPI_METHOD(GetError);
    NAPI_METHOD(GetParameter);
    NAPI_METHOD(GetRenderTarget);
    NAPI_METHOD(GetSupportedExtensions);
    NAPI_METHOD(Hint);
    NAPI_METHOD(IsEnabled);
    NAPI_METHOD(LineWidth);
    NAPI_METHOD(PixelStorei);
    NAPI_METHOD(PolygonOffset);
    NAPI_METHOD(ReadPixels);
    NAPI_METHOD(SampleCoverage);
    NAPI_METHOD(Scissor);
    NAPI_METHOD(Viewport);
};

#endif