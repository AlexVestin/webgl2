
const enforceF32 = v => v instanceof Array ? new Float32Array(v) : v;
const enforceId = x => x ? x._ : 0;
const enforceBool = x => typeof x === 'boolean' ? (x ? 1 : 0) : x;

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

class WrappedWebGLRenderingContext {    
    constructor(gl) {
        this._gl = gl;
    }


    bindAttribLocation = (program, index, name) => gl.bindAttribLocation(enforceId(program), index, name);
    getActiveAttrib = (program, index) => new gl.WebGLActiveInfo(gl.getActiveAttrib(enforceId(program), index));

}