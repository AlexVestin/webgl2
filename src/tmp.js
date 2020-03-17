'use strict';

const getMethods = obj => {
  let properties = new Set();
  let currentObj = obj;
  do {
    Object.getOwnPropertyNames(currentObj).map(item => properties.add(item));
  } while ((currentObj = Object.getPrototypeOf(currentObj)));
  return [...properties.keys()];
};
/*
const getProps = obj => {
    let properties = new Set();
    let currentObj = obj;
    do {
      Object.getOwnPropertyNames(currentObj).map(item => properties.add(item));
    } while ((currentObj = Object.getPrototypeOf(currentObj)));
    return [...properties.keys()].filter(item => typeof obj[item] === "function");
  };
*/
var logFunction = function(obj, fn) {
  return function() {
    const ret = obj[fn].apply(obj, [...arguments]);
    return ret;
  };
};

const WebGLRenderingContext = require('../gl');
function wrap(width, height) {
  let origGl = new WebGLRenderingContext(width, height);
  let gl = { ...origGl };

  for (const key of getMethods(origGl)) {
    if (typeof origGl[key] === 'function') {
      gl[key] = logFunction(origGl, key);
    } else {
      gl[key] = origGl[key];
    }
  }

  gl.WebGLRenderingContext = function WebGLRenderingContext(_) {
    this._ = _;
  };
  gl.WebGLSampler = function WebGLSampler(_) {
    this._ = _;
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

  const enforceBool = x => (typeof x === 'boolean' ? (x ? 1 : 0) : x);

  ////////////////////////////////////////////////////////////////////////////////

  // Attrib

  gl.bindAttribLocation = (program, index, name) =>
    origGl.bindAttribLocation(enforceId(program), index, name);

  gl.getActiveAttrib = (program, index) =>
    new gl.WebGLActiveInfo(origGl.getActiveAttrib(enforceId(program), index));

  gl.getAttribLocation = (program, name) =>
    origGl.getAttribLocation(enforceId(program), name);

  gl.vertexAttrib1fv = (indx, v) => {
    return origGl.vertexAttrib1fv(indx, enforceF32(v));
  };

  gl.vertexAttrib2fv = (indx, v) => {
    return origGl.vertexAttrib2fv(indx, enforceF32(v));
  };

  gl.vertexAttrib3fv = (indx, v) => {
    return origGl.vertexAttrib3fv(indx, enforceF32(v));
  };

  gl.vertexAttrib4fv = (indx, v) => {
    return origGl.vertexAttrib4fv(indx, enforceF32(v));
  };

  // VBO

  gl.createBuffer = () => new gl.WebGLBuffer(origGl.createBuffer());
  gl.deleteBuffer = buffer => origGl.deleteBuffer(enforceId(buffer));
  gl.isBuffer = buffer => origGl.isBuffer(enforceId(buffer));

  gl.bindBuffer = (target, buffer) =>
    origGl.bindBuffer(target, enforceId(buffer));

  gl.bindBufferBase = (target, index, buffer) =>
    origGl.bindBufferBase(target, index, enforceId(buffer));

  gl.bindBufferRange = (target, index, buffer, offset, size) =>
    origGl.bindBufferRange(target, index, enforceId(buffer), offset, size);

  // FBO

  gl.createFramebuffer = () =>
    new gl.WebGLFramebuffer(origGl.createFramebuffer());
  gl.deleteFramebuffer = framebuffer =>
    origGl.deleteFramebuffer(enforceId(framebuffer));
  gl.isFramebuffer = framebuffer =>
    origGl.isFramebuffer(enforceId(framebuffer));
  gl.bindFramebuffer = (target, framebuffer) => {
    origGl.bindFramebuffer(target, enforceId(framebuffer));
  };

  gl.framebufferRenderbuffer = (
    target,
    attachment,
    renderbuffertarget,
    renderbuffer,
  ) =>
    origGl.framebufferRenderbuffer(
      target,
      attachment,
      renderbuffertarget,
      enforceId(renderbuffer),
    );

  gl.framebufferTexture2D = (target, attachment, textarget, texture, level) =>
    origGl.framebufferTexture2D(
      target,
      attachment,
      textarget,
      enforceId(texture),
      level,
    );

  // Program

  gl.createProgram = () => new gl.WebGLProgram(origGl.createProgram());
  gl.deleteProgram = program => origGl.deleteProgram(enforceId(program));
  gl.isProgram = program => origGl.isProgram(enforceId(program));

  gl.getProgramInfoLog = program => {
    origGl.getProgramInfoLog(enforceId(program));
    return '\n';
  };

  gl.getProgramParameter = (program, pname) =>
    origGl.getProgramParameter(enforceId(program), pname);

  gl.linkProgram = program => origGl.linkProgram(enforceId(program));

  gl.useProgram = program => origGl.useProgram(enforceId(program));

  gl.validateProgram = program => origGl.validateProgram(enforceId(program));

  // RBO

  gl.createRenderbuffer = () =>
    new gl.WebGLRenderbuffer(origGl.createRenderbuffer());
  gl.deleteRenderbuffer = renderbuffer =>
    origGl.deleteRenderbuffer(enforceId(renderbuffer));
  gl.isRenderbuffer = renderbuffer =>
    origGl.isRenderbuffer(enforceId(renderbuffer));
  gl.bindRenderbuffer = (target, renderbuffer) =>
    origGl.bindRenderbuffer(target, enforceId(renderbuffer));

  // Sampler
  gl.createSampler = () => {
    return new gl.WebGLSampler(origGl.createSampler());
  };

  gl.samplerParameteri = (sampler, pname, param) => {
    origGl.samplerParameteri(enforceId(sampler), pname, param);
  };

  gl.bindSampler = (unit, sampler) => {
    origGl.bindSampler(unit, enforceId(sampler));
  };

  // Shader

  gl.createShader = type => new gl.WebGLShader(origGl.createShader(type));
  gl.deleteShader = shader => origGl.deleteShader(enforceId(shader));

  gl.isShader = shader => origGl.isShader(enforceId(shader));

  gl.attachShader = (program, shader) =>
    origGl.attachShader(enforceId(program), enforceId(shader));

  gl.compileShader = shader => origGl.compileShader(enforceId(shader));

  gl.detachShader = (program, shader) =>
    origGl.detachShader(enforceId(program), enforceId(shader));

  gl.getAttachedShaders = program =>
    origGl.getAttachedShaders(enforceId(program));

  gl.getShaderInfoLog = shader => origGl.getShaderInfoLog(enforceId(shader));

  gl.getShaderParameter = (shader, pname) =>
    origGl.getShaderParameter(enforceId(shader), pname);

  gl.getShaderSource = shader => origGl.getShaderSource(enforceId(shader));

  gl.shaderSource = (shaderId, code) =>
    origGl.shaderSource(
      enforceId(shaderId),
      code,
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

  gl.createTexture = () => new gl.WebGLTexture(origGl.createTexture());
  gl.deleteTexture = texture => origGl.deleteTexture(enforceId(texture));
  gl.isTexture = texture => origGl.isTexture(enforceId(texture));
  gl.bindTexture = (target, texture) =>
    origGl.bindTexture(target, enforceId(texture));

  gl.texImage2D = function(
    target,
    level,
    internalformat,
    width,
    height,
    border,
    format,
    type,
    pixels,
  ) {
    if (arguments.length === 6) {
      // width is now format, height is now type, and border is now pixels
      pixels = border;
      type = height;
      format = width;
      return origGl.texImage2D(
        target,
        level,
        internalformat,
        pixels.width,
        pixels.height,
        0,
        format,
        type,
        pixels,
      );
    }

    if (arguments.length === 9) {
      return origGl.texImage2D(
        target,
        level,
        internalformat,
        width,
        height,
        border,
        format,
        type,
        pixels,
      );
    }

    throw new TypeError('Function texImage2D() takes 6 or 9 arguments.');
  };

  // Uniform

  gl.getActiveUniform = (program, index) =>
    new gl.WebGLActiveInfo(origGl.getActiveUniform(enforceId(program), index));

  gl.getUniform = (program, location) =>
    origGl.getUniform(enforceId(program), enforceId(location));

  gl.getUniformLocation = (program, name) =>
    new gl.WebGLUniformLocation(
      origGl.getUniformLocation(enforceId(program), name),
    );

  gl.uniform1f = (location, x) => origGl.uniform1f(enforceId(location), x);
  gl.uniform1fv = (location, v) => {
    return origGl.uniform1fv(enforceId(location), enforceF32(v));
  };

  gl.uniform1i = (location, x) =>
    origGl.uniform1i(enforceId(location), enforceBool(x));

  gl.uniform1iv = (location, v) => {
    return origGl.uniform1iv(enforceId(location), enforceF32(v));
  };

  gl.uniform2f = (location, x, y) =>
    origGl.uniform2f(enforceId(location), x, y);

  gl.uniform2fv = (location, v) => {
    return origGl.uniform2fv(enforceId(location), enforceF32(v));
  };

  gl.uniform2i = (location, x, y) =>
    origGl.uniform2i(enforceId(location), x, y);

  gl.uniform2iv = (location, v) => {
    return origGl.uniform2iv(enforceId(location), enforceF32(v));
  };

  gl.uniform3f = (location, x, y, z) =>
    origGl.uniform3f(enforceId(location), x, y, z);

  gl.uniform3fv = (location, v) => {
    return origGl.uniform3fv(enforceId(location), enforceF32(v));
  };

  gl.uniform3i = (location, x, y, z) =>
    origGl.uniform3i(enforceId(location), x, y, z);

  gl.uniform3iv = (location, v) => {
    return origGl.uniform3iv(enforceId(location), enforceF32(v));
  };

  gl.uniform4f = (location, x, y, z, w) =>
    origGl.uniform4f(enforceId(location), x, y, z, w);

  gl.uniform4fv = (location, v) => {
    return origGl.uniform4fv(enforceId(location), enforceF32(v));
  };

  gl.uniform4i = (location, x, y, z, w) =>
    origGl.uniform4i(enforceId(location), x, y, z, w);

  gl.uniform4iv = (location, v) => {
    return origGl.uniform4iv(enforceId(location), enforceF32(v));
  };

  gl.uniformMatrix2fv = (location, transpose, v) => {
    return origGl.uniformMatrix2fv(
      enforceId(location),
      transpose,
      enforceF32(v),
    );
  };

  gl.uniformMatrix3fv = (location, transpose, v) => {
    return origGl.uniformMatrix3fv(
      enforceId(location),
      transpose,
      enforceF32(v),
    );
  };

  gl.uniformMatrix4fv = (location, transpose, v) => {
    return origGl.uniformMatrix4fv(
      enforceId(location),
      transpose,
      enforceF32(v),
    );
  };

  // VAO

  gl.createVertexArray = () =>
    new gl.WebGLVertexArray(origGl.createVertexArray());

  gl.deleteVertexArray = vao => origGl.deleteVertexArray(enforceId(vao));

  gl.isVertexArray = vao => origGl.isVertexArray(enforceId(vao));

  gl.bindVertexArray = vao => origGl.bindVertexArray(enforceId(vao));

  // Transform feedback
  gl.createTransformFeedback = () =>
    new gl.WebGLTransformFeedback(origGl.createTransformFeedback());

  gl.deleteTransformFeedback = transformFeedback =>
    origGl.deleteTransformFeedback(enforceId(transformFeedback));

  gl.isTransformFeedback = transformFeedback =>
    origGl.isTransformFeedback(enforceId(transformFeedback));

  gl.bindTransformFeedback = (target, transformFeedback) =>
    origGl.bindTransformFeedback(target, enforceId(transformFeedback));

  gl.transformFeedbackVaryings = (program, varyings, mode) =>
    origGl.transformFeedbackVaryings(enforceId(program), varyings, mode);

  gl.getTransformFeedbackVarying = (program, location) =>
    new gl.WebGLActiveInfo(
      origGl.getTransformFeedbackVarying(enforceId(program), location),
    );

  // Misc OpenGL Functions
  gl.getParameter = pname => {
    let ret;
    switch (pname) {
      case 0x9245:
        ret = origGl.getParameter(0x1f00); // UNMASKED VENDOR
        break;
      case 0x9246:
        ret = origGl.getParameter(0x1f01); // UNMASKED RENDERER
        break;
      case 0x1f01:
        ret = 'WebKit'; // VENDOR
        break;
      case 0x1f00:
        ret = 'WebKit WebGL'; // RENDERER
        break;
      default:
        ret = origGl.getParameter(pname);
    }

    return ret;
  };
  gl.pixelStorei = (pname, param) => {
    origGl.pixelStorei(pname, enforceBool(param));
  };

  gl.drawElements = (a, b, c, d) => {
    origGl.drawElements(a, b, c, d);
  };

  gl.viewport = (x, y, width, height) => origGl.viewport(x, y, width, height);

  Object.defineProperty(gl, 'drawingBufferWidth', {
    get: () => gl.getParameter(gl.VIEWPORT)[2],
  });
  Object.defineProperty(gl, 'drawingBufferHeight', {
    get: () => gl.getParameter(gl.VIEWPORT)[3],
  });

  ////////////////////////////////////////////////////////////////////////////////

  const extensions = {
    ANGLE_instanced_arrays: {
      VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE: 0x88fe,
      drawArraysInstancedANGLE: (mode, first, count, instanceCount) =>
        origGl.drawArraysInstanced(mode, first, count, instanceCount),
      drawElementsInstancedANGLE: (mode, count, type, offset, instanceCount) =>
        origGl.drawElementsInstanced(mode, count, type, offset, instanceCount),
      vertexAttribDivisorANGLE: (index, divisor) =>
        origGl.vertexAttribDivisor(index, divisor),
    },
    EXT_blend_minmax: {
      MIN_EXT: 0x8007,
      MAX_EXT: 0x8008,
    },
    EXT_color_buffer_float: {},
    EXT_color_buffer_half_float: {
      RGBA16F_EXT: 0x881a,
      RGB16F_EXT: 0x881b,
      FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE_EXT: 0x8211,
      UNSIGNED_NORMALIZED_EXT: 0x8c17,
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
      getQueryObjectEXT() {},
    },
    EXT_frag_depth: {},
    EXT_sRGB: {
      SRGB_EXT: 0x8c40,
      SRGB_ALPHA_EXT: 0x8c42,
      SRGB8_ALPHA8_EXT: 0x8c43,
      FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_EXT: 0x8210,
    },
    EXT_shader_texture_lod: {},
    EXT_texture_filter_anisotropic: {
      MAX_TEXTURE_MAX_ANISOTROPY_EXT: 0x84ff,
      TEXTURE_MAX_ANISOTROPY_EXT: 0x84fe,
    },
    OES_element_index_uint: {},
    OES_standard_derivatives: {
      FRAGMENT_SHADER_DERIVATIVE_HINT_OES: 0x8b8b,
    },
    OES_texture_float: {},
    OES_texture_float_linear: {},
    OES_texture_half_float: {
      HALF_FLOAT_OES: 0x8d61,
    },
    OES_texture_half_float_linear: {},
    OES_vertex_array_object: {
      VERTEX_ARRAY_BINDING_OES: 0x85b5,
      createVertexArrayOES: () =>
        new gl.WebGLVertexArray(origGl.createVertexArray()),
      deleteVertexArrayOES: vao => origGl.deleteVertexArray(enforceId(vao)),
      isVertexArrayOES: vao => origGl.isVertexArray(enforceId(vao)),
      bindVertexArrayOES: v => origGl.bindVertexArray(enforceId(v)),
    },
    WEBGL_color_buffer_float: {
      RGBA32F_EXT: 0x8814,
      RGB32F_EXT: 0x8815,
      FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE_EXT: 0x8211,
      UNSIGNED_NORMALIZED_EXT: 0x8c17,
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
      COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR: 0x93dd,
    },
    WEBGL_compressed_texture_atc: {
      COMPRESSED_RGB_ATC_WEBGL: 0x8c92,
      COMPRESSED_RGBA_ATC_EXPLICIT_ALPHA_WEBGL: 0x8c92,
      COMPRESSED_RGBA_ATC_INTERPOLATED_ALPHA_WEBGL: 0x87ee,
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
      COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2: 0x9279,
    },
    WEBGL_compressed_texture_etc1: {
      COMPRESSED_RGB_ETC1_WEBGL: 0x8d64,
    },
    WEBGL_compressed_texture_pvrtc: {
      COMPRESSED_RGB_PVRTC_4BPPV1_IMG: 0x8c00,
      COMPRESSED_RGBA_PVRTC_4BPPV1_IMG: 0x8c02,
      COMPRESSED_RGB_PVRTC_2BPPV1_IMG: 0x8c01,
      COMPRESSED_RGBA_PVRTC_2BPPV1_IMG: 0x8c03,
    },
    WEBGL_compressed_texture_s3tc: {
      COMPRESSED_RGB_S3TC_DXT1_EXT: 0x83f0,
      COMPRESSED_RGBA_S3TC_DXT1_EXT: 0x83f1,
      COMPRESSED_RGBA_S3TC_DXT3_EXT: 0x83f2,
      COMPRESSED_RGBA_S3TC_DXT5_EXT: 0x83f3,
    },
    WEBGL_compressed_texture_s3tc_srgb: {
      COMPRESSED_SRGB_S3TC_DXT1_EXT: 0x8c4c,
      COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT: 0x8c4d,
      COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT: 0x8c4e,
      COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT: 0x8c4f,
    },
    WEBGL_debug_renderer_info: {
      UNMASKED_VENDOR_WEBGL: 0x9245,
      UNMASKED_RENDERER_WEBGL: 0x9246,
    },
    WEBGL_debug_shaders: {
      getTranslatedShaderSource() {},
    },
    WEBGL_depth_texture: {
      UNSIGNED_INT_24_8_WEBGL: 0x84fa,
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
      MAX_DRAW_BUFFERS_WEBGL: 0x8824,
    },
    WEBGL_lose_context: {
      loseContext() {},
      restoreContext() {},
    },
  };

  gl.getSupportedExtensions = () => {
    gl._realExtensions = origGl.getSupportedExtensions().split(' ');
    return Object.keys(extensions);
  };

  gl.getExtension = name => {
    return extensions[name];
  };

  return gl;
}

module.exports = wrap;
