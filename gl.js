const { WebGLRenderingContext, VideoDecodingContext } = require('bindings')(
  'webgl',
);
module.exports = {
  VideoDecodingContext,
  WebGLRenderingContext,
};
