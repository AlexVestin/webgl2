#include "webgl.h"
#include "extra.h"
#include <iostream>
#include "jpeglib.h"

#define ANGLE 0

#define GL_METHOD(method_name) Napi::Value WebGLRenderingContext:: method_name \
   (const Napi::CallbackInfo& info)




void logError(int errnum) {
  static char str[AV_ERROR_MAX_STRING_SIZE];
	memset(str, 0, sizeof(str));
	printf("Error occured: %s\n", av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, errnum));
}

void write_to_img(std::string name, int screenWidth, int screenHeight) {
  const int num_components = 4;
  unsigned char* pdata = new unsigned char[screenWidth * screenHeight * num_components];
  glReadPixels(0, 0, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, pdata);

  FILE* outfile;
  std::string padded_string =
      std::string(3 - name.length(), '0') + name + ".jpg";
  std::string pre = "buffer/" + padded_string;
  if ((outfile = fopen(pre.c_str(), "wb")) == NULL) {
    printf("can't open %s");
    exit(1);
  }
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, outfile);
  cinfo.image_width = screenWidth;
  cinfo.image_height = screenHeight;
  cinfo.input_components = num_components;
  cinfo.in_color_space = JCS_EXT_RGBA;
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, 100, true);
  jpeg_start_compress(&cinfo, true);
  JSAMPROW row_pointer;
  int row_stride = screenWidth * num_components;
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer = (JSAMPROW)&pdata[cinfo.next_scanline * row_stride];
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);
  fclose(outfile);
  jpeg_destroy_compress(&cinfo);
}



GL_METHOD(NvencWriteImage) { NAPI_ENV;
    write_to_img(std::to_string(counter++), screenWidth, screenHeight);
    RET_UNDEFINED
}



GL_METHOD(NvencBindBuffer) { NAPI_ENV;
    glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
    RET_NUM(fboMsaaId);
}


GL_METHOD(NvencGetFrameSize) { NAPI_ENV;
    if(!have_audio || !audio_frame) {
        std::cerr << "Haven't initialized the audio" << std::endl;
        exit(1);
    }
    RET_NUM(audio_frame->nb_samples);
}

void WebGLRenderingContext::encode(AVFrame* frame, AVCodecContext* cod,
                               AVStream* out, AVPacket* p) {
  int ret = avcodec_send_frame(cod, frame);

  if (ret < 0) {
    // printf(stderr, "Error sending a frame for encoding\n");
    exit(1);
  }

  while (ret >= 0) {
    ret = avcodec_receive_packet(cod, p);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
      av_packet_unref(p);
      return;
    } else if (ret < 0) {
      std::cerr << "Error during encoding" << std::endl;
      exit(1);
    }

    // log_packet(ofmt_ctx, pkt, "write");
    p->stream_index = out->index;
    av_packet_rescale_ts(p, cod->time_base, out->time_base);
    av_write_frame(ofmt_ctx, p);
    av_packet_unref(p);
  }
}

AVFrame* WebGLRenderingContext::alloc_audio_frame() {
  AVFrame* audio_frame = av_frame_alloc();
  int ret;

  if (!audio_frame) {
    std::cerr << "Error allocating an audio frame" << std::endl;
    exit(1);
  }

  audio_frame->format = audio_ctx->sample_fmt;
  audio_frame->channel_layout = audio_ctx->channel_layout;
  audio_frame->sample_rate = audio_ctx->sample_rate;
  audio_frame->nb_samples = audio_ctx->frame_size;

  std::cout << "Frame size" << audio_ctx->frame_size << std::endl;

  ret = av_frame_get_buffer(audio_frame, 0);
  if (ret < 0) {
    std::cerr << "Error allocating an audio buffer\n" << std::endl;
    exit(1);
  }

  return audio_frame;
}

GL_METHOD(NvencInitAudio) { NAPI_ENV;
   int ret;
   REQ_INT32_ARG(0, sample_rate);
   REQ_INT32_ARG(1, bit_rate);
   REQ_INT32_ARG(2, nr_channels);


  src_sample_rate = sample_rate;
  src_bit_rate = bit_rate;
  src_nr_channels = nr_channels;

  std::cout << "initializing audio with sr: " << sample_rate
            << " br: " << bit_rate << " nrc: " << nr_channels << std::endl;
  // const int codecs[] = { AV_CODEC_ID_OPUS, AV_CODEC_ID_AAC, AV_CODEC_ID_MP3
  // };

  frame_bytes = 0;

  AVCodec* ac = avcodec_find_encoder(AV_CODEC_ID_MP3);
  if (!ac) {
    printf("error making audio codec context\n");
    exit(-1);
  }

  audio_stream = avformat_new_stream(ofmt_ctx, NULL);
  audio_stream->id = ofmt_ctx->nb_streams - 1;
  audio_ctx = avcodec_alloc_context3(ac);

  audio_ctx->bit_rate = bit_rate;
  audio_ctx->sample_rate = sample_rate;
  if (ac->supported_samplerates) {
    audio_ctx->sample_rate = ac->supported_samplerates[0];
    for (int i = 0; ac->supported_samplerates[i]; i++) {
      if (ac->supported_samplerates[i] == sample_rate)
        audio_ctx->sample_rate = sample_rate;
    }
  }
  audio_ctx->channels = nr_channels;
  audio_ctx->channel_layout = AV_CH_LAYOUT_STEREO;
  if (ac->channel_layouts) {
    audio_ctx->channel_layout = ac->channel_layouts[0];
    for (int i = 0; ac->channel_layouts[i]; i++) {
      if (ac->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
        audio_ctx->channel_layout = AV_CH_LAYOUT_STEREO;
    }
  }

  audio_ctx->channels =
      av_get_channel_layout_nb_channels(audio_ctx->channel_layout);
  audio_ctx->sample_fmt = ac->sample_fmts[0];
  audio_stream->time_base = (AVRational){1, audio_ctx->sample_rate};

  ret = avcodec_open2(audio_ctx, ac, NULL);
  if (ret < 0) {
    // printf("Could not open audio codec: %s\n", av_err2str(ret));
    exit(1);
  }

  audio_frame = alloc_audio_frame();
  ret = avcodec_parameters_from_context(audio_stream->codecpar, audio_ctx);
  if (ret < 0) {
    printf("Could not copy the stream parameters\n");
    exit(1);
  }

  audio_swr_ctx = swr_alloc();
  if (!audio_swr_ctx) {
    printf("Could not allocate resampler context\n");
    exit(1);
  }

  av_opt_set_int(audio_swr_ctx, "in_channel_count", nr_channels, 0);
  av_opt_set_int(audio_swr_ctx, "in_sample_rate", sample_rate, 0);
  av_opt_set_sample_fmt(audio_swr_ctx, "in_sample_fmt", AV_SAMPLE_FMT_FLTP, 0);
  av_opt_set_int(audio_swr_ctx, "out_channel_count", audio_ctx->channels, 0);
  av_opt_set_int(audio_swr_ctx, "out_sample_rate", audio_ctx->sample_rate, 0);
  av_opt_set_sample_fmt(audio_swr_ctx, "out_sample_fmt", audio_ctx->sample_fmt,
                        0);

  if ((ret = swr_init(audio_swr_ctx)) < 0) {
    std::cerr << "Failed to initialize the resampling context\n" << std::endl;
    exit(1);
  }

  if (!pkt) pkt = av_packet_alloc();

  if (!pkt) {
    exit(1);
  }

  have_audio = 1;
  RET_UNDEFINED;
}

GL_METHOD(NvencEncode) { NAPI_ENV;
    // draw to fb
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fboMsaaId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboFlip);
    glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);


    // bind, resets
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0.f,0.f,0.f,1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    glBindVertexArray(0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glUseProgram(0);
    glEnable(GL_DITHER);
    glEnable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);

    for (int i = 0; i < 16; i++) {
      glDisableVertexAttribArray(i);
    }

    // draw
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, flipTexture);
    glUniform1i(textureLocation, 0);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //write_to_img(std::to_string(counter++), screenWidth, screenHeight);
    

    int ret;
    if(has_hardware_support) {
      if (!(frame = av_frame_alloc())) {
        std::cout << "Error allocating frame" << std::endl;
        exit(1);
     }

      if ((ret = av_hwframe_get_buffer(c->hw_frames_ctx, frame, 0)) < 0) {
          std::cout << "error getting buffer" << std::endl;
          std::cout << AVERROR(EINVAL) << ":" << ret << std::endl;
          char err_chars[64];
          av_strerror(ret, err_chars, 64);
          std::cout << err_chars << std::endl;
          exit(1);
      }
      if (!frame->hw_frames_ctx) {
          std::cout << "no hw frames ctx" << std::endl;
          exit(1);
      }

      // Perform cuda mem copy for input buffer
      CUresult cuRes;
      CUarray mappedArray;
      CUcontext oldCtx;

      // Get context
      cuRes = cuCtxPopCurrent_v2(&oldCtx);  // THIS IS ALLOWED TO FAIL
      cuRes = cuCtxPushCurrent_v2(*m_cuContext);

      // Get Texture
      cuGraphicsResourceSetMapFlags_v2(*cuInpTexRes, CU_GRAPHICS_MAP_RESOURCE_FLAGS_READ_ONLY);
      cuGraphicsMapResources(1, cuInpTexRes, 0);

      // Map texture to cuda array
      cuRes = cuGraphicsSubResourceGetMappedArray(&mappedArray, *cuInpTexRes, 0, 0);
      if (cuRes != 0) {
          std::cout << "ERROR get mapped array" << std::endl;
      }
      cuRes = cuGraphicsUnmapResources(1, cuInpTexRes, 0);
      if (cuRes != 0) {
          std::cout << "ERROR unmapped array" << std::endl;
      }

      m_memCpyStruct->srcArray = mappedArray;
      m_memCpyStruct->dstDevice = (CUdeviceptr)frame->data[0];
      m_memCpyStruct->dstPitch = frame->linesize[0];
      m_memCpyStruct->WidthInBytes = frame->width * 4;  //* 4 needed for each pixel
      m_memCpyStruct->Height = frame->height;           // Vanilla height for frame

      cuRes = cuMemcpy2D_v2(m_memCpyStruct);
    } else {
      frame = av_frame_alloc();
      ret = av_frame_make_writable(frame);

      unsigned char* pixels = new unsigned char[screenWidth * screenHeight * 4];
      glReadPixels(0, 0, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
      uint8_t *inData[1]     = { pixels };
      int inLinesize[1] = { 4 * c->width };

      frame->format = AV_PIX_FMT_YUV420P;
      frame->width = c->width;
      frame->height = c->height;
      ret = av_frame_get_buffer(frame, 32);
      if(ret < 0) {
        std::cerr << "Couldnt get frame buffer for encoding." << std::endl;
        exit(1);
      }
        
      sws_scale(video_sws_ctx, inData, inLinesize, 0, c->height, frame->data, frame->linesize);
      free(pixels);
    }
    

    // ret = avcodec_send_frame(c, frame);
    AVPacket enc_pkt;
    av_init_packet(&enc_pkt);
    enc_pkt.data = NULL;
    enc_pkt.size = 0;

    // https://github.com/FFmpeg/FFmpeg/blob/master/doc/examples/muxing.c#L154
    // https://stackoverflow.com/questions/20909252/calculate-pts-before-frame-encoding-in-ffmpeg
    // confused
    frame->pts = frame_index++;

    encode(frame, c, video_stream, &enc_pkt);
    av_frame_unref(frame);

        // unbind
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    
    RET_UNDEFINED;
    
}

GL_METHOD(NvencAddAudioFrame) { NAPI_ENV;
    int ret;
    REQ_OBJ_ARG(0, left);
    REQ_OBJ_ARG(1, right);
    REQ_UINT32_ARG(2, size);

    int s;
    void* leftData = getArrayData<uint8_t>(env, left, &s);
    void* rightData = getArrayData<uint8_t>(env, right, &s);

    ret = av_frame_make_writable(audio_frame);
    if (ret < 0) {
        printf("error\n");
        exit(1);
    }

    audio_frame->data[0] = (uint8_t*)leftData;
    audio_frame->data[1] = (uint8_t*)rightData;
    dst_nb_samples =
        av_rescale_rnd(swr_get_delay(audio_swr_ctx, audio_ctx->sample_rate) +
                            audio_frame->nb_samples,
                        src_sample_rate, audio_ctx->sample_rate, AV_ROUND_UP);

    ret =
        swr_convert(audio_swr_ctx, audio_frame->data, dst_nb_samples,
                    (const uint8_t**)audio_frame->data, audio_frame->nb_samples);

    if (ret < 0) {
        std::cerr << "Error in swr_convert, ret: " << ret << std::endl;
        exit(1);
    }
    audio_frame->pts =
        av_rescale_q(frame_bytes, (AVRational){1, audio_ctx->sample_rate},
                    audio_ctx->time_base);

    frame_bytes += dst_nb_samples;

    AVPacket audio_pkt;
    av_init_packet(&audio_pkt);
    encode(audio_frame, audio_ctx, audio_stream, &audio_pkt);
    RET_UNDEFINED;    
}


GL_METHOD(NvencClose) { NAPI_ENV;
    std::cout << "Starting nvencClose" << std::endl;
    AVPacket p;
    av_init_packet(&p);
    if (have_video) encode(NULL, c, video_stream, &p);
    if (have_audio) encode(NULL, audio_ctx, audio_stream, &p);
    av_packet_unref(&p);
    av_write_trailer(ofmt_ctx);

    if (have_video) {
        avcodec_free_context(&c);
        av_frame_free(&frame);
    }

    if (have_audio) {
        avcodec_free_context(&audio_ctx);
        av_frame_free(&audio_frame);
        swr_free(&audio_swr_ctx);
    }

    avio_close(ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);

    // free opengl stuff
    glDeleteFramebuffers(1, &fbo);
    glDeleteFramebuffers(1, &fboMsaaId);
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &rboColorId);
    glDeleteRenderbuffers(1, &rboDepthId);
    glDeleteRenderbuffers(1, &rboId);
    // Free cuda stuff
    if (has_hardware_support) {
      cuGraphicsUnregisterResource(*cuInpTexRes);
      // free(cuInpTexRes);
      free(m_memCpyStruct);
    }
    

    std::cout << "done nvencClose" << std::endl;
    RET_UNDEFINED;
}


void WebGLRenderingContext::setUpFramebuffers() {
  // Set up basic shader program ------------------------------------------------------------------------------------
   // Vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  GLint success;
  GLchar infoLog[512];

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    exit(1);
  }

  std::cout << "Setting up shaders..." << std::endl;
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
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
  glUseProgram(shaderProgram);
  textureLocation = glGetUniformLocation(shaderProgram, "tex");
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
  glBindVertexArray(0);

  // FRAMEBUFFERS -------------------------------------------------------------------------------
  // http://www.songho.ca/opengl/gl_fbo.html
  glGenTextures(1, &flipTexture);
  glBindTexture(GL_TEXTURE_2D, flipTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glGenFramebuffers(1, &fboFlip);
  glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);

  std::cout << "Setting up flip framebuffer..." << std::endl;
  GLuint flipRbo;
  glGenRenderbuffers(1, &flipRbo);
  glBindRenderbuffer(GL_RENDERBUFFER, flipRbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, flipTexture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, flipRbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "ERROR binding FLIP FRAMEBUFFER!" << std::endl;
    exit(0);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);



  // -----------------------------------------------------------------------
  // http://www.songho.ca/opengl/gl_fbo.html
  std::cout << "Setting up cuda msaa framebuffer..." << std::endl;
  glGenFramebuffers(1, &fboMsaaId);
  glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);

  int msaaCount = 4;
  // create a MSAA renderbuffer object to store color info
  glGenRenderbuffers(1, &rboColorId);
  glBindRenderbuffer(GL_RENDERBUFFER, rboColorId);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaCount, GL_RGBA8, screenWidth, screenHeight);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glGenRenderbuffers(1, &rboDepthId);
  glBindRenderbuffer(GL_RENDERBUFFER, rboDepthId);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaCount, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  // attach msaa RBOs to FBO attachment points
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboColorId);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthId);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "ERROR binding msaa FRAMEBUFFER!" << std::endl;
    exit(0);
  }

  std::cout << "Setting up out framebuffer..." << std::endl;

    std::cout << "Setting up cuda texture..." << std::endl;
  // Generate texture cuda can grab
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glGenRenderbuffers(1, &rboId);
  glBindRenderbuffer(GL_RENDERBUFFER, rboId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "ERROR binding screen framebuffr!" << std::endl;
    exit(0);
  }

  // Create output fbo https://learnopengl.com/Advanced-OpenGL/Framebuffers
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

  //glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_NATIVE_ID_ANGLE, &native_texture_id);
  native_texture_id = texture;

  std::cout << "Native texture id: " << native_texture_id << std::endl;

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "ERROR BINDING RENDERBUFFER!" << std::endl;
    exit(0);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


GL_METHOD(NvencInitVideo) { NAPI_ENV;
  REQ_INT32_ARG(0, bit_rate);
  REQ_INT32_ARG(1, min_bitrate);
  REQ_INT32_ARG(2, max_bitrate);
  REQ_INT32_ARG(3, fps);
  REQ_STR_ARG(4, profile);
  REQ_STR_ARG(5, level);
  REQ_STR_ARG(6, preset);


  glViewport(0, 0, screenWidth, screenHeight);
  setUpFramebuffers();

  std::cout << glGetString(GL_VENDOR) << std::endl;
  std::cout << glGetString(GL_RENDERER) << std::endl;
  std::cout << glGetString(GL_VERSION) << std::endl;
  std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "width: "<< screenWidth << std::endl;
  std::cout << "height: "<< screenHeight << std::endl;

  counter = 0;
  CUresult res;
  int ret;
  //has_hardware_support = false;
  ret = av_hwdevice_ctx_create(&m_avBufferRefDevice, AV_HWDEVICE_TYPE_CUDA, "", NULL, 0);

  
  if (ret < 0) {
    std::cerr << "Couldn't create hw_frame_ctx: " << ret << std::endl;
    has_hardware_support = false;
    logError(ret);
    //exit(1);
  }
  
  AVCodec* codec;

  if (has_hardware_support) {
    hwDevContext = (AVHWDeviceContext*)(m_avBufferRefDevice->data);
    cudaDevCtx = (AVCUDADeviceContext*)(hwDevContext->hwctx);
    m_cuContext = &(cudaDevCtx->cuda_ctx);

    m_avBufferRefFrame = av_hwframe_ctx_alloc(m_avBufferRefDevice);
    frameCtxPtr = (AVHWFramesContext*)(m_avBufferRefFrame->data);
    frameCtxPtr->width = screenWidth;
    frameCtxPtr->height = screenHeight;
    frameCtxPtr->sw_format = AV_PIX_FMT_0BGR32;
    frameCtxPtr->format = AV_PIX_FMT_CUDA;

    ret = av_hwframe_ctx_init(m_avBufferRefFrame);
    if (ret < 0) {
      std::cerr << "Couldn't initialize hw_frame_ctx: " << ret << std::endl;
      logError(ret);
      exit(1);
    }
  
    // Cast the OGL texture/buffer to cuda ptr
    std::cout << "Setting up cuda driver context..." << std::endl;
    CUcontext oldCtx;
    res = cuCtxPopCurrent_v2(&oldCtx);  // THIS IS ALLOWED TO FAIL
    res = cuCtxPushCurrent_v2(*m_cuContext);
    cuInpTexRes = (CUgraphicsResource*)malloc(sizeof(CUgraphicsResource));
    res = cuGraphicsGLRegisterImage(cuInpTexRes, native_texture_id, GL_TEXTURE_2D, CU_GRAPHICS_REGISTER_FLAGS_READ_ONLY);
    if (res != 0) {
        res =  cuGraphicsGLRegisterImage(cuInpTexRes, texture, GL_TEXTURE_2D, CU_GRAPHICS_REGISTER_FLAGS_READ_ONLY);
        if(res != 0) {
          std::cout << "failed to initialize image: " << native_texture_id  << std::endl;
          exit(1);
        } 
        std::cout << "not using ANGLE" << std::endl;
    } 

    std::cout << "Image initialized" << std::endl;
    res = cuCtxPopCurrent_v2(&oldCtx);  // THIS IS ALLOWED TO FAIL

    std::cout << "Setting up codec..." << std::endl;
      
    codec = avcodec_find_encoder_by_name("h264_nvenc");
    if(!codec) {
      std::cout << "Faile to find encoder by name h264_nvenc" << std::endl;
      exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if(!c) {
        std::cout << "Failed to allocate encoder cintextr" << std::endl;
        exit(1);
    }

    // Assign some hardware accel specific data to AvCodecContext
    c->hw_device_ctx = m_avBufferRefDevice;
    c->pix_fmt = AV_PIX_FMT_CUDA;
    c->sw_pix_fmt = AV_PIX_FMT_0BGR32;
    c->hw_frames_ctx = m_avBufferRefFrame;

    av_opt_set(c->priv_data, "preset", preset.c_str(), 0);
    av_opt_set(c->priv_data, "profile", profile.c_str(), 0);
    av_opt_set(c->priv_data, "level", level.c_str(), 0);

  
    m_memCpyStruct = (CUDA_MEMCPY2D_st*)malloc(sizeof(CUDA_MEMCPY2D_st));
    m_memCpyStruct->srcXInBytes = 0;
    m_memCpyStruct->srcY = 0;
    m_memCpyStruct->srcMemoryType = CUmemorytype::CU_MEMORYTYPE_ARRAY;

    m_memCpyStruct->dstXInBytes = 0;
    m_memCpyStruct->dstY = 0;
    m_memCpyStruct->dstMemoryType = CUmemorytype::CU_MEMORYTYPE_DEVICE;

    std::cout << "Setting up output format..." << std::endl;
  } else {
    codec = avcodec_find_encoder_by_name("libx264");
    c = avcodec_alloc_context3(codec);

    video_sws_ctx = sws_getContext(screenWidth, screenHeight,
                            AV_PIX_FMT_RGBA,
                            screenWidth, screenHeight,
                            AV_PIX_FMT_YUV420P,
                            0, 0, 0, 0);
    c->pix_fmt = AV_PIX_FMT_YUV420P;
  }

  
  c->codec_type = AVMEDIA_TYPE_VIDEO;
  c->width = screenWidth;
  c->height = screenHeight;
  c->time_base = (AVRational){1, fps};

  c->bit_rate = bit_rate;
  c->rc_max_rate = max_bitrate;
  c->rc_min_rate = min_bitrate;
  ret = avcodec_open2(c, codec, NULL);
  if (ret < 0) {
    std::cout << "Could not open avcodec" << std::endl;
    logError(ret);

    exit(1);
  }

  AVOutputFormat* of = av_guess_format("mp4", 0, 0);
  avformat_alloc_output_context2(&ofmt_ctx, of, NULL, NULL);
  if (!ofmt_ctx) {
    av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
    logError(ret);

    exit(1);
  }

  video_stream = avformat_new_stream(ofmt_ctx, NULL);
  if (!video_stream) {
    std::cout << "error making stream\n" << std::endl;
    logError(ret);
    exit(1);
  }

  // video_stream->codec->codec_tag = 0;
  video_stream->time_base = c->time_base;
  video_stream->id = ofmt_ctx->nb_streams - 1;
  std::cout << "Setting up output parameters..." << std::endl;
  ret = avcodec_parameters_from_context(video_stream->codecpar, c);
  video_stream->time_base = c->time_base;

  if (ret < 0) {
    std::cout << "Couldn't copy params" << std::endl;
    logError(ret);
    exit(0);
  }

  // unbind
  glBindVertexArray(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  RET_UNDEFINED;
}

GL_METHOD(NvencOpen) { NAPI_ENV;
    REQ_STR_ARG(0, filename);
  
    av_dump_format(ofmt_ctx, 0, filename.c_str(), 1);
    avio_open(&ofmt_ctx->pb, filename.c_str(), AVIO_FLAG_WRITE);
    if(!ofmt_ctx) {
        std::cerr << "couldn't open out_format_context" << std::endl;
        
        exit(1);
    }

    int ret = avformat_write_header(ofmt_ctx, NULL);
    
    if (ret < 0) {
        printf("Couldnt write header\n");

        logError(ret);

        exit(1);
    }
    RET_UNDEFINED;
}
