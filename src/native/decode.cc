#include "decode.h"
#include "extra.h"
#include <unistd.h>

#define JS_METHOD(webgl_name, method_name) \
  InstanceMethod( \
    webgl_name \ 
    , &VideoDecodingContext:: method_name \
  )


static const GLchar* vertexShaderSource2 = "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 position;\n"
    "const vec2 madd=vec2(0.5,0.5);\n"
    "out vec2 textureCoord;\n"
    "void main()\n"
    "{\n"
      "textureCoord = position.xy*madd+madd;\n"
      "gl_Position = vec4(position.xy, 0.0, 1.0);\n"
    "}\0";

static const GLchar* fragmentShaderSource2 =
    "#version 300 es\n"
    "precision highp float;\n"
    "out vec4 fragColor;\n"
    "uniform sampler2D tex;\n"
    "in vec2 textureCoord;\n"
    "void main () {\n"
      "fragColor = texture(tex, vec2(textureCoord.x, 1.0 - textureCoord.y ));\n"
    "}\n";

#define METHOD(method_name) Napi::Value VideoDecodingContext:: method_name \
   (const Napi::CallbackInfo& info)

Napi::FunctionReference VideoDecodingContext::constructor;


METHOD(InitGL) { NAPI_ENV

    buildShader();
    RET_UNDEFINED;
}

void VideoDecodingContext::buildShader() {
    std::cout << "Setting up shaders..." << std::endl;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource2, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        exit(1);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                << infoLog << std::endl;
        exit(1);
    }

    std::cout << "Linking shaders..." << std::endl;
    // Link  shaders
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for linking errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::LINKING_FAILED\n" << infoLog << std::endl;
        exit(1);
    }

    std::cout << "Deleting shaders..." << std::endl;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    GLfloat vertices[] = {
        -1.f, -1.f,
        -1.f, 1.f,
        1.f,  1.f,
        -1.f, -1.f,
        1.f,  1.f,
        1.f,  -1.f,
    };
    glUseProgram(program);
    textureLocation = glGetUniformLocation(program, "tex");
    std::cout << "Texture location: " << textureLocation <<  ", setting up vertex buffers..." << std::endl;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    std::cout << "Setting up frame buffers..." << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glUniform1i(textureLocation, 0);


    glBindTexture(GL_TEXTURE_2D, NULL);
    glUseProgram(NULL);
    glUseProgram(NULL);
    glBindVertexArray(NULL);

    std::cout << "Done with set up" << std::endl;
}

void VideoDecodingContext::createWindow() {
    #if USE_GLFW
    if (!glfwInit()) {
        printf("GLFW context creation failed;\n");
        exit(1);
    }
    //glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    window = glfwCreateWindow(width, height, "Video test", NULL, NULL);
    if(!window) {
        printf("Window creation failed;\n");
        exit(1);
    }	

    glfwMakeContextCurrent(window);


    std::cout << "Initialized" << std::endl;
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    #endif
}

int64_t FrameToPts(AVStream* st, int frame) {
    return (int64_t(frame) * st->r_frame_rate.den *  st->time_base.den) /  (int64_t(st->r_frame_rate.num) * st->time_base.num);
}

METHOD(CreateWindow) { NAPI_ENV
    createWindow();
    RET_UNDEFINED;
}

METHOD(SeekFrame) { NAPI_ENV
    REQ_UINT32_ARG(0, frame_idx);
    int64_t pts = FrameToPts(video_stream, frame_idx);
    int ret = av_seek_frame(fmt_ctx, video_stream_idx, pts, AVSEEK_FLAG_FRAME);
    RET_NUM(ret)
}

METHOD(GetInfo) { NAPI_ENV
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("den", Napi::Number::New(env, video_stream->r_frame_rate.den));
    obj.Set("num", Napi::Number::New(env, video_stream->r_frame_rate.num));
    obj.Set("width", Napi::Number::New(env, video_ctx->width));
    obj.Set("height", Napi::Number::New(env, video_ctx->height));
    return obj;
}


METHOD(Draw) { NAPI_ENV
    glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDepthMask(false);

    glClear(GL_COLOR_BUFFER_BIT);	
    glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
    #if USE_GLFW
    if (window) {
        glfwSwapBuffers(window);
    }
    #endif

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    
	
    RET_UNDEFINED;
}

METHOD(GetFrame) { NAPI_ENV
   int ret = 0;

   ret = av_read_frame(fmt_ctx, pkt);
   ret = avcodec_send_packet(video_ctx, pkt);

   if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
    }
    
    while (ret >= 0) {
        ret = avcodec_receive_frame(video_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            RET_UNDEFINED
        } else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }

        if (!conv_inited) {
            conv_ctx = sws_getContext(width, height, video_ctx->pix_fmt, width, height, AV_PIX_FMT_RGB32, SWS_BILINEAR, NULL, NULL, NULL);
            conv_inited = 1;
        }

        sws_scale(conv_ctx, frame->data, frame->linesize, 0,  height, scaled_frame->data, scaled_frame->linesize);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, scaled_frame->data[0]);	
    }
}



METHOD(OpenVideo) { NAPI_ENV
    REQ_STR_ARG(0, src_filename);


       /* open input file, and allocate format context */
    if (avformat_open_input(&fmt_ctx, src_filename.c_str(), NULL, NULL) < 0) {
        fprintf(stderr, "Could not open source file %s\n", src_filename.c_str());
        exit(1);
    }

    /* retrieve stream information */
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        exit(1);
    }
    int ret, stream_index;
    AVStream *st;
    AVCodec *dec = NULL;
    AVDictionary *opts = NULL;

    const AVMediaType type = AVMEDIA_TYPE_VIDEO;

    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
      if (ret < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n", av_get_media_type_string(type), src_filename.c_str());
        exit(1);
    } 
    stream_index = ret;
    st = fmt_ctx->streams[stream_index];

    /* find decoder for the stream */
    dec = avcodec_find_decoder(st->codecpar->codec_id);
    if (!dec) {
        fprintf(stderr, "Failed to find %s codec\n", av_get_media_type_string(type));
        exit(1);
    }

    /* Allocate a codec context for the decoder */
    video_ctx = avcodec_alloc_context3(dec);
    if (!video_ctx) {
        fprintf(stderr, "Failed to allocate the %s codec context\n", av_get_media_type_string(type));
        exit(1);
    }

    /* Copy codec parameters from input stream to output codec context */
    if ((ret = avcodec_parameters_to_context(video_ctx, st->codecpar)) < 0) {
        fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n", av_get_media_type_string(type));
        exit(1);
    }

    /* Init the decoders, with or without reference counting */
    static int refcount = 0;
    if ((ret = avcodec_open2(video_ctx, dec, &opts)) < 0) {
        fprintf(stderr, "Failed to open %s codec\n", av_get_media_type_string(type));
        exit(1);
    }
    video_stream_idx = stream_index;
    video_stream = fmt_ctx->streams[video_stream_idx];

    /* allocate image where the decoded image will be put */
    width = video_ctx->width;
    height = video_ctx->height;
    pix_fmt = video_ctx->pix_fmt;

    scaled_frame = av_frame_alloc();
    av_image_alloc(scaled_frame->data, scaled_frame->linesize, width, height, AV_PIX_FMT_RGB32, 1);


    /* dump input information to stderr */
    printf("Dumping format \n");
    av_dump_format(fmt_ctx, 0, src_filename.c_str(), 0);


    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate frame\n");
        exit(1);
    }

    printf("allocing frame \n");

    /* initialize packet, set data to NULL, let the demuxer fill it */
    pkt = av_packet_alloc();
    if(!pkt) {
        fprintf(stderr, "Could not allocate packet\n");
        exit(1);
    }
    /* initialize packet, set data to NULL, let the demuxer fill it */
    av_init_packet(pkt);
    pkt->data = NULL;
    pkt->size = 0;



    printf("Done allocating \n");
    RET_UNDEFINED;
}


METHOD(Close) { NAPI_ENV
    avcodec_free_context(&video_ctx);
    avformat_close_input(&fmt_ctx);
    av_frame_free(&frame);
    //av_free(video_dst_data[0]);
    RET_UNDEFINED;
}



Napi::Object VideoDecodingContext::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "VideoDecodingContext",
    {      
      JS_METHOD("setBuffer", SetBuffer),
      JS_METHOD("openVideo", OpenVideo),
      JS_METHOD("getFrame", GetFrame),
      JS_METHOD("close", Close),
      JS_METHOD("createWindow", CreateWindow),
      JS_METHOD("draw", Draw),
      JS_METHOD("initGL", InitGL),
      JS_METHOD("seek", SeekFrame),
      JS_METHOD("getInfo", GetInfo),

    });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("VideoDecodingContext", func);
  return exports;
}

VideoDecodingContext::VideoDecodingContext(const Napi::CallbackInfo& info): Napi::ObjectWrap<VideoDecodingContext>(info) {

}




METHOD(SetBuffer) { NAPI_ENV
    printf("hello\n");
    RET_UNDEFINED;
}
    
