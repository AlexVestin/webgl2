
const createContext = require(".");
const gl = new createContext();
console.log(gl.getExtension, gl.getExtension())

gl.depthMask(false);