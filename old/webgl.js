"use strict";

const WebGLRenderingContext = require("../gl");
function wrap(gl) {
  gl.WebGLRenderingContext = function WebGLRenderingContext(_) {
    this._ = _;
  };
  gl.WebGLSampler = function WebGLSampler(_) {
    this._ = _;
    this.name = _.name;
  };

  gl.WebGLProgram = function WebGLProgram(_) {
    this._ = _;
  };
  gl.WebGLShader = function WebGLShader(_) {
    this._ = _;
  };
  gl.WebGLBuffer = function WebGLBuffer(_) {
    this._ = _;
  };
  gl.WebGLVertexArray = function WebGLVertexArray(_) {
    this._ = _;
  };
  gl.WebGLFramebuffer = function WebGLFramebuffer(_) {
    this._ = _;
  };
  gl.WebGLRenderbuffer = function WebGLRenderbuffer(_) {
    this._ = _;
  };
  gl.WebGLTexture = function WebGLTexture(_) {
    this._ = _;
  };
  gl.WebGLUniformLocation = function WebGLUniformLocation(_) {
    this._ = _;
  };
  gl.WebGLActiveInfo = function WebGLActiveInfo(_) {
    this._ = _;
    this.size = _.size;
    this.type = _.type;
    this.name = _.name;
  };
  gl.WebGLTransformFeedback = function(_) {
    this._ = _;
  };

  // Global scope constructors for browser-style libs
  global.WebGLRenderingContext = gl.WebGLRenderingContext;
  global.WebGLProgram = gl.WebGLProgram;
  global.WebGLShader = gl.WebGLShader;
  global.WebGLBuffer = gl.WebGLBuffer;
  global.WebGLVertexArrayObject = gl.WebGLVertexArrayObject;
  global.WebGLFramebuffer = gl.WebGLFramebuffer;
  global.WebGLSampler = gl.WebGLSampler;
  global.WebGLRenderbuffer = gl.WebGLRenderbuffer;
  global.WebGLTexture = gl.WebGLTexture;
  global.WebGLUniformLocation = gl.WebGLUniformLocation;
  global.WebGLActiveInfo = gl.WebGLActiveInfo;
  global.WebGLTransformFeedback = gl.WebGLTransformFeedback;

  module.exports = gl;

  const enforceF32 = v => (v instanceof Array ? new Float32Array(v) : v);

  const enforceId = x => (x ? x._ : 0);

  const enforceBool = x => (typeof x === "boolean" ? (x ? 1 : 0) : x);

  ////////////////////////////////////////////////////////////////////////////////

  // Attrib

  const _bindAttribLocation = gl.bindAttribLocation;
  gl.bindAttribLocation = (program, index, name) =>
    _bindAttribLocation(enforceId(program), index, name);

  const _getActiveAttrib = gl.getActiveAttrib;
  gl.getActiveAttrib = (program, index) =>
    new gl.WebGLActiveInfo(_getActiveAttrib(enforceId(program), index));

  const _getAttribLocation = gl.getAttribLocation;
  gl.getAttribLocation = (program, name) =>
    _getAttribLocation(enforceId(program), name);

  const _vertexAttrib1fv = gl.vertexAttrib1fv;
  gl.vertexAttrib1fv = (indx, v) => {
    return _vertexAttrib1fv(indx, enforceF32(v));
  };

  const _vertexAttrib2fv = gl.vertexAttrib2fv;
  gl.vertexAttrib2fv = (indx, v) => {
    return _vertexAttrib2fv(indx, enforceF32(v));
  };

  const _vertexAttrib3fv = gl.vertexAttrib3fv;
  gl.vertexAttrib3fv = (indx, v) => {
    return _vertexAttrib3fv(indx, enforceF32(v));
  };

  const _vertexAttrib4fv = gl.vertexAttrib4fv;
  gl.vertexAttrib4fv = (indx, v) => {
    return _vertexAttrib4fv(indx, enforceF32(v));
  };

  // VBO

  const _createBuffer = gl.createBuffer;
  gl.createBuffer = () => new gl.WebGLBuffer(_createBuffer());

  const _deleteBuffer = gl.deleteBuffer;
  gl.deleteBuffer = buffer => _deleteBuffer(enforceId(buffer));

  const _isBuffer = gl.isBuffer;
  gl.isBuffer = buffer => _isBuffer(enforceId(buffer));

  const _bindBuffer = gl.bindBuffer;
  gl.bindBuffer = (target, buffer) => _bindBuffer(target, enforceId(buffer));

  const _bindBufferBase = gl.bindBufferBase;
  gl.bindBufferBase = (target, index, buffer) =>
    _bindBufferBase(target, index, enforceId(buffer));

  const _bindBufferRange = gl.bindBufferRange;
  gl.bindBufferRange = (target, index, buffer, offset, size) =>
    _bindBufferRange(target, index, enforceId(buffer), offset, size);

  // FBO

  const _createFramebuffer = gl.createFramebuffer;
  gl.createFramebuffer = () => new gl.WebGLFramebuffer(_createFramebuffer());

  const _deleteFramebuffer = gl.deleteFramebuffer;
  gl.deleteFramebuffer = framebuffer =>
    _deleteFramebuffer(enforceId(framebuffer));

  const _isFramebuffer = gl.isFramebuffer;
  gl.isFramebuffer = framebuffer => _isFramebuffer(enforceId(framebuffer));

  const _bindFramebuffer = gl.bindFramebuffer;
  gl.bindFramebuffer = (target, framebuffer) =>
    _bindFramebuffer(target, enforceId(framebuffer));

  const _framebufferRenderbuffer = gl.framebufferRenderbuffer;
  gl.framebufferRenderbuffer = (
    target,
    attachment,
    renderbuffertarget,
    renderbuffer
  ) =>
    _framebufferRenderbuffer(
      target,
      attachment,
      renderbuffertarget,
      enforceId(renderbuffer)
    );

  const _framebufferTexture2D = gl.framebufferTexture2D;
  gl.framebufferTexture2D = (target, attachment, textarget, texture, level) =>
    _framebufferTexture2D(
      target,
      attachment,
      textarget,
      enforceId(texture),
      level
    );

  // Program

  const _createProgram = gl.createProgram;
  gl.createProgram = () => new gl.WebGLProgram(_createProgram());

  const _deleteProgram = gl.deleteProgram;
  gl.deleteProgram = program => _deleteProgram(enforceId(program));

  const _isProgram = gl.isProgram;
  gl.isProgram = program => _isProgram(enforceId(program));

  const _getProgramInfoLog = gl.getProgramInfoLog;
  gl.getProgramInfoLog = program => _getProgramInfoLog(enforceId(program));

  const _getProgramParameter = gl.getProgramParameter;
  gl.getProgramParameter = (program, pname) =>
    _getProgramParameter(enforceId(program), pname);

  const _linkProgram = gl.linkProgram;
  gl.linkProgram = program => _linkProgram(enforceId(program));

  const _useProgram = gl.useProgram;
  gl.useProgram = program => _useProgram(enforceId(program));

  const _validateProgram = gl.validateProgram;
  gl.validateProgram = program => _validateProgram(enforceId(program));

  // RBO

  const _createRenderbuffer = gl.createRenderbuffer;
  gl.createRenderbuffer = () => new gl.WebGLRenderbuffer(_createRenderbuffer());

  const _deleteRenderbuffer = gl.deleteRenderbuffer;
  gl.deleteRenderbuffer = renderbuffer =>
    _deleteRenderbuffer(enforceId(renderbuffer));

  const _isRenderbuffer = gl.isRenderbuffer;
  gl.isRenderbuffer = renderbuffer => _isRenderbuffer(enforceId(renderbuffer));

  const _bindRenderbuffer = gl.bindRenderbuffer;
  gl.bindRenderbuffer = (target, renderbuffer) =>
    _bindRenderbuffer(target, enforceId(renderbuffer));

  // Sampler
  const _createSampler = gl.createSampler;
  gl.createSampler = () => {
    const samp = new gl.WebGLSampler(_createSampler());
    return samp;
  };

  const _samplerParameteri = gl.samplerParameteri;
  gl.samplerParameteri = (sampler, pname, param) => {
    _samplerParameteri(sampler.name, pname, param);
  };

  const _bindSampler = gl.bindSampler;
  gl.bindSampler = (unit, sampler) => {
    _bindSampler(enforceId(unit), sampler.name);
  };

  // Shader

  const _createShader = gl.createShader;
  gl.createShader = type => new gl.WebGLShader(_createShader(type));

  const _deleteShader = gl.deleteShader;
  gl.deleteShader = shader => _deleteShader(enforceId(shader));

  const _isShader = gl.isShader;
  gl.isShader = shader => _isShader(enforceId(shader));

  const _attachShader = gl.attachShader;
  gl.attachShader = (program, shader) =>
    _attachShader(enforceId(program), enforceId(shader));

  const _compileShader = gl.compileShader;
  gl.compileShader = shader => _compileShader(enforceId(shader));

  const _detachShader = gl.detachShader;
  gl.detachShader = (program, shader) =>
    _detachShader(enforceId(program), enforceId(shader));

  const _getAttachedShaders = gl.getAttachedShaders;
  gl.getAttachedShaders = program => _getAttachedShaders(enforceId(program));

  const _getShaderInfoLog = gl.getShaderInfoLog;
  gl.getShaderInfoLog = shader => _getShaderInfoLog(enforceId(shader));

  const _getShaderParameter = gl.getShaderParameter;
  gl.getShaderParameter = (shader, pname) =>
    _getShaderParameter(enforceId(shader), pname);

  const _getShaderSource = gl.getShaderSource;
  gl.getShaderSource = shader => _getShaderSource(enforceId(shader));

  gl._shaderSource = gl.shaderSource;
  gl.shaderSource = (shaderId, code) =>
    gl._shaderSource(
      enforceId(shaderId),
      code
      /*code.replace(
            /^\s*?(#version|precision).*?($|;)/gm, ''
        ).replace(
            /^/, '#version 120\n'
        ).replace(
            /gl_FragDepthEXT/g, 'gl_FragDepth'
        ).replace(
            '#extension GL_EXT_frag_depth : enable', ''
        )*/
    );

  // Texture

  const _createTexture = gl.createTexture;
  gl.createTexture = () => new gl.WebGLTexture(_createTexture());

  const _deleteTexture = gl.deleteTexture;
  gl.deleteTexture = texture => _deleteTexture(enforceId(texture));

  const _isTexture = gl.isTexture;
  gl.isTexture = texture => _isTexture(enforceId(texture));

  const _bindTexture = gl.bindTexture;
  gl.bindTexture = (target, texture) =>
    _bindTexture(target, enforceId(texture));

  const _texImage2D = gl.texImage2D;
  gl.texImage2D = function(
    target,
    level,
    internalformat,
    width,
    height,
    border,
    format,
    type,
    pixels
  ) {
    if (arguments.length === 6) {
      // width is now format, height is now type, and border is now pixels
      pixels = border;
      type = height;
      format = width;
      return _texImage2D(
        target,
        level,
        internalformat,
        pixels.width,
        pixels.height,
        0,
        format,
        type,
        pixels
      );
    }

    if (arguments.length === 9) {
      return _texImage2D(
        target,
        level,
        internalformat,
        width,
        height,
        border,
        format,
        type,
        pixels
      );
    }

    throw new TypeError("Function texImage2D() takes 6 or 9 arguments.");
  };

  // Uniform

  const _getActiveUniform = gl.getActiveUniform;
  gl.getActiveUniform = (program, index) =>
    new gl.WebGLActiveInfo(_getActiveUniform(enforceId(program), index));

  const _getUniform = gl.getUniform;
  gl.getUniform = (program, location) =>
    _getUniform(enforceId(program), enforceId(location));

  const _getUniformLocation = gl.getUniformLocation;
  gl.getUniformLocation = (program, name) =>
    new gl.WebGLUniformLocation(_getUniformLocation(enforceId(program), name));

  const _uniform1f = gl.uniform1f;
  gl.uniform1f = (location, x) => _uniform1f(enforceId(location), x);

  const _uniform1fv = gl.uniform1fv;
  gl.uniform1fv = (location, v) => {
    return _uniform1fv(enforceId(location), enforceF32(v));
  };

  const _uniform1i = gl.uniform1i;
  gl.uniform1i = (location, x) =>
    _uniform1i(enforceId(location), enforceBool(x));

  const _uniform1iv = gl.uniform1iv;
  gl.uniform1iv = (location, v) => {
    return _uniform1iv(enforceId(location), enforceF32(v));
  };

  const _uniform2f = gl.uniform2f;
  gl.uniform2f = (location, x, y) => _uniform2f(enforceId(location), x, y);

  const _uniform2fv = gl.uniform2fv;
  gl.uniform2fv = (location, v) => {
    return _uniform2fv(enforceId(location), enforceF32(v));
  };

  const _uniform2i = gl.uniform2i;
  gl.uniform2i = (location, x, y) => _uniform2i(enforceId(location), x, y);

  const _uniform2iv = gl.uniform2iv;
  gl.uniform2iv = (location, v) => {
    return _uniform2iv(enforceId(location), enforceF32(v));
  };

  const _uniform3f = gl.uniform3f;
  gl.uniform3f = (location, x, y, z) =>
    _uniform3f(enforceId(location), x, y, z);

  const _uniform3fv = gl.uniform3fv;
  gl.uniform3fv = (location, v) => {
    return _uniform3fv(enforceId(location), enforceF32(v));
  };

  const _uniform3i = gl.uniform3i;
  gl.uniform3i = (location, x, y, z) =>
    _uniform3i(enforceId(location), x, y, z);

  const _uniform3iv = gl.uniform3iv;
  gl.uniform3iv = (location, v) => {
    return _uniform3iv(enforceId(location), enforceF32(v));
  };

  const _uniform4f = gl.uniform4f;
  gl.uniform4f = (location, x, y, z, w) =>
    _uniform4f(enforceId(location), x, y, z, w);

  const _uniform4fv = gl.uniform4fv;
  gl.uniform4fv = (location, v) => {
    return _uniform4fv(enforceId(location), enforceF32(v));
  };

  const _uniform4i = gl.uniform4i;
  gl.uniform4i = (location, x, y, z, w) =>
    _uniform4i(enforceId(location), x, y, z, w);

  const _uniform4iv = gl.uniform4iv;
  gl.uniform4iv = (location, v) => {
    return _uniform4iv(enforceId(location), enforceF32(v));
  };

  const _uniformMatrix2fv = gl.uniformMatrix2fv;
  gl.uniformMatrix2fv = (location, transpose, v) => {
    return _uniformMatrix2fv(enforceId(location), transpose, enforceF32(v));
  };

  const _uniformMatrix3fv = gl.uniformMatrix3fv;
  gl.uniformMatrix3fv = (location, transpose, v) => {
    return _uniformMatrix3fv(enforceId(location), transpose, enforceF32(v));
  };

  const _uniformMatrix4fv = gl.uniformMatrix4fv;
  gl.uniformMatrix4fv = (location, transpose, v) => {
    return _uniformMatrix4fv(enforceId(location), transpose, enforceF32(v));
  };

  // VAO

  const _createVertexArray = gl.createVertexArray;
  gl.createVertexArray = () => new gl.WebGLVertexArray(_createVertexArray());

  const _deleteVertexArray = gl.deleteVertexArray;
  gl.deleteVertexArray = vao => _deleteVertexArray(enforceId(vao));

  const _isVertexArray = gl.isVertexArray;
  gl.isVertexArray = vao => _isVertexArray(enforceId(vao));

  const _bindVertexArray = gl.bindVertexArray;
  gl.bindVertexArray = vao => _bindVertexArray(enforceId(vao));

  // Transform feedback

  const _createTransformFeedback = gl.createTransformFeedback;
  gl.createTransformFeedback = () =>
    new gl.WebGLTransformFeedback(_createTransformFeedback());

  const _deleteTransformFeedback = gl.deleteTransformFeedback;
  gl.deleteTransformFeedback = transformFeedback =>
    _deleteTransformFeedback(enforceId(transformFeedback));

  const _isTransformFeedback = gl.isTransformFeedback;
  gl.isTransformFeedback = transformFeedback =>
    _isTransformFeedback(enforceId(transformFeedback));

  const _bindTransformFeedback = gl.bindTransformFeedback;
  gl.bindTransformFeedback = (target, transformFeedback) =>
    _bindTransformFeedback(target, enforceId(transformFeedback));

  const _transformFeedbackVaryings = gl.transformFeedbackVaryings;
  gl.transformFeedbackVaryings = (program, varyings, mode) =>
    _transformFeedbackVaryings(enforceId(program), varyings, mode);

  const _getTransformFeedbackVarying = gl.getTransformFeedbackVarying;
  gl.getTransformFeedbackVarying = (program, location) =>
    new gl.WebGLActiveInfo(
      _getTransformFeedbackVarying(enforceId(program), location)
    );

  // Misc OpenGL Functions

  const _getParameter = gl.getParameter;
  gl.getParameter = pname => {
    switch (pname) {
      case gl.VERSION:
        return "OpenGL ES 2.0 Chromium";
      case 0x9245:
        return origGl.getParameter(0x1f00); // UNMASKED VENDOR
      case 0x9246:
        return origGl.getParameter(0x1f01); // UNMASKED RENDERER
      case 0x1f01:
        return "WebKit"; // VENDOR
      case 0x1f00:
        return "WebKit WebGL"; // RENDERER
      default:
        return origGl.getParameter(pname);
    }
  };
  const _pixelStorei = gl.pixelStorei;
  gl.pixelStorei = (pname, param) => _pixelStorei(pname, enforceBool(param));

  const _viewport = gl.viewport;
  gl.viewport = (x, y, width, height) => _viewport(x, y, width, height);

  Object.defineProperty(gl, "drawingBufferWidth", {
    get: () => gl.getParameter(gl.VIEWPORT)[2]
  });
  Object.defineProperty(gl, "drawingBufferHeight", {
    get: () => gl.getParameter(gl.VIEWPORT)[3]
  });

  ////////////////////////////////////////////////////////////////////////////////

  const extensions = {
    ANGLE_instanced_arrays: {
      VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE: 0x88fe,
      drawArraysInstancedANGLE: gl.drawArraysInstanced,
      drawElementsInstancedANGLE: gl.drawElementsInstanced,
      vertexAttribDivisorANGLE: gl.vertexAttribDivisor
    },
    EXT_blend_minmax: {
      MIN_EXT: 0x8007,
      MAX_EXT: 0x8008
    },
    EXT_color_buffer_float: {},
    EXT_color_buffer_half_float: {
      RGBA16F_EXT: 0x881a,
      RGB16F_EXT: 0x881b,
      FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE_EXT: 0x8211,
      UNSIGNED_NORMALIZED_EXT: 0x8c17
    },
    EXT_disjoint_timer_query: {
      QUERY_COUNTER_BITS_EXT: 0x8864,
      CURRENT_QUERY_EXT: 0x8865,
      QUERY_RESULT_EXT: 0x8866,
      QUERY_RESULT_AVAILABLE_EXT: 0x8867,
      TIME_ELAPSED_EXT: 0x88bf,
      TIMESTAMP_EXT: 0x8e28,
      GPU_DISJOINT_EXT: 0x8fbb,
      createQueryEXT() {},
      deleteQueryEXT() {},
      isQueryEXT() {},
      beginQueryEXT() {},
      endQueryEXT() {},
      queryCounterEXT() {},
      getQueryEXT() {},
      getQueryObjectEXT() {}
    },
    EXT_frag_depth: {},
    EXT_sRGB: {
      SRGB_EXT: 0x8c40,
      SRGB_ALPHA_EXT: 0x8c42,
      SRGB8_ALPHA8_EXT: 0x8c43,
      FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_EXT: 0x8210
    },
    EXT_shader_texture_lod: {},
    EXT_texture_filter_anisotropic: {
      MAX_TEXTURE_MAX_ANISOTROPY_EXT: 0x84ff,
      TEXTURE_MAX_ANISOTROPY_EXT: 0x84fe
    },
    OES_element_index_uint: {},
    OES_standard_derivatives: {
      FRAGMENT_SHADER_DERIVATIVE_HINT_OES: 0x8b8b
    },
    OES_texture_float: {},
    OES_texture_float_linear: {},
    OES_texture_half_float: {
      HALF_FLOAT_OES: 0x8d61
    },
    OES_texture_half_float_linear: {},
    OES_vertex_array_object: {
      VERTEX_ARRAY_BINDING_OES: 0x85b5,
      createVertexArrayOES: gl.createVertexArray,
      deleteVertexArrayOES: gl.deleteVertexArray,
      isVertexArrayOES: gl.isVertexArray,
      bindVertexArrayOES: gl.bindVertexArray
    },
    WEBGL_color_buffer_float: {
      RGBA32F_EXT: 0x8814,
      RGB32F_EXT: 0x8815,
      FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE_EXT: 0x8211,
      UNSIGNED_NORMALIZED_EXT: 0x8c17
    },
    WEBGL_compressed_texture_astc: {
      getSupportedProfiles() {},
      COMPRESSED_RGBA_ASTC_4x4_KHR: 0x93b0,
      COMPRESSED_RGBA_ASTC_5x4_KHR: 0x93b1,
      COMPRESSED_RGBA_ASTC_5x5_KHR: 0x93b2,
      COMPRESSED_RGBA_ASTC_6x5_KHR: 0x93b3,
      COMPRESSED_RGBA_ASTC_6x6_KHR: 0x93b4,
      COMPRESSED_RGBA_ASTC_8x5_KHR: 0x93b5,
      COMPRESSED_RGBA_ASTC_8x6_KHR: 0x93b6,
      COMPRESSED_RGBA_ASTC_8x8_KHR: 0x93b7,
      COMPRESSED_RGBA_ASTC_10x5_KHR: 0x93b8,
      COMPRESSED_RGBA_ASTC_10x6_KHR: 0x93b9,
      COMPRESSED_RGBA_ASTC_10x8_KHR: 0x93ba,
      COMPRESSED_RGBA_ASTC_10x10_KHR: 0x93bb,
      COMPRESSED_RGBA_ASTC_12x10_KHR: 0x93bc,
      COMPRESSED_RGBA_ASTC_12x12_KHR: 0x93bd,
      COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR: 0x93d0,
      COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR: 0x93d1,
      COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR: 0x93d2,
      COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR: 0x93d3,
      COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR: 0x93d4,
      COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR: 0x93d5,
      COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR: 0x93d6,
      COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR: 0x93d7,
      COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR: 0x93d8,
      COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR: 0x93d9,
      COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR: 0x93da,
      COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR: 0x93db,
      COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR: 0x93dc,
      COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR: 0x93dd
    },
    WEBGL_compressed_texture_atc: {
      COMPRESSED_RGB_ATC_WEBGL: 0x8c92,
      COMPRESSED_RGBA_ATC_EXPLICIT_ALPHA_WEBGL: 0x8c92,
      COMPRESSED_RGBA_ATC_INTERPOLATED_ALPHA_WEBGL: 0x87ee
    },
    WEBGL_compressed_texture_etc: {
      COMPRESSED_R11_EAC: 0x9270,
      COMPRESSED_SIGNED_R11_EAC: 0x9271,
      COMPRESSED_RG11_EAC: 0x9272,
      COMPRESSED_SIGNED_RG11_EAC: 0x9273,
      COMPRESSED_RGB8_ETC2: 0x9274,
      COMPRESSED_RGBA8_ETC2_EAC: 0x9275,
      COMPRESSED_SRGB8_ETC2: 0x9276,
      COMPRESSED_SRGB8_ALPHA8_ETC2_EAC: 0x9277,
      COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2: 0x9278,
      COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2: 0x9279
    },
    WEBGL_compressed_texture_etc1: {
      COMPRESSED_RGB_ETC1_WEBGL: 0x8d64
    },
    WEBGL_compressed_texture_pvrtc: {
      COMPRESSED_RGB_PVRTC_4BPPV1_IMG: 0x8c00,
      COMPRESSED_RGBA_PVRTC_4BPPV1_IMG: 0x8c02,
      COMPRESSED_RGB_PVRTC_2BPPV1_IMG: 0x8c01,
      COMPRESSED_RGBA_PVRTC_2BPPV1_IMG: 0x8c03
    },
    WEBGL_compressed_texture_s3tc: {
      COMPRESSED_RGB_S3TC_DXT1_EXT: 0x83f0,
      COMPRESSED_RGBA_S3TC_DXT1_EXT: 0x83f1,
      COMPRESSED_RGBA_S3TC_DXT3_EXT: 0x83f2,
      COMPRESSED_RGBA_S3TC_DXT5_EXT: 0x83f3
    },
    WEBGL_compressed_texture_s3tc_srgb: {
      COMPRESSED_SRGB_S3TC_DXT1_EXT: 0x8c4c,
      COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT: 0x8c4d,
      COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT: 0x8c4e,
      COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT: 0x8c4f
    },
    WEBGL_debug_renderer_info: {
      UNMASKED_VENDOR_WEBGL: 0x9245,
      UNMASKED_RENDERER_WEBGL: 0x9246
    },
    WEBGL_debug_shaders: {
      getTranslatedShaderSource() {}
    },
    WEBGL_depth_texture: {
      UNSIGNED_INT_24_8_WEBGL: 0x84fa
    },
    WEBGL_draw_buffers: {
      COLOR_ATTACHMENT0_WEBGL: 0x8ce0,
      COLOR_ATTACHMENT1_WEBGL: 0x8ce1,
      COLOR_ATTACHMENT2_WEBGL: 0x8ce2,
      COLOR_ATTACHMENT3_WEBGL: 0x8ce3,
      COLOR_ATTACHMENT4_WEBGL: 0x8ce4,
      COLOR_ATTACHMENT5_WEBGL: 0x8ce5,
      COLOR_ATTACHMENT6_WEBGL: 0x8ce6,
      COLOR_ATTACHMENT7_WEBGL: 0x8ce7,
      COLOR_ATTACHMENT8_WEBGL: 0x8ce8,
      COLOR_ATTACHMENT9_WEBGL: 0x8ce9,
      COLOR_ATTACHMENT10_WEBGL: 0x8cea,
      COLOR_ATTACHMENT11_WEBGL: 0x8ceb,
      COLOR_ATTACHMENT12_WEBGL: 0x8cec,
      COLOR_ATTACHMENT13_WEBGL: 0x8ced,
      COLOR_ATTACHMENT14_WEBGL: 0x8cee,
      COLOR_ATTACHMENT15_WEBGL: 0x8cef,
      DRAW_BUFFER0_WEBGL: 0x8825,
      DRAW_BUFFER1_WEBGL: 0x8826,
      DRAW_BUFFER2_WEBGL: 0x8827,
      DRAW_BUFFER3_WEBGL: 0x8828,
      DRAW_BUFFER4_WEBGL: 0x8829,
      DRAW_BUFFER5_WEBGL: 0x882a,
      DRAW_BUFFER6_WEBGL: 0x882b,
      DRAW_BUFFER7_WEBGL: 0x882c,
      DRAW_BUFFER8_WEBGL: 0x882d,
      DRAW_BUFFER9_WEBGL: 0x882e,
      DRAW_BUFFER10_WEBGL: 0x882f,
      DRAW_BUFFER11_WEBGL: 0x8830,
      DRAW_BUFFER12_WEBGL: 0x8831,
      DRAW_BUFFER13_WEBGL: 0x8832,
      DRAW_BUFFER14_WEBGL: 0x8833,
      DRAW_BUFFER15_WEBGL: 0x8834,
      MAX_COLOR_ATTACHMENTS_WEBGL: 0x8cdf,
      MAX_DRAW_BUFFERS_WEBGL: 0x8824
    },
    WEBGL_lose_context: {
      loseContext() {},
      restoreContext() {}
    }
  };

  const _getSupportedExtensions = gl.getSupportedExtensions;
  gl.getSupportedExtensions = () => {
    gl._realExtensions = _getSupportedExtensions().split(" ");
    return Object.keys(extensions);
  };

  gl.getExtension = name => {
    return extensions[name];
  };

  return gl;
}

const wrapped = wrap(WebGLRenderingContext);
module.exports = wrapped;
