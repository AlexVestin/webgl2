#include <napi.h>
#include "webgl.h"
#include "decode.h";
#define JS_CONSTANT(x,v) InstanceValue(\
  #x \
  , Napi::Number::New(env, v))


#define JS_GL_CONSTANT(name) JS_CONSTANT(name, GL_ ## name)
#define JS_GL_METHOD(webgl_name, method_name) \
  InstanceMethod( \
    webgl_name \ 
    , &WebGLRenderingContext:: method_name \
  )

Napi::Object WebGLRenderingContext::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "WebGLRenderingContext",
    {      
      JS_GL_METHOD("nvencBindBuffer", NvencBindBuffer),
      JS_GL_METHOD("nvencAddAudioFrame", NvencAddAudioFrame),
      JS_GL_METHOD("nvencEncode", NvencEncode),
      JS_GL_METHOD("nvencInitVideo", NvencInitVideo),
      JS_GL_METHOD("nvencInitAudio", NvencInitAudio),
      JS_GL_METHOD("nvencGetAudioFrameSize", NvencGetFrameSize),
      JS_GL_METHOD("nvencOpen", NvencOpen),
      JS_GL_METHOD("nvencClose", NvencClose),


      // Attribs
      JS_GL_METHOD("bindAttribLocation", BindAttribLocation),
      JS_GL_METHOD("disableVertexAttribArray", DisableVertexAttribArray),
      JS_GL_METHOD("enableVertexAttribArray", EnableVertexAttribArray),
      JS_GL_METHOD("getActiveAttrib", GetActiveAttrib),
      JS_GL_METHOD("getAttribLocation", GetAttribLocation),
      JS_GL_METHOD("getVertexAttrib", GetVertexAttrib),
      JS_GL_METHOD("getVertexAttribOffset", GetVertexAttribOffset),
      JS_GL_METHOD("vertexAttrib1f", VertexAttrib1f),
      JS_GL_METHOD("vertexAttrib1fv", VertexAttrib1fv),
      JS_GL_METHOD("vertexAttrib2f", VertexAttrib2f),
      JS_GL_METHOD("vertexAttrib2fv", VertexAttrib2fv),
      JS_GL_METHOD("vertexAttrib3f", VertexAttrib3f),
      JS_GL_METHOD("vertexAttrib3fv", VertexAttrib3fv),
      JS_GL_METHOD("vertexAttrib4f", VertexAttrib4f),
      JS_GL_METHOD("vertexAttrib4fv", VertexAttrib4fv),
      JS_GL_METHOD("vertexAttribPointer", VertexAttribPointer),
      JS_GL_METHOD("vertexAttribIPointer", VertexAttribIPointer),

      // Blend		
      JS_GL_METHOD("blendColor", BlendColor),
      JS_GL_METHOD("blendEquation", BlendEquation),
      JS_GL_METHOD("blendEquationSeparate", BlendEquationSeparate),
      JS_GL_METHOD("blendFunc", BlendFunc),
      JS_GL_METHOD("blendFuncSeparate", BlendFuncSeparate),
      // VBO
      
      JS_GL_METHOD("createBuffer", CreateBuffer),
      JS_GL_METHOD("deleteBuffer", DeleteBuffer),
      JS_GL_METHOD("isBuffer", IsBuffer),
      JS_GL_METHOD("bindBuffer", BindBuffer),
      JS_GL_METHOD("bindBufferBase", BindBufferBase),
      JS_GL_METHOD("bindBufferRange", BindBufferRange),
      JS_GL_METHOD("bufferData", BufferData),
      JS_GL_METHOD("bufferSubData", BufferSubData),
      JS_GL_METHOD("copyBufferSubData", CopyBufferSubData),
      //JS_GL_METHOD(getBufferSubData),
      JS_GL_METHOD("getBufferParameter", GetBufferParameter),
      
      
      // FBO
      
      JS_GL_METHOD("createFramebuffer", CreateFramebuffer),
      JS_GL_METHOD("deleteFramebuffer", DeleteFramebuffer),
      JS_GL_METHOD("isFramebuffer", IsFramebuffer),
      JS_GL_METHOD("bindFramebuffer", BindFramebuffer),
      JS_GL_METHOD("bindFrameBuffer", BindFrameBuffer),
      JS_GL_METHOD("blitFrameBuffer", BlitFrameBuffer),
      JS_GL_METHOD("checkFramebufferStatus", CheckFramebufferStatus),
      JS_GL_METHOD("framebufferRenderbuffer", FramebufferRenderbuffer),
      JS_GL_METHOD("framebufferTexture2D", FramebufferTexture2D),
      JS_GL_METHOD("getFramebufferAttachmentParameter", GetFramebufferAttachmentParameter),
      
      
      // Program
      
      JS_GL_METHOD("createProgram", CreateProgram),
      JS_GL_METHOD("deleteProgram", DeleteProgram),
      JS_GL_METHOD("isProgram", IsProgram),
      JS_GL_METHOD("getProgramInfoLog", GetProgramInfoLog),
      JS_GL_METHOD("getProgramParameter", GetProgramParameter),
      JS_GL_METHOD("linkProgram", LinkProgram),
      JS_GL_METHOD("useProgram", UseProgram),
      JS_GL_METHOD("validateProgram", ValidateProgram),
      
      
      // RBO
      
      JS_GL_METHOD("createRenderbuffer", CreateRenderbuffer),
      JS_GL_METHOD("deleteRenderbuffer", DeleteRenderbuffer),
      JS_GL_METHOD("isRenderbuffer", IsRenderbuffer),
      JS_GL_METHOD("bindRenderbuffer", BindRenderbuffer),
      JS_GL_METHOD("getRenderbufferParameter", GetRenderbufferParameter),
      JS_GL_METHOD("renderbufferStorage", RenderbufferStorage),
      
      
      // Shader
      
      
      JS_GL_METHOD("deleteShader", DeleteShader),
      JS_GL_METHOD("createShader", CreateShader),
      JS_GL_METHOD("isShader", IsShader),
      JS_GL_METHOD("attachShader", AttachShader),
      JS_GL_METHOD("compileShader", CompileShader),
      JS_GL_METHOD("detachShader", DetachShader),
      JS_GL_METHOD("getAttachedShaders", GetAttachedShaders),
      JS_GL_METHOD("getShaderInfoLog", GetShaderInfoLog),
      JS_GL_METHOD("getShaderParameter", GetShaderParameter),
      JS_GL_METHOD("getShaderSource", GetShaderSource),
      JS_GL_METHOD("shaderSource", ShaderSource),
      
      
      // Stencil
      
      JS_GL_METHOD("clearStencil", ClearStencil),
      JS_GL_METHOD("stencilFunc", StencilFunc),
      JS_GL_METHOD("stencilFuncSeparate", StencilFuncSeparate),
      JS_GL_METHOD("stencilMask", StencilMask),
      JS_GL_METHOD("stencilMaskSeparate", StencilMaskSeparate),
      JS_GL_METHOD("stencilOp", StencilOp),
      JS_GL_METHOD("stencilOpSeparate", StencilOpSeparate),
      
      
      // Texture
      
      JS_GL_METHOD("createTexture", CreateTexture),
      JS_GL_METHOD("deleteTexture", DeleteTexture),
      JS_GL_METHOD("isTexture", IsTexture),
      JS_GL_METHOD("bindTexture", BindTexture),
      JS_GL_METHOD("activeTexture", ActiveTexture),
      JS_GL_METHOD("copyTexImage2D", CopyTexImage2D),
      JS_GL_METHOD("copyTexSubImage2D", CopyTexSubImage2D),
      JS_GL_METHOD("generateMipmap", GenerateMipmap),
      JS_GL_METHOD("getTexParameter", GetTexParameter),
      JS_GL_METHOD("texImage2D", TexImage2D),
      JS_GL_METHOD("texParameterf", TexParameterf),
      JS_GL_METHOD("texParameteri", TexParameteri),
      JS_GL_METHOD("texSubImage2D", TexSubImage2D),

      JS_GL_METHOD("texStorage2D", TexStorage2D),
      JS_GL_METHOD("createSampler", CreateSampler),
      JS_GL_METHOD("samplerParameteri", SamplerParameteri),
      JS_GL_METHOD("bindSampler", BindSampler),


      
      
      // Uniform
      
      JS_GL_METHOD("getActiveUniform", GetActiveUniform),
      JS_GL_METHOD("getUniform", GetUniform),
      JS_GL_METHOD("getUniformLocation", GetUniformLocation),
      JS_GL_METHOD("uniform1f", Uniform1f),
      JS_GL_METHOD("uniform1fv", Uniform1fv),
      JS_GL_METHOD("uniform1i", Uniform1i),
      JS_GL_METHOD("uniform1iv", Uniform1iv),
      JS_GL_METHOD("uniform2f", Uniform2f),
      JS_GL_METHOD("uniform2fv", Uniform2fv),
      JS_GL_METHOD("uniform2i", Uniform2i),
      JS_GL_METHOD("uniform2iv", Uniform2iv),
      JS_GL_METHOD("uniform3f", Uniform3f),
      JS_GL_METHOD("uniform3fv", Uniform3fv),
      JS_GL_METHOD("uniform3i", Uniform3i),
      JS_GL_METHOD("uniform3iv", Uniform3iv),
      JS_GL_METHOD("uniform4f", Uniform4f),
      JS_GL_METHOD("uniform4fv", Uniform4fv),
      JS_GL_METHOD("uniform4i", Uniform4i),
      JS_GL_METHOD("uniform4iv", Uniform4iv),
      JS_GL_METHOD("uniformMatrix2fv", UniformMatrix2fv),
      JS_GL_METHOD("uniformMatrix3fv", UniformMatrix3fv),
      JS_GL_METHOD("uniformMatrix4fv", UniformMatrix4fv),
      
      
      // VAO
      
      JS_GL_METHOD("createVertexArray", CreateVertexArray),
      JS_GL_METHOD("deleteVertexArray", DeleteVertexArray),
      JS_GL_METHOD("isVertexArray", IsVertexArray),
      JS_GL_METHOD("bindVertexArray", BindVertexArray),
      
      
      // Instances
      
      JS_GL_METHOD("drawArraysInstanced", DrawArraysInstanced),
      JS_GL_METHOD("drawElementsInstanced", DrawElementsInstanced),
      JS_GL_METHOD("vertexAttribDivisor", VertexAttribDivisor),
      

      // Transform feedback
      JS_GL_METHOD("createTransformFeedback", CreateTransformFeedback),
      JS_GL_METHOD("deleteTransformFeedback", DeleteTransformFeedback),
      JS_GL_METHOD("isTransformFeedback", IsTransformFeedback),
      JS_GL_METHOD("bindTransformFeedback", BindTransformFeedback),
      JS_GL_METHOD("beginTransformFeedback", BeginTransformFeedback),
      JS_GL_METHOD("endTransformFeedback", EndTransformFeedback),
      JS_GL_METHOD("pauseTransformFeedback", PauseTransformFeedback),
      JS_GL_METHOD("resumeTransformFeedback", ResumeTransformFeedback),
      JS_GL_METHOD("transformFeedbackVaryings", TransformFeedbackVaryings),
      JS_GL_METHOD("getTransformFeedbackVarying", GetTransformFeedbackVarying),
      
      
      // Misc OpenGL Functions
      
      JS_GL_METHOD("clear", Clear),
      JS_GL_METHOD("clearColor", ClearColor),
      JS_GL_METHOD("clearDepth", ClearDepth),
      JS_GL_METHOD("colorMask", ColorMask),
      JS_GL_METHOD("cullFace", CullFace),
      JS_GL_METHOD("depthFunc", DepthFunc),
      JS_GL_METHOD("depthMask", DepthMask),
      JS_GL_METHOD("depthRange", DepthRange),
      JS_GL_METHOD("disable", Disable),
      JS_GL_METHOD("drawArrays", DrawArrays),
      JS_GL_METHOD("drawElements", DrawElements),
      JS_GL_METHOD("enable", Enable),
      JS_GL_METHOD("finish", Finish),
      JS_GL_METHOD("flush", Flush),
      JS_GL_METHOD("frontFace", FrontFace),
      JS_GL_METHOD("getError", GetError),
      JS_GL_METHOD("getParameter", GetParameter),
      JS_GL_METHOD("getRenderTarget", GetRenderTarget),
      JS_GL_METHOD("getSupportedExtensions", GetSupportedExtensions),
      JS_GL_METHOD("hint", Hint),
      JS_GL_METHOD("isEnabled", IsEnabled),
      JS_GL_METHOD("lineWidth", LineWidth),
      JS_GL_METHOD("pixelStorei", PixelStorei),
      JS_GL_METHOD("polygonOffset", PolygonOffset),
      JS_GL_METHOD("readPixels", ReadPixels),
      JS_GL_METHOD("sampleCoverage", SampleCoverage),
      JS_GL_METHOD("scissor", Scissor),
      JS_GL_METHOD("viewport", Viewport),

      // shader precisions
      JS_GL_METHOD("getShaderPrecisionFormat", GetShaderPrecisionFormat),


      
      
      // OpenGL ES 2.1 constants
      
      /* ClearBufferMask */
      JS_GL_CONSTANT(DEPTH_BUFFER_BIT),
      JS_GL_CONSTANT(STENCIL_BUFFER_BIT),
      JS_GL_CONSTANT(COLOR_BUFFER_BIT),
      
      /* Boolean */
      JS_GL_CONSTANT(FALSE),
      JS_GL_CONSTANT(TRUE),
      
      /* BeginMode */
      JS_GL_CONSTANT(POINTS),
      JS_GL_CONSTANT(LINES),
      JS_GL_CONSTANT(LINE_LOOP),
      JS_GL_CONSTANT(LINE_STRIP),
      JS_GL_CONSTANT(TRIANGLES),
      JS_GL_CONSTANT(TRIANGLE_STRIP),
      JS_GL_CONSTANT(TRIANGLE_FAN),
      
      /* AlphaFunction (not supported in ES20) */
      /*			GL_NEVER */
      /*			GL_LESS */
      /*			GL_EQUAL */
      /*			GL_LEQUAL */
      /*			GL_GREATER */
      /*			GL_NOTEQUAL */
      /*			GL_GEQUAL */
      /*			GL_ALWAYS */
      
      /* BlendingFactorDest */
      JS_GL_CONSTANT(ZERO),
      JS_GL_CONSTANT(ONE),
      JS_GL_CONSTANT(SRC_COLOR),
      JS_GL_CONSTANT(ONE_MINUS_SRC_COLOR),
      JS_GL_CONSTANT(SRC_ALPHA),
      JS_GL_CONSTANT(ONE_MINUS_SRC_ALPHA),
      JS_GL_CONSTANT(DST_ALPHA),
      JS_GL_CONSTANT(ONE_MINUS_DST_ALPHA),
      
      /* BlendingFactorSrc */
      /*			GL_ZERO */
      /*			GL_ONE */
      JS_GL_CONSTANT(DST_COLOR),
      JS_GL_CONSTANT(ONE_MINUS_DST_COLOR),
      JS_GL_CONSTANT(SRC_ALPHA_SATURATE),
      /*			GL_SRC_ALPHA */
      /*			GL_ONE_MINUS_SRC_ALPHA */
      /*			GL_DST_ALPHA */
      /*			GL_ONE_MINUS_DST_ALPHA */
      
      /* BlendEquationSeparate */
      JS_GL_CONSTANT(FUNC_ADD),
      JS_GL_CONSTANT(BLEND_EQUATION),
      JS_GL_CONSTANT(BLEND_EQUATION_RGB),		/* same as BLEND_EQUATION */
      JS_GL_CONSTANT(BLEND_EQUATION_ALPHA),
      
      /* BlendSubtract */
      JS_GL_CONSTANT(FUNC_SUBTRACT),
      JS_GL_CONSTANT(FUNC_REVERSE_SUBTRACT),
      
      /* Separate Blend Functions */
      JS_GL_CONSTANT(BLEND_DST_RGB),
      JS_GL_CONSTANT(BLEND_SRC_RGB),
      JS_GL_CONSTANT(BLEND_DST_ALPHA),
      JS_GL_CONSTANT(BLEND_SRC_ALPHA),
      JS_GL_CONSTANT(CONSTANT_COLOR),
      JS_GL_CONSTANT(ONE_MINUS_CONSTANT_COLOR),
      JS_GL_CONSTANT(CONSTANT_ALPHA),
      JS_GL_CONSTANT(ONE_MINUS_CONSTANT_ALPHA),
      JS_GL_CONSTANT(BLEND_COLOR),
      
      /* Buffer Objects */
      JS_GL_CONSTANT(ARRAY_BUFFER),
      JS_GL_CONSTANT(ELEMENT_ARRAY_BUFFER),
      JS_GL_CONSTANT(ARRAY_BUFFER_BINDING),
      JS_GL_CONSTANT(ELEMENT_ARRAY_BUFFER_BINDING),
      
      JS_GL_CONSTANT(STREAM_DRAW),
      JS_GL_CONSTANT(STATIC_DRAW),
      JS_GL_CONSTANT(DYNAMIC_DRAW),
      
      JS_GL_CONSTANT(BUFFER_SIZE),
      JS_GL_CONSTANT(BUFFER_USAGE),
      
      JS_GL_CONSTANT(CURRENT_VERTEX_ATTRIB),
      
      /* CullFaceMode */
      JS_GL_CONSTANT(FRONT),
      JS_GL_CONSTANT(BACK),
      JS_GL_CONSTANT(FRONT_AND_BACK),
      
      /* DepthFunction */
      /*			GL_NEVER */
      /*			GL_LESS */
      /*			GL_EQUAL */
      /*			GL_LEQUAL */
      /*			GL_GREATER */
      /*			GL_NOTEQUAL */
      /*			GL_GEQUAL */
      /*			GL_ALWAYS */
      
      /* EnableCap */
      JS_GL_CONSTANT(TEXTURE_2D),
      JS_GL_CONSTANT(CULL_FACE),
      JS_GL_CONSTANT(BLEND),
      JS_GL_CONSTANT(DITHER),
      JS_GL_CONSTANT(STENCIL_TEST),
      JS_GL_CONSTANT(DEPTH_TEST),
      JS_GL_CONSTANT(SCISSOR_TEST),
      JS_GL_CONSTANT(POLYGON_OFFSET_FILL),
      JS_GL_CONSTANT(SAMPLE_ALPHA_TO_COVERAGE),
      JS_GL_CONSTANT(SAMPLE_COVERAGE),
      
      /* ErrorCode */
      JS_GL_CONSTANT(NO_ERROR),
      JS_GL_CONSTANT(INVALID_ENUM),
      JS_GL_CONSTANT(INVALID_VALUE),
      JS_GL_CONSTANT(INVALID_OPERATION),
      JS_GL_CONSTANT(OUT_OF_MEMORY),
      
      /* FrontFaceDirection */
      JS_GL_CONSTANT(CW),
      JS_GL_CONSTANT(CCW),
      
      /* GetPName */
      JS_GL_CONSTANT(LINE_WIDTH),
      JS_GL_CONSTANT(ALIASED_POINT_SIZE_RANGE),
      JS_GL_CONSTANT(ALIASED_LINE_WIDTH_RANGE),
      JS_GL_CONSTANT(CULL_FACE_MODE),
      JS_GL_CONSTANT(FRONT_FACE),
      JS_GL_CONSTANT(DEPTH_RANGE),
      JS_GL_CONSTANT(DEPTH_WRITEMASK),
      JS_GL_CONSTANT(DEPTH_CLEAR_VALUE),
      JS_GL_CONSTANT(DEPTH_FUNC),
      JS_GL_CONSTANT(STENCIL_CLEAR_VALUE),
      JS_GL_CONSTANT(STENCIL_FUNC),
      JS_GL_CONSTANT(STENCIL_FAIL),
      JS_GL_CONSTANT(STENCIL_PASS_DEPTH_FAIL),
      JS_GL_CONSTANT(STENCIL_PASS_DEPTH_PASS),
      JS_GL_CONSTANT(STENCIL_REF),
      JS_GL_CONSTANT(STENCIL_VALUE_MASK),
      JS_GL_CONSTANT(STENCIL_WRITEMASK),
      JS_GL_CONSTANT(STENCIL_BACK_FUNC),
      JS_GL_CONSTANT(STENCIL_BACK_FAIL),
      JS_GL_CONSTANT(STENCIL_BACK_PASS_DEPTH_FAIL),
      JS_GL_CONSTANT(STENCIL_BACK_PASS_DEPTH_PASS),
      JS_GL_CONSTANT(STENCIL_BACK_REF),
      JS_GL_CONSTANT(STENCIL_BACK_VALUE_MASK),
      JS_GL_CONSTANT(STENCIL_BACK_WRITEMASK),
      JS_GL_CONSTANT(VIEWPORT),
      JS_GL_CONSTANT(SCISSOR_BOX),
      /*			GL_SCISSOR_TEST */
      JS_GL_CONSTANT(COLOR_CLEAR_VALUE),
      JS_GL_CONSTANT(COLOR_WRITEMASK),
      JS_GL_CONSTANT(UNPACK_ALIGNMENT),
      JS_GL_CONSTANT(PACK_ALIGNMENT),
      JS_GL_CONSTANT(MAX_TEXTURE_SIZE),
      JS_GL_CONSTANT(MAX_VIEWPORT_DIMS),
      JS_GL_CONSTANT(SUBPIXEL_BITS),
      JS_GL_CONSTANT(RED_BITS),
      JS_GL_CONSTANT(GREEN_BITS),
      JS_GL_CONSTANT(BLUE_BITS),
      JS_GL_CONSTANT(ALPHA_BITS),
      JS_GL_CONSTANT(DEPTH_BITS),
      JS_GL_CONSTANT(STENCIL_BITS),
      JS_GL_CONSTANT(POLYGON_OFFSET_UNITS),
      /*			GL_POLYGON_OFFSET_FILL */
      JS_GL_CONSTANT(POLYGON_OFFSET_FACTOR),
      JS_GL_CONSTANT(TEXTURE_BINDING_2D),
      JS_GL_CONSTANT(SAMPLE_BUFFERS),
      JS_GL_CONSTANT(SAMPLES),
      JS_GL_CONSTANT(SAMPLE_COVERAGE_VALUE),
      JS_GL_CONSTANT(SAMPLE_COVERAGE_INVERT),
      
      /* GetTextureParameter */
      /*			GL_TEXTURE_MAG_FILTER */
      /*			GL_TEXTURE_MIN_FILTER */
      /*			GL_TEXTURE_WRAP_S */
      /*			GL_TEXTURE_WRAP_T */
      
      JS_GL_CONSTANT(NUM_COMPRESSED_TEXTURE_FORMATS),
      JS_GL_CONSTANT(COMPRESSED_TEXTURE_FORMATS),
      
      /* HintMode */
      JS_GL_CONSTANT(DONT_CARE),
      JS_GL_CONSTANT(FASTEST),
      JS_GL_CONSTANT(NICEST),
      
      /* HintTarget */
      JS_GL_CONSTANT(GENERATE_MIPMAP_HINT),
      
      /* DataType */
      JS_GL_CONSTANT(BYTE),
      JS_GL_CONSTANT(UNSIGNED_BYTE),
      JS_GL_CONSTANT(SHORT),
      JS_GL_CONSTANT(UNSIGNED_SHORT),
      JS_GL_CONSTANT(INT),
      JS_GL_CONSTANT(UNSIGNED_INT),
      JS_GL_CONSTANT(FLOAT),
      JS_GL_CONSTANT(FIXED),
      
      /* PixelFormat */
      JS_GL_CONSTANT(DEPTH_COMPONENT),
      JS_GL_CONSTANT(ALPHA),
      JS_GL_CONSTANT(RGB),
      JS_GL_CONSTANT(RGBA),
      JS_GL_CONSTANT(LUMINANCE),
      JS_GL_CONSTANT(LUMINANCE_ALPHA),
      
      /* PixelType */
      /*			GL_UNSIGNED_BYTE */
      JS_GL_CONSTANT(UNSIGNED_SHORT_4_4_4_4),
      JS_GL_CONSTANT(UNSIGNED_SHORT_5_5_5_1),
      JS_GL_CONSTANT(UNSIGNED_SHORT_5_6_5),
      
      /* Shaders */
      JS_GL_CONSTANT(FRAGMENT_SHADER),
      JS_GL_CONSTANT(VERTEX_SHADER),
      JS_GL_CONSTANT(MAX_VERTEX_ATTRIBS),
      
      JS_GL_CONSTANT(MAX_VERTEX_UNIFORM_VECTORS),
      JS_GL_CONSTANT(MAX_VARYING_VECTORS),
      
      JS_GL_CONSTANT(MAX_COMBINED_TEXTURE_IMAGE_UNITS),
      JS_GL_CONSTANT(MAX_VERTEX_TEXTURE_IMAGE_UNITS),
      JS_GL_CONSTANT(MAX_TEXTURE_IMAGE_UNITS),
      
      JS_GL_CONSTANT(MAX_FRAGMENT_UNIFORM_VECTORS),
      
      JS_GL_CONSTANT(SHADER_TYPE),
      JS_GL_CONSTANT(DELETE_STATUS),
      JS_GL_CONSTANT(LINK_STATUS),
      JS_GL_CONSTANT(VALIDATE_STATUS),
      JS_GL_CONSTANT(ATTACHED_SHADERS),
      JS_GL_CONSTANT(ACTIVE_UNIFORMS),
      JS_GL_CONSTANT(ACTIVE_UNIFORM_MAX_LENGTH),
      JS_GL_CONSTANT(ACTIVE_ATTRIBUTES),
      JS_GL_CONSTANT(ACTIVE_ATTRIBUTE_MAX_LENGTH),
      JS_GL_CONSTANT(SHADING_LANGUAGE_VERSION),
      JS_GL_CONSTANT(CURRENT_PROGRAM),
      
      /* StencilFunction */
      JS_GL_CONSTANT(NEVER),
      JS_GL_CONSTANT(LESS),
      JS_GL_CONSTANT(EQUAL),
      JS_GL_CONSTANT(LEQUAL),
      JS_GL_CONSTANT(GREATER),
      JS_GL_CONSTANT(NOTEQUAL),
      JS_GL_CONSTANT(GEQUAL),
      JS_GL_CONSTANT(ALWAYS),
      
      /* StencilOp */
      /*			GL_ZERO */
      JS_GL_CONSTANT(KEEP),
      JS_GL_CONSTANT(REPLACE),
      JS_GL_CONSTANT(INCR),
      JS_GL_CONSTANT(DECR),
      JS_GL_CONSTANT(INVERT),
      JS_GL_CONSTANT(INCR_WRAP),
      JS_GL_CONSTANT(DECR_WRAP),
      
    //add missing const for point clouds, sprites:
      // JS_GL_CONSTANT(PROGRAM_POINT_SIZE),
      // JS_GL_CONSTANT(POINT_SPRITE),
      
      /* StringName */
      JS_GL_CONSTANT(VENDOR),
      JS_GL_CONSTANT(RENDERER),
      JS_GL_CONSTANT(VERSION),
      JS_GL_CONSTANT(EXTENSIONS),
      
      /* TextureMagFilter */
      JS_GL_CONSTANT(NEAREST),
      JS_GL_CONSTANT(LINEAR),
      
      /* TextureMinFilter */
      /*			GL_NEAREST */
      /*			GL_LINEAR */
      JS_GL_CONSTANT(NEAREST_MIPMAP_NEAREST),
      JS_GL_CONSTANT(LINEAR_MIPMAP_NEAREST),
      JS_GL_CONSTANT(NEAREST_MIPMAP_LINEAR),
      JS_GL_CONSTANT(LINEAR_MIPMAP_LINEAR),
      
      /* TextureParameterName */
      JS_GL_CONSTANT(TEXTURE_MAG_FILTER),
      JS_GL_CONSTANT(TEXTURE_MIN_FILTER),
      JS_GL_CONSTANT(TEXTURE_WRAP_S),
      JS_GL_CONSTANT(TEXTURE_WRAP_T),
      
      /* TextureTarget */
      /*			GL_TEXTURE_2D */
      JS_GL_CONSTANT(TEXTURE),
      
      JS_GL_CONSTANT(TEXTURE_CUBE_MAP),
      JS_GL_CONSTANT(TEXTURE_BINDING_CUBE_MAP),
      JS_GL_CONSTANT(TEXTURE_CUBE_MAP_POSITIVE_X),
      JS_GL_CONSTANT(TEXTURE_CUBE_MAP_NEGATIVE_X),
      JS_GL_CONSTANT(TEXTURE_CUBE_MAP_POSITIVE_Y),
      JS_GL_CONSTANT(TEXTURE_CUBE_MAP_NEGATIVE_Y),
      JS_GL_CONSTANT(TEXTURE_CUBE_MAP_POSITIVE_Z),
      JS_GL_CONSTANT(TEXTURE_CUBE_MAP_NEGATIVE_Z),
      JS_GL_CONSTANT(MAX_CUBE_MAP_TEXTURE_SIZE),
      
      /* TextureUnit */
      JS_GL_CONSTANT(TEXTURE0),
      JS_GL_CONSTANT(TEXTURE1),
      JS_GL_CONSTANT(TEXTURE2),
      JS_GL_CONSTANT(TEXTURE3),
      JS_GL_CONSTANT(TEXTURE4),
      JS_GL_CONSTANT(TEXTURE5),
      JS_GL_CONSTANT(TEXTURE6),
      JS_GL_CONSTANT(TEXTURE7),
      JS_GL_CONSTANT(TEXTURE8),
      JS_GL_CONSTANT(TEXTURE9),
      JS_GL_CONSTANT(TEXTURE10),
      JS_GL_CONSTANT(TEXTURE11),
      JS_GL_CONSTANT(TEXTURE12),
      JS_GL_CONSTANT(TEXTURE13),
      JS_GL_CONSTANT(TEXTURE14),
      JS_GL_CONSTANT(TEXTURE15),
      JS_GL_CONSTANT(TEXTURE16),
      JS_GL_CONSTANT(TEXTURE17),
      JS_GL_CONSTANT(TEXTURE18),
      JS_GL_CONSTANT(TEXTURE19),
      JS_GL_CONSTANT(TEXTURE20),
      JS_GL_CONSTANT(TEXTURE21),
      JS_GL_CONSTANT(TEXTURE22),
      JS_GL_CONSTANT(TEXTURE23),
      JS_GL_CONSTANT(TEXTURE24),
      JS_GL_CONSTANT(TEXTURE25),
      JS_GL_CONSTANT(TEXTURE26),
      JS_GL_CONSTANT(TEXTURE27),
      JS_GL_CONSTANT(TEXTURE28),
      JS_GL_CONSTANT(TEXTURE29),
      JS_GL_CONSTANT(TEXTURE30),
      JS_GL_CONSTANT(TEXTURE31),
      JS_GL_CONSTANT(ACTIVE_TEXTURE),
      
      /* TextureWrapMode */
      //JS_GL_CONSTANT(CLAMP),
      //JS_GL_CONSTANT(CLAMP_TO_BORDER),
      JS_GL_CONSTANT(CLAMP_TO_EDGE),
      JS_GL_CONSTANT(MIRRORED_REPEAT),
      JS_GL_CONSTANT(REPEAT),
      
      /* Uniform Types */
      JS_GL_CONSTANT(FLOAT_VEC2),
      JS_GL_CONSTANT(FLOAT_VEC3),
      JS_GL_CONSTANT(FLOAT_VEC4),
      JS_GL_CONSTANT(INT_VEC2),
      JS_GL_CONSTANT(INT_VEC3),
      JS_GL_CONSTANT(INT_VEC4),
      JS_GL_CONSTANT(BOOL),
      JS_GL_CONSTANT(BOOL_VEC2),
      JS_GL_CONSTANT(BOOL_VEC3),
      JS_GL_CONSTANT(BOOL_VEC4),
      JS_GL_CONSTANT(FLOAT_MAT2),
      JS_GL_CONSTANT(FLOAT_MAT3),
      JS_GL_CONSTANT(FLOAT_MAT4),
      JS_GL_CONSTANT(SAMPLER_2D),
      JS_GL_CONSTANT(SAMPLER_CUBE),
      
      /* Vertex Arrays */
      JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_ENABLED),
      JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_SIZE),
      JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_STRIDE),
      JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_TYPE),
      JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_NORMALIZED),
      JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_POINTER),
      JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_BUFFER_BINDING),
      
      /* Read Format */
      
      JS_GL_CONSTANT(IMPLEMENTATION_COLOR_READ_TYPE),
      JS_GL_CONSTANT(IMPLEMENTATION_COLOR_READ_FORMAT),
      
      
      /* Shader Source */
      JS_GL_CONSTANT(COMPILE_STATUS),
      JS_GL_CONSTANT(INFO_LOG_LENGTH),
      JS_GL_CONSTANT(SHADER_SOURCE_LENGTH),
      
      JS_GL_CONSTANT(SHADER_COMPILER),
      
      
      /* Shader Binary */
      
      JS_GL_CONSTANT(SHADER_BINARY_FORMATS),
      JS_GL_CONSTANT(NUM_SHADER_BINARY_FORMATS),
      
      
      /* Shader Precision-Specified Types */
      
      JS_GL_CONSTANT(LOW_FLOAT),
      JS_GL_CONSTANT(MEDIUM_FLOAT),
      JS_GL_CONSTANT(HIGH_FLOAT),
      JS_GL_CONSTANT(LOW_INT),
      JS_GL_CONSTANT(MEDIUM_INT),
      JS_GL_CONSTANT(HIGH_INT),
      
      
      /* Framebuffer Object. */
      JS_GL_CONSTANT(FRAMEBUFFER),
      JS_GL_CONSTANT(RENDERBUFFER),
      
      JS_GL_CONSTANT(RGBA4),
      JS_GL_CONSTANT(RGB5_A1),
      
      //JS_GL_CONSTANT(RGB565),
      
      JS_GL_CONSTANT(DEPTH_COMPONENT16),
      //JS_GL_CONSTANT(STENCIL_INDEX),
      JS_GL_CONSTANT(STENCIL_INDEX8),
      
      JS_GL_CONSTANT(DEPTH_STENCIL),
      JS_GL_CONSTANT(DEPTH24_STENCIL8),
      
      
      JS_GL_CONSTANT(RENDERBUFFER_WIDTH),
      JS_GL_CONSTANT(RENDERBUFFER_HEIGHT),
      JS_GL_CONSTANT(RENDERBUFFER_INTERNAL_FORMAT),
      JS_GL_CONSTANT(RENDERBUFFER_RED_SIZE),
      JS_GL_CONSTANT(RENDERBUFFER_GREEN_SIZE),
      JS_GL_CONSTANT(RENDERBUFFER_BLUE_SIZE),
      JS_GL_CONSTANT(RENDERBUFFER_ALPHA_SIZE),
      JS_GL_CONSTANT(RENDERBUFFER_DEPTH_SIZE),
      JS_GL_CONSTANT(RENDERBUFFER_STENCIL_SIZE),
      
      JS_GL_CONSTANT(FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE),
      JS_GL_CONSTANT(FRAMEBUFFER_ATTACHMENT_OBJECT_NAME),
      JS_GL_CONSTANT(FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL),
      JS_GL_CONSTANT(FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE),
      
      JS_GL_CONSTANT(COLOR_ATTACHMENT0),
      JS_GL_CONSTANT(DEPTH_ATTACHMENT),
      JS_GL_CONSTANT(STENCIL_ATTACHMENT),
      
      JS_GL_CONSTANT(DEPTH_STENCIL_ATTACHMENT),
      
      JS_GL_CONSTANT(VERTEX_ARRAY_BINDING),
      
      
      JS_GL_CONSTANT(NONE),
      
      JS_GL_CONSTANT(FRAMEBUFFER_COMPLETE),
      JS_GL_CONSTANT(FRAMEBUFFER_INCOMPLETE_ATTACHMENT),
      JS_GL_CONSTANT(FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT),
      
      //JS_GL_CONSTANT(FRAMEBUFFER_INCOMPLETE_DIMENSIONS),
      
      JS_GL_CONSTANT(FRAMEBUFFER_UNSUPPORTED),
      
      JS_GL_CONSTANT(FRAMEBUFFER_BINDING),
      JS_GL_CONSTANT(RENDERBUFFER_BINDING),
      JS_GL_CONSTANT(MAX_RENDERBUFFER_SIZE),
      
      JS_GL_CONSTANT(INVALID_FRAMEBUFFER_OPERATION),

      // Transform feedback

      JS_GL_CONSTANT(TRANSFORM_FEEDBACK_BUFFER_MODE),
      JS_GL_CONSTANT(MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS),
      JS_GL_CONSTANT(TRANSFORM_FEEDBACK_VARYINGS),
      JS_GL_CONSTANT(TRANSFORM_FEEDBACK_BUFFER_START),
      JS_GL_CONSTANT(TRANSFORM_FEEDBACK_BUFFER_SIZE),
      JS_GL_CONSTANT(TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN),
      JS_GL_CONSTANT(MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS),
      JS_GL_CONSTANT(MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS),
      JS_GL_CONSTANT(INTERLEAVED_ATTRIBS),
      JS_GL_CONSTANT(SEPARATE_ATTRIBS),
      JS_GL_CONSTANT(TRANSFORM_FEEDBACK_BUFFER),
      JS_GL_CONSTANT(TRANSFORM_FEEDBACK_BUFFER_BINDING),
      JS_GL_CONSTANT(TRANSFORM_FEEDBACK),
      JS_GL_CONSTANT(TRANSFORM_FEEDBACK_PAUSED),
      JS_GL_CONSTANT(TRANSFORM_FEEDBACK_ACTIVE),
      JS_GL_CONSTANT(TRANSFORM_FEEDBACK_BINDING),
      
      /* WebGL-specific enums */
      JS_CONSTANT(UNPACK_FLIP_Y_WEBGL, 0x9240),
      JS_CONSTANT(UNPACK_PREMULTIPLY_ALPHA_WEBGL, 0x9241),
      JS_CONSTANT(CONTEXT_LOST_WEBGL, 0x9242),
      JS_CONSTANT(UNPACK_COLORSPACE_CONVERSION_WEBGL, 0x9243),
      JS_CONSTANT(BROWSER_DEFAULT_WEBGL, 0x9244),
      
      //////////////////////////////
      // NOT in WebGL spec
      //////////////////////////////
      
      // PBO
      JS_CONSTANT(PIXEL_PACK_BUFFER, 0x88EB),
      JS_CONSTANT(PIXEL_UNPACK_BUFFER, 0x88EC),
      JS_CONSTANT(PIXEL_PACK_BUFFER_BINDING, 0x88ED),
      JS_CONSTANT(PIXEL_UNPACK_BUFFER_BINDING, 0x88EF),
    });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("WebGLRenderingContext", func);
  return exports;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  VideoDecodingContext::Init(env, exports);
  WebGLRenderingContext::Init(env, exports);

  return exports;
}

NODE_API_MODULE(addon, InitAll)