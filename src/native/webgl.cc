#include "webgl.h"
#include "extra.h"
#include <iostream>

#define GL_METHOD(method_name) Napi::Value WebGLRenderingContext:: method_name \
   (const Napi::CallbackInfo& info)

Napi::FunctionReference WebGLRenderingContext::constructor;


void checkEGLError() {
	int err = eglGetError();
	const char* errorString;
	switch(err) {
		case EGL_NOT_INITIALIZED: errorString = "EGL_NOT_INITIALIZED"; break;
		case EGL_BAD_ACCESS: errorString = "EGL_BAD_ACCESS";break;
		case EGL_BAD_ALLOC: errorString = "EGL_BAD_ALLOC";break;
		case EGL_BAD_ATTRIBUTE: errorString = "EGL_BAD_ATTRIBUTE"; break;
		case EGL_BAD_CONTEXT: errorString = "EGL_BAD_CONTEXT"; break;
		case EGL_BAD_CONFIG: errorString = "EGL_BAD_CONFIG"; break;
		case EGL_BAD_CURRENT_SURFACE: errorString = "EGL_BAD_CURRENT_SURFACE"; break;
		case EGL_BAD_DISPLAY: errorString = "EGL_BAD_DISPLAY"; break;
		case EGL_BAD_SURFACE: errorString = "EGL_BAD_SURFACE"; break;
		case EGL_BAD_MATCH: errorString = "EGL_BAD_MATCH"; break;
		case EGL_BAD_PARAMETER: errorString = "EGL_BAD_PARAMETER"; break;
		case EGL_BAD_NATIVE_PIXMAP: errorString = "EGL_BAD_NATIVE_PIXMAP"; break;
		case EGL_BAD_NATIVE_WINDOW: errorString = "EGL_BAD_NATIVE_WINDOW"; break;
		case EGL_CONTEXT_LOST: errorString = "EGL_CONTEXT_LOST"; break;
	}

	std::cout << "EGL error: " << errorString << std::endl;
}


WebGLRenderingContext::WebGLRenderingContext(const Napi::CallbackInfo& info): Napi::ObjectWrap<WebGLRenderingContext>(info) {
    int32_t width = 1280;
	int32_t height = 720;

	if (info.Length() >= 2) {
		width = info[0].As<Napi::Number>().Int32Value();   //Width
		height = info[1].As<Napi::Number>().Int32Value();   //Height
	}

	if(info.Length() >= 3 && info[3].IsBoolean()) alpha = info[3].As<Napi::Boolean>().Value();
	if(info.Length() >= 4 && info[4].IsBoolean()) depth = info[4].As<Napi::Boolean>().Value();
	if(info.Length() >= 5 && info[5].IsBoolean()) stencil = info[5].As<Napi::Boolean>().Value();
	if(info.Length() >= 6 && info[6].IsBoolean()) antialias = info[6].As<Napi::Boolean>().Value();
	if(info.Length() >= 7 && info[7].IsBoolean()) premultipliedAlpha = info[7].As<Napi::Boolean>().Value();
	if(info.Length() >= 8 && info[8].IsBoolean()) preserveDrawingBuffer = info[8].As<Napi::Boolean>().Value();
	if(info.Length() >= 9 && info[9].IsBoolean()) preferLowPowerToHighPerformance = info[9].As<Napi::Boolean>().Value();
	if(info.Length() == 10 && info[10].IsBoolean()) failIfMajorPerformanceCaveat = info[10].As<Napi::Boolean>().Value();

	unpack_flip_y = false;
	unpack_colorspace_conversion = 0x9244;
	unpack_alignment = 4;


    screenWidth = width;
    screenHeight = height;


	/*
	std::vector<EGLAttrib> display_attributes = {
		EGL_PLATFORM_ANGLE_TYPE_ANGLE,
		#if defined(__arm__)
			EGL_PLATFORM_ANGLE_TYPE_OPENGLES_ANGLE,
		#else
			EGL_PLATFORM_ANGLE_TYPE_DEFAULT_ANGLE,
		#endif
		EGL_NONE
	};

	std::cout << "Getting platform display" << std::endl; 	
	display = eglGetPlatformDisplay(EGL_PLATFORM_ANGLE_ANGLE, nullptr, &display_attributes[0]);

	//Get display
	if (display == EGL_NO_DISPLAY) {
		std::cout << "Using default display" << std::endl;
		
	}*/

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (display == EGL_NO_DISPLAY) {
		std::cerr << "Unable to initialize display" <<std::endl;
		checkEGLError();
		exit(1);
	}

	EGLint major;
	EGLint minor;

	std::cout << "eglInitialize" << std::endl; 	

	//Initialize EGL
	if (!eglInitialize(display, &major, &minor)) {
		std::cerr << "Unable to initialize egl versions" <<std::endl;
		checkEGLError();
		exit(1);
	}
	//Save display


	//Set up configuration
	EGLint attrib_list[] = {
		EGL_SURFACE_TYPE, EGL_PBUFFER_BIT
		, EGL_RED_SIZE,     8
		, EGL_GREEN_SIZE,   8
		, EGL_BLUE_SIZE,    8
		, EGL_ALPHA_SIZE,   alpha ? 8 : 0
		, EGL_DEPTH_SIZE,   depth ? 24 : 0
		, EGL_STENCIL_SIZE, stencil ? 8 : 0
		, EGL_NONE
	};
	EGLint num_config;
	std::cout << "eglChooseConfig" << std::endl; 	
	if (!eglChooseConfig(
		display,
		attrib_list,
		&config,
		1,
		&num_config) ||
		num_config != 1) {
		std::cerr << "Unable to initialize with config" <<std::endl;
		checkEGLError();
		exit(1);
	}

	EGLint config_renderable_type;
	std::cout << "eglGetConfigAttrib" << std::endl; 	
	
	if (!eglGetConfigAttrib(display, config, EGL_RENDERABLE_TYPE, &config_renderable_type)) {
		// TODO error handling
		std::cerr << "Unable to get config attrib" << std::endl;
		checkEGLError();
		exit(1);
	}

	// TODO input from user
	EGLint major_version = 3;
	EGLint minor_version = 1;
	if ((config_renderable_type & EGL_OPENGL_ES3_BIT) == 0 && major_version >= 3) {
		std::cout << "Using lower version" << std::endl;
		major_version = 2;
		minor_version = 0;
	}

	// Append attributes based on available features
	std::vector<EGLint> context_attributes;
	context_attributes.push_back(EGL_CONTEXT_MAJOR_VERSION);
	context_attributes.push_back(major_version);

	context_attributes.push_back(EGL_CONTEXT_MINOR_VERSION);
	context_attributes.push_back(minor_version);

	// TODO(kreeger): This is only needed to avoid validation.
	// This is needed for OES_TEXTURE_HALF_FLOAT textures uploading as FLOAT
	context_attributes.push_back(EGL_CONTEXT_OPENGL_NO_ERROR_KHR);
	context_attributes.push_back(EGL_TRUE);
	context_attributes.push_back(EGL_NONE);

	std::cout << "eglCreateContext" << std::endl; 	
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes.data());
	if (context == EGL_NO_CONTEXT) {
		std::cerr << "Unable to initialize context" <<std::endl;
		checkEGLError();
		exit(1);
	}

	EGLint surfaceAttribs[] = {
			EGL_WIDTH,  (EGLint)width
		, EGL_HEIGHT, (EGLint)height
		, EGL_NONE
	};

	std::cout << "eglCreatePbufferSurface" << std::endl; 	
	surface = eglCreatePbufferSurface(display, config, surfaceAttribs);
	if (surface == EGL_NO_SURFACE) {
		std::cerr << "Unable to initialize pbuffersurface" <<std::endl;
		checkEGLError();
		exit(1);
	}

	//Set active
	std::cout << "eglMakeCurrent" << std::endl; 	

	if (!eglMakeCurrent(display, surface, surface, context)) {
		std::cerr << "Unable to make context current" <<std::endl;
		checkEGLError();
		exit(1);
	}

	std::cout << "Initialized" << std::endl;
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}


GL_METHOD(Clear) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	
	glClear(target);
	RET_UNDEFINED;
	
}


GL_METHOD(ClearColor) { NAPI_ENV;
	
	REQ_FLOAT_ARG(0, red);
	REQ_FLOAT_ARG(1, green);
	REQ_FLOAT_ARG(2, blue);
	REQ_FLOAT_ARG(3, alpha);
	
	glClearColor(red, green, blue, alpha);
	RET_UNDEFINED;
	
}


GL_METHOD(ClearDepth) { NAPI_ENV;
	
	REQ_FLOAT_ARG(0, depth);
	glClearDepthf(depth);
	RET_UNDEFINED;
	
}


GL_METHOD(ColorMask) { NAPI_ENV;
	
	LET_BOOL_ARG(0, red);
	LET_BOOL_ARG(1, green);
	LET_BOOL_ARG(2, blue);
	LET_BOOL_ARG(3, alpha);
	
	glColorMask(red, green, blue, alpha);
	RET_UNDEFINED;
	
}


GL_METHOD(CullFace) { NAPI_ENV;
	
	REQ_INT32_ARG(0, mode);
	
	glCullFace(mode);
	RET_UNDEFINED;
	
}


GL_METHOD(DepthFunc) { NAPI_ENV;
	
	REQ_INT32_ARG(0, id);
	
	glDepthFunc(id);
	RET_UNDEFINED;
	
}


GL_METHOD(DepthMask) { NAPI_ENV;
	
	LET_BOOL_ARG(0, flag);
	
	glDepthMask(flag);
	RET_UNDEFINED;
	
}


GL_METHOD(DepthRange) { NAPI_ENV;
	
	REQ_FLOAT_ARG(0, zNear);
	REQ_FLOAT_ARG(1, zFar);
	
	glDepthRangef(zNear, zFar);
	RET_UNDEFINED;
	
}


GL_METHOD(Disable) { NAPI_ENV;
	
	REQ_INT32_ARG(0, id);
	
	glDisable(id);
	RET_UNDEFINED;
	
}


GL_METHOD(DrawArrays) { NAPI_ENV;
	
	REQ_INT32_ARG(0, mode);
	REQ_INT32_ARG(1, first);
	REQ_INT32_ARG(2, count);
	
	glDrawArrays(mode, first, count);
	RET_UNDEFINED;
	
}


GL_METHOD(DrawElements) { NAPI_ENV;
	
	REQ_INT32_ARG(0, mode);
	REQ_INT32_ARG(1, count);
	REQ_INT32_ARG(2, type);
	REQ_OFFS_ARG(3, ptr);

	size_t offset =  info[3].As<Napi::Number>().Uint32Value(); //Nan::To<uint32_t>(info[3]).ToChecked();
	GLvoid *indices = ptr == 0 ? nullptr : reinterpret_cast<GLvoid*>(ptr);
	glDrawElements(mode, count, type, reinterpret_cast<GLvoid*>(offset));
	RET_UNDEFINED;
	
}


GL_METHOD(Enable) { NAPI_ENV;
	
	REQ_INT32_ARG(0, id);
	
	glEnable(id);
	RET_UNDEFINED;
	
}


GL_METHOD(Finish) { NAPI_ENV;
	
	glFinish();
	RET_UNDEFINED;
	
}


GL_METHOD(Flush) { NAPI_ENV;
	
	glFlush();
	RET_UNDEFINED;
	
}


GL_METHOD(FrontFace) { NAPI_ENV;
	
	REQ_INT32_ARG(0, id);
	
	glFrontFace(id);
	RET_UNDEFINED;
	
}


GL_METHOD(GetError) { NAPI_ENV;
	
	RET_NUM(glGetError());
	RET_UNDEFINED;
	
}


#define CASES_PARAM_BOOL case GL_BLEND: \
	case GL_CULL_FACE: \
	case GL_DEPTH_TEST: \
	case GL_DEPTH_WRITEMASK: \
	case GL_DITHER: \
	case GL_POLYGON_OFFSET_FILL: \
	case GL_SAMPLE_COVERAGE_INVERT: \
	case GL_SCISSOR_TEST: \
	case GL_STENCIL_TEST: \
	case 0x9240 /* UNPACK_FLIP_Y_WEBGL */: \
	case 0x9241 /* UNPACK_PREMULTIPLY_ALPHA_WEBGL*/:

#define CASES_PARAM_FLOAT case GL_DEPTH_CLEAR_VALUE: \
	case GL_LINE_WIDTH: \
	case GL_POLYGON_OFFSET_FACTOR: \
	case GL_POLYGON_OFFSET_UNITS: \
	case GL_SAMPLE_COVERAGE_VALUE:

#define CASES_PARAM_STRING case GL_RENDERER: \
	case GL_SHADING_LANGUAGE_VERSION: \
	case GL_VENDOR: \
	case GL_VERSION: \
	case GL_EXTENSIONS: \

#define CASES_PARAM_INT2 case GL_MAX_VIEWPORT_DIMS:

#define CASES_PARAM_INT4 case GL_SCISSOR_BOX: \
	case GL_VIEWPORT:

#define CASES_PARAM_FLOAT2 case GL_ALIASED_LINE_WIDTH_RANGE: \
	case GL_ALIASED_POINT_SIZE_RANGE: \
	case GL_DEPTH_RANGE:

#define CASES_PARAM_FLOAT4 case GL_BLEND_COLOR: \
	case GL_COLOR_CLEAR_VALUE:

#define CASES_PARAM_BOOL4 case GL_COLOR_WRITEMASK:

#define CASES_PARAM_INT case GL_ARRAY_BUFFER_BINDING: \
	case GL_CURRENT_PROGRAM: \
	case GL_ELEMENT_ARRAY_BUFFER_BINDING: \
	case GL_FRAMEBUFFER_BINDING: \
	case GL_RENDERBUFFER_BINDING: \
	case GL_TEXTURE_BINDING_2D: \
	case GL_TEXTURE_BINDING_CUBE_MAP:



GL_METHOD(GetParameter) { NAPI_ENV;
	
	REQ_INT32_ARG(0, name);
	
	GLboolean bParams[4];
	const char *cParams;
	GLint iParams[4];
	GLfloat fParams[4];
	Napi::Array arr = Napi::Array::New(env);
	
	switch(name) {
	
	CASES_PARAM_BOOL
		glGetBooleanv(name, bParams);
		RET_BOOL(bParams[0] != 0);
		break;
	
	CASES_PARAM_FLOAT
		glGetFloatv(name, fParams);
		RET_NUM(fParams[0]);
		break;
	
	CASES_PARAM_STRING
		cParams = reinterpret_cast<const char*>(glGetString(name));
		if (cParams != NULL) {
			RET_STR(cParams);
		} else {
			RET_UNDEFINED;
		}
		break;
	
	CASES_PARAM_INT2
		glGetIntegerv(name, iParams);
		arr = Napi::Array::New(env);
		arr.Set(0U, JS_NUM(iParams[0]));
		arr.Set(1U, JS_NUM(iParams[1]));
		RET_VALUE(arr);
		break;
	
	CASES_PARAM_INT4
		glGetIntegerv(name, iParams);
		arr.Set(0U, JS_NUM(iParams[0]));
		arr.Set(1U, JS_NUM(iParams[1]));
		arr.Set(2U, JS_NUM(iParams[2]));
		arr.Set(3U, JS_NUM(iParams[3]));
		RET_VALUE(arr);
		break;
	
	CASES_PARAM_FLOAT2
		glGetFloatv(name, fParams);
		arr.Set(0U, JS_NUM(fParams[0]));
		arr.Set(1U, JS_NUM(fParams[1]));
		RET_VALUE(arr);
		break;
	
	CASES_PARAM_FLOAT4
		glGetFloatv(name, fParams);
		arr.Set(0U, JS_NUM(fParams[0]));
		arr.Set(1U, JS_NUM(fParams[1]));
		arr.Set(2U, JS_NUM(fParams[2]));
		arr.Set(3U, JS_NUM(fParams[3]));
		RET_VALUE(arr);
		break;
	
	CASES_PARAM_BOOL4
		glGetBooleanv(name, bParams);
		arr.Set(0U, JS_BOOL(bParams[0] != 0));
		arr.Set(1U, JS_BOOL(bParams[1] != 0));
		arr.Set(2U, JS_BOOL(bParams[2] != 0));
		arr.Set(3U, JS_BOOL(bParams[3] != 0));
		RET_VALUE(arr);
		break;
	
	CASES_PARAM_INT
		glGetIntegerv(name, iParams);
		RET_NUM(iParams[0]);
		break;
	
	// returns an int
	default:
		glGetIntegerv(name, iParams);
		RET_NUM(iParams[0]);
		break;
	
	}
	
}


GL_METHOD(GetRenderTarget) { NAPI_ENV;

	std::cout << "Get render target called, verify glTexStorage" << std::endl;
	
	REQ_UINT32_ARG(0, width);
	REQ_UINT32_ARG(1, height);
	REQ_UINT32_ARG(2, samples);
	
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	GLuint renderBuffer;
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	
	if (samples > 1) {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, width, height);
	} else {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	}
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,	renderBuffer);
	
	GLuint tex;
	glGenTextures(1, &tex);
	
	if (samples > 1) {
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
		// glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_LEVEL, 0);

		// TODO verify same as TexImage
		glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA8, width, height, true);
		// glFramebufferTexture2DMultisampleEXT(
		// 	GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0, samples
		// );
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);
	} else {
		glBindTexture(GL_TEXTURE_2D, tex);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	if (framebufferStatus == GL_FRAMEBUFFER_COMPLETE) {
		
		Napi::Array result = Napi::Array::New(env);
		result.Set(0U, JS_NUM(fbo));
		result.Set(1U, JS_NUM(tex));
		
		RET_VALUE(result);
		
	} else {
		RET_NULL;
	}
	
}


GL_METHOD(GetSupportedExtensions) { NAPI_ENV;
	
	const char *extensions = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
	
	if (extensions != NULL) {
		RET_STR(extensions);
	} else {
		RET_STR("");
	}
	
}







GL_METHOD(Hint) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, mode);
	
	glHint(target, mode);
	RET_UNDEFINED;
	
}


GL_METHOD(IsEnabled) { NAPI_ENV;
	
	REQ_INT32_ARG(0, cap);
	
	bool ret = glIsEnabled(cap) != 0;
	
	RET_BOOL(ret);
	
}


GL_METHOD(LineWidth) { NAPI_ENV;
	
	REQ_FLOAT_ARG(0, width);
	
	glLineWidth(width);
	RET_UNDEFINED;
	
}


GL_METHOD(PixelStorei) { NAPI_ENV;
	
	REQ_INT32_ARG(0, pname);
	REQ_INT32_ARG(1, param);

     //Handle WebGL specific extensions
    switch(pname) {
        case 0x9240:
            unpack_flip_y = param != 0;
            break;

        case 0x9241:
            unpack_premultiply_alpha = param != 0;
            break;

        case 0x9243:
            unpack_colorspace_conversion = param;
            break;

        case GL_UNPACK_ALIGNMENT:
            unpack_alignment = param;
            glPixelStorei(pname, param);
            break;

        default:
            glPixelStorei(pname, param);
    }

	RET_UNDEFINED;
}


GL_METHOD(PolygonOffset) { NAPI_ENV;
	
	REQ_FLOAT_ARG(0, factor);
	REQ_FLOAT_ARG(1, units);
	
	glPolygonOffset(factor, units);
	RET_UNDEFINED;
	
}


GL_METHOD(ReadPixels) { NAPI_ENV;
	
	REQ_INT32_ARG(0, x);
	REQ_INT32_ARG(1, y);
	REQ_INT32_ARG(2, width);
	REQ_INT32_ARG(3, height);
	REQ_INT32_ARG(4, format);
	REQ_INT32_ARG(5, type);
	
	REQ_OBJ_ARG(6, image);
	
	void *pixels = getData(env, image);
	glReadPixels(x, y, width, height, format, type, pixels);
	RET_UNDEFINED;
	
}


GL_METHOD(SampleCoverage) { NAPI_ENV;
	
	REQ_FLOAT_ARG(0, value);
	LET_BOOL_ARG(1, invert);
	
	glSampleCoverage(value, invert);
	RET_UNDEFINED;
	
}


GL_METHOD(Scissor) { NAPI_ENV;
	
	REQ_INT32_ARG(0, x);
	REQ_INT32_ARG(1, y);
	REQ_INT32_ARG(2, width);
	REQ_INT32_ARG(3, height);
	
	glScissor(x, y, width, height);
	RET_UNDEFINED;
	
}


GL_METHOD(Viewport) { NAPI_ENV;
	
	REQ_INT32_ARG(0, x);
	REQ_INT32_ARG(1, y);
	REQ_INT32_ARG(2, w);
	REQ_INT32_ARG(3, h);
	
	glViewport(x, y, w, h);
	RET_UNDEFINED;
	
}

// attrib 



GL_METHOD(BindAttribLocation) { NAPI_ENV;
	
	REQ_INT32_ARG(0, program);
	REQ_INT32_ARG(1, index);
	REQ_STR_ARG(2, name);
	
	glBindAttribLocation(program, index, name.c_str());
	RET_UNDEFINED;
	
}


GL_METHOD(DisableVertexAttribArray) { NAPI_ENV;
	
	REQ_INT32_ARG(0, index);
	
	glDisableVertexAttribArray(index);
	RET_UNDEFINED;
	
}


GL_METHOD(EnableVertexAttribArray) { NAPI_ENV;
	
	REQ_INT32_ARG(0, id);
	
	glEnableVertexAttribArray(id);
	RET_UNDEFINED;
	
}


GL_METHOD(GetActiveAttrib) { NAPI_ENV;
	
	REQ_INT32_ARG(0, program);
	REQ_INT32_ARG(1, index);
	
	char name[1024];
	GLsizei length = 0;
	GLenum type;
	GLsizei size;
	glGetActiveAttrib(program, index, 1024, &length, &size, &type, name);
	
	Napi::Array activeInfo = Napi::Array::New(env);
	activeInfo.Set("size", JS_NUM(size));
	activeInfo.Set("type", JS_NUM(static_cast<int>(type)));
	activeInfo.Set("name", JS_STR(name));
	
	RET_VALUE(activeInfo);
	
}


GL_METHOD(GetAttribLocation) { NAPI_ENV;
	
	REQ_INT32_ARG(0, program);
	REQ_STR_ARG(1, name);
	
	RET_NUM(glGetAttribLocation(program, name.c_str()));
	
}


#define CASES_VERTEX_ATTR_BOOL case GL_VERTEX_ATTRIB_ARRAY_ENABLED: \
	case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED:

#define CASES_VERTEX_ATTR_INT case GL_VERTEX_ATTRIB_ARRAY_SIZE: \
	case GL_VERTEX_ATTRIB_ARRAY_STRIDE: \
	case GL_VERTEX_ATTRIB_ARRAY_TYPE: \
	case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:

#define CASES_VERTEX_ATTR_FLOAT4 case GL_CURRENT_VERTEX_ATTRIB:


GL_METHOD(GetVertexAttrib) { NAPI_ENV;
	
	REQ_INT32_ARG(0, index);
	REQ_INT32_ARG(1, pname);
	
	GLint value = 0;
	float vextex_attribs[4];
	Napi::Array arr = Napi::Array::New(env);
	
	switch (pname) {
	
	CASES_VERTEX_ATTR_BOOL
		glGetVertexAttribiv(index, pname, &value);
		RET_BOOL(value != 0);
		break;
	
	CASES_VERTEX_ATTR_INT
		glGetVertexAttribiv(index, pname, &value);
		RET_NUM(value);
		break;
	
	CASES_VERTEX_ATTR_FLOAT4
		glGetVertexAttribfv(index, pname, vextex_attribs);
		arr.Set(0U, JS_NUM(vextex_attribs[0]));
		arr.Set(1U, JS_NUM(vextex_attribs[1]));
		arr.Set(2U, JS_NUM(vextex_attribs[2]));
		arr.Set(3U, JS_NUM(vextex_attribs[3]));
		RET_VALUE(arr);
		break;
	
	default:
		JS_THROW("GetVertexAttrib: Invalid Enum");
		RET_UNDEFINED;
	
	}
	
}


GL_METHOD(GetVertexAttribOffset) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, index);
	REQ_INT32_ARG(1, name);
	
	void *ret = NULL;
	glGetVertexAttribPointerv(index, name, &ret);
	
	RET_NUM(ToGLuint(ret));
	
}


GL_METHOD(VertexAttrib1f) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_FLOAT_ARG(1, x);
	
	glVertexAttrib1f(location, x);
	RET_UNDEFINED;
	
}


GL_METHOD(VertexAttrib1fv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_OBJ_ARG(1, abv);
	
	GLfloat *data = getArrayData<GLfloat>(env, abv);
	
	glVertexAttrib1fv(location, data);
	RET_UNDEFINED;
	
}


GL_METHOD(VertexAttrib2f) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_FLOAT_ARG(1, x);
	REQ_FLOAT_ARG(2, y);
	
	glVertexAttrib2f(location, x, y);
	RET_UNDEFINED;
	
}


GL_METHOD(VertexAttrib2fv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_OBJ_ARG(1, abv);
	
	GLfloat *data = getArrayData<GLfloat>(env, abv);
	
	glVertexAttrib2fv(location, data);
	RET_UNDEFINED;
	
}


GL_METHOD(VertexAttrib3f) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_FLOAT_ARG(1, x);
	REQ_FLOAT_ARG(2, y);
	REQ_FLOAT_ARG(3, z);
	
	glVertexAttrib3f(location, x, y, z);
	RET_UNDEFINED;
	
}


GL_METHOD(VertexAttrib3fv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_OBJ_ARG(1, abv);
	
	GLfloat *data = getArrayData<GLfloat>(env, abv);
	
	glVertexAttrib3fv(location, data);
	RET_UNDEFINED;
	
}


GL_METHOD(VertexAttrib4f) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_FLOAT_ARG(1, x);
	REQ_FLOAT_ARG(2, y);
	REQ_FLOAT_ARG(3, z);
	REQ_FLOAT_ARG(4, w);
	
	glVertexAttrib4f(location, x, y, z, w);
	RET_UNDEFINED;
	
}


GL_METHOD(VertexAttrib4fv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_OBJ_ARG(1, abv);
	
	GLfloat *data = getArrayData<GLfloat>(env, abv);
	
	glVertexAttrib4fv(location, data);
	RET_UNDEFINED;
	
}


GL_METHOD(VertexAttribPointer) { NAPI_ENV;
	
	REQ_INT32_ARG(0, indx);
	REQ_INT32_ARG(1, size);
	REQ_INT32_ARG(2, type);
	LET_BOOL_ARG(3, normalized);
	REQ_INT32_ARG(4, stride);
	REQ_OFFS_ARG(5, offset);
	
	GLvoid *vertices = reinterpret_cast<GLvoid*>(offset);
	
	glVertexAttribPointer(indx, size, type, normalized, stride, vertices);
	RET_UNDEFINED;
	
}


GL_METHOD(VertexAttribIPointer) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, indx);
	REQ_INT32_ARG(1, size);
	REQ_INT32_ARG(2, type);
	REQ_UINT32_ARG(3, stride);
	REQ_OFFS_ARG(4, offset);
	
	GLvoid *vertices = reinterpret_cast<GLvoid*>(offset);
	
	glVertexAttribIPointer(indx, size, type, stride, vertices);
	RET_UNDEFINED;
	
}


// blend
GL_METHOD(BlendColor) { NAPI_ENV;
	
	REQ_FLOAT_ARG(0, red);
	REQ_FLOAT_ARG(1, green);
	REQ_FLOAT_ARG(2, blue);
	REQ_FLOAT_ARG(3, alpha);
	
	glBlendColor(red, green, blue, alpha);
	RET_UNDEFINED;
	
}


GL_METHOD(BlendEquation) { NAPI_ENV;
	
	REQ_INT32_ARG(0, mode);
	
	glBlendEquation(mode);
	RET_UNDEFINED;
	
}


GL_METHOD(BlendEquationSeparate) { NAPI_ENV;
	
	REQ_INT32_ARG(0, modeRGB);
	REQ_INT32_ARG(1, modeAlpha);
	
	glBlendEquationSeparate(modeRGB, modeAlpha);
	RET_UNDEFINED;
	
}


GL_METHOD(BlendFunc) { NAPI_ENV;
	
	REQ_INT32_ARG(0, sfactor);
	REQ_INT32_ARG(1, dfactor);
	
	glBlendFunc(sfactor, dfactor);
	RET_UNDEFINED;
	
}


GL_METHOD(BlendFuncSeparate) { NAPI_ENV;
	
	REQ_INT32_ARG(0, srcRGB);
	REQ_INT32_ARG(1, dstRGB);
	REQ_INT32_ARG(2, srcAlpha);
	REQ_INT32_ARG(3, dstAlpha);
	
	glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
	RET_UNDEFINED;
	
}


// buffers

GL_METHOD(CreateBuffer) { NAPI_ENV;
	GLuint buffer;
	glGenBuffers(1, &buffer);
	RET_NUM(buffer);
}


GL_METHOD(DeleteBuffer) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, buffer);
	
	glDeleteBuffers(1, reinterpret_cast<GLuint*>(&buffer));
	RET_UNDEFINED;
	
}


GL_METHOD(IsBuffer) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, buffer);
	
	RET_BOOL(glIsBuffer(buffer) != 0);
	
}


GL_METHOD(BindBuffer) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_UINT32_ARG(1, buffer);
	
	glBindBuffer(target, buffer);
	RET_UNDEFINED;
	
}


GL_METHOD(BindBufferBase) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_UINT32_ARG(1, index);
	REQ_UINT32_ARG(2, buffer);
	
	glBindBufferBase(target, index, buffer);
	RET_UNDEFINED;
	
}


GL_METHOD(BindBufferRange) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_UINT32_ARG(1, index);
	REQ_UINT32_ARG(2, buffer);
	REQ_INT32_ARG(3, offset);
	REQ_INT32_ARG(4, size);
	
	glBindBufferRange(target, index, buffer, offset, size);
	RET_UNDEFINED;
	
}


GL_METHOD(BufferData) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);

	// WebGL 1
	if(info.Length() == 3) {
		if (info[1].IsObject()) {
		
		REQ_OBJ_ARG(1, arr);
		REQ_INT32_ARG(2, usage);
		
		int size;
		void* data = getArrayData<uint8_t>(env, info[1].As<Napi::TypedArray>(), &size);
		glBufferData(target, size, data, usage);

	} else if (info[1].IsNumber()) {
		
		REQ_UINT32_ARG(1, size);
		REQ_INT32_ARG(2, usage);
		
		glBufferData(target, size, NULL, usage);
		
	}
	} else {
		// WebGL 2		

		REQ_OBJ_ARG(1, arr);
		REQ_INT32_ARG(2, usage);
		REQ_INT32_ARG(3, srcOffset);
		REQ_INT32_ARG(4, length);

		int size;
		void* data = getArrayData<uint8_t>(env, info[1].As<Napi::TypedArray>(), &size, srcOffset);
		glBufferData(target, length, data, usage);
	}
	
	RET_UNDEFINED;	
}


GL_METHOD(BufferSubData) { NAPI_ENV;
	
REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, offset);
	REQ_TYPED_ARRAY_ARG(2, arr);
	
	int size = arr.ByteLength();
	
	if (info.Length() == 3)  {
		void* data = getArrayData<uint8_t>(env, arr);
		glBufferSubData(target, offset, size, data);
	} else {
		REQ_UINT32_ARG(3, srcOffset);
		REQ_UINT32_ARG(4, length);
		
		void* data = getArrayData<uint8_t>(env, arr, nullptr, srcOffset);
		glBufferSubData(target, offset, length, data);
	}

	RET_UNDEFINED;
	
}


GL_METHOD(CopyBufferSubData) { NAPI_ENV;

	REQ_INT32_ARG(0, readTarget);
	REQ_INT32_ARG(1, writeTarget);
	REQ_INT32_ARG(2, readOffset);
	REQ_INT32_ARG(3, writeOffset);
	REQ_INT32_ARG(4, size);
	
	glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
	RET_UNDEFINED;
	
}

// not opengl es 
GL_METHOD(GetBufferSubData) { NAPI_ENV;

	REQ_INT32_ARG(0, readTarget);
	REQ_INT32_ARG(1, sourceByteOffset);
	REQ_TYPED_ARRAY_ARG(2, dest);
	LET_OFFS_ARG(3, destByteOffset);
	LET_OFFS_ARG(4, length);
	
	size_t elementSize = dest.ElementSize();
	size_t size = std::min(dest.ByteLength(), length * elementSize);
	size_t offset = destByteOffset * elementSize;
	
	void* data = getArrayData<uint8_t>(env, dest) + offset;

	std::cerr << "GL GET BUFFER SUBDATA NOT SUPPORTED" << std::endl;
	// glGetBufferSubData(readTarget, sourceByteOffset, size, data);
	RET_UNDEFINED;
	
}


GL_METHOD(GetBufferParameter) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, name);
	
	GLint params;
	glGetBufferParameteriv(target, name, &params);
	
	RET_NUM(params);
	
}


// framebuffers

GL_METHOD(CreateFramebuffer) { NAPI_ENV;
	
	GLuint buffer;
	glGenFramebuffers(1, &buffer);
	
	RET_NUM(buffer);
	
}


GL_METHOD(DeleteFramebuffer) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, buffer);
	
	glDeleteFramebuffers(1, reinterpret_cast<GLuint*>(&buffer));
	RET_UNDEFINED;
	
}


GL_METHOD(IsFramebuffer) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, buffer);
	
	RET_BOOL(glIsFramebuffer(buffer) != 0);
	
}


GL_METHOD(BindFramebuffer) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	LET_INT32_ARG(1, buffer);
	
	glBindFramebuffer(target, buffer);
	RET_UNDEFINED;
	
}


GL_METHOD(BindFrameBuffer) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, fbo);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	RET_UNDEFINED;
	
}


GL_METHOD(BlitFrameBuffer) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, fbo1);
	REQ_UINT32_ARG(1, fbo2);
	REQ_UINT32_ARG(2, sw);
	REQ_UINT32_ARG(3, sh);
	REQ_UINT32_ARG(4, dw);
	REQ_UINT32_ARG(5, dh);
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo1);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo2);
	
	glBlitFramebuffer(0, 0, sw, sh, 0, 0, dw, dh, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RET_UNDEFINED;
	
}


GL_METHOD(CheckFramebufferStatus) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	
	RET_NUM(glCheckFramebufferStatus(target));
	
}


GL_METHOD(FramebufferRenderbuffer) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, attachment);
	REQ_INT32_ARG(2, renderbuffertarget);
	REQ_UINT32_ARG(3, renderbuffer);
	
	glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
	RET_UNDEFINED;
	
}


GL_METHOD(FramebufferTexture2D) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, attachment);
	REQ_INT32_ARG(2, textarget);
	REQ_INT32_ARG(3, texture);
	REQ_INT32_ARG(4, level);
	
	glFramebufferTexture2D(target, attachment, textarget, texture, level);
	RET_UNDEFINED;
	
}


GL_METHOD(GetFramebufferAttachmentParameter) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, attachment);
	REQ_INT32_ARG(2, name);
	
	GLint params;
	glGetFramebufferAttachmentParameteriv(target, attachment, name, &params);
	
	RET_NUM(params);
	
}


// instances 


GL_METHOD(DrawArraysInstanced) { NAPI_ENV;
	
	REQ_INT32_ARG(0, mode);
	REQ_INT32_ARG(1, first);
	REQ_INT32_ARG(2, count);
	REQ_INT32_ARG(3, primcount);
	
	glDrawArraysInstanced(mode, first, count, primcount);
	RET_UNDEFINED;
	
}


GL_METHOD(DrawElementsInstanced) { NAPI_ENV;
	REQ_INT32_ARG(0, mode);
	REQ_INT32_ARG(1, count);
	REQ_INT32_ARG(2, type);
	REQ_OFFS_ARG(3, ptr);
	REQ_INT32_ARG(4, primcount);

	GLvoid *indices = reinterpret_cast<GLvoid*>(ptr);
	glDrawElementsInstanced(mode, count, type, indices, primcount);
	RET_UNDEFINED;
	
}


GL_METHOD(VertexAttribDivisor) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, index);
	REQ_UINT32_ARG(1, divisor);
	
	glVertexAttribDivisor(index, divisor);
	RET_UNDEFINED;
	
}



// programs

GL_METHOD(CreateProgram) { NAPI_ENV;
	GLuint program = glCreateProgram();
	RET_NUM(program);
}


GL_METHOD(DeleteProgram) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, program);
	
	glDeleteProgram(program);
	RET_UNDEFINED;
	
}


GL_METHOD(IsProgram) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, program);
	
	RET_BOOL(glIsProgram(program) != 0);
	RET_UNDEFINED;
	
}


GL_METHOD(GetProgramInfoLog) { NAPI_ENV;
	
	REQ_INT32_ARG(0, program);
	
	int Len = 1024;
	char Error[1024];
	glGetProgramInfoLog(program, 1024, &Len, Error);
	
	RET_STR(Error);
	
}


#define CASES_PROGRAM_PARAM_BOOL case GL_DELETE_STATUS: \
	case GL_LINK_STATUS: \
	case GL_VALIDATE_STATUS:

#define CASES_PROGRAM_PARAM_ENUM case GL_TRANSFORM_FEEDBACK_BUFFER_MODE:

#define CASES_PROGRAM_PARAM_NUMBER case GL_ATTACHED_SHADERS: \
	case GL_ACTIVE_ATTRIBUTES: \
	case GL_ACTIVE_UNIFORMS: \
	case GL_ACTIVE_UNIFORM_BLOCKS: \
	case GL_INFO_LOG_LENGTH: \
	case GL_TRANSFORM_FEEDBACK_VARYINGS:

GL_METHOD(GetProgramParameter) { NAPI_ENV;
	
	REQ_INT32_ARG(0, program);
	REQ_INT32_ARG(1, name);
	
	int value = 0;
	
	switch (name) {
	
	CASES_PROGRAM_PARAM_BOOL
		glGetProgramiv(program, name, &value);
		RET_BOOL(value != 0);
		break;
	
	CASES_PROGRAM_PARAM_ENUM
		glGetProgramiv(program, name, &value);
		RET_NUM(value);
		break;
	
	CASES_PROGRAM_PARAM_NUMBER
		glGetProgramiv(program, name, &value);
		RET_NUM(value);
		break;
	
	default:
		JS_THROW("GetProgramParameter: Invalid Enum");
		RET_NULL;
	}
	
}


GL_METHOD(LinkProgram) { NAPI_ENV;
	
	REQ_INT32_ARG(0, id);
	
	glLinkProgram(id);
	RET_UNDEFINED;
	
}


GL_METHOD(UseProgram) { NAPI_ENV;
	
	REQ_INT32_ARG(0, id);
	
	glUseProgram(id);
	RET_UNDEFINED;
	
}


GL_METHOD(ValidateProgram) { NAPI_ENV;
	
	REQ_INT32_ARG(0, program);
	
	glValidateProgram(program);
	RET_UNDEFINED;
	
}



// renderbuffers


GL_METHOD(CreateRenderbuffer) { NAPI_ENV;
	
	GLuint renderbuffers;
	glGenRenderbuffers(1, &renderbuffers);
	
	RET_NUM(renderbuffers);
	
}


GL_METHOD(DeleteRenderbuffer) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, renderbuffer);
	
	glDeleteRenderbuffers(1, reinterpret_cast<GLuint*>(&renderbuffer));
	RET_UNDEFINED;
	
}


GL_METHOD(IsRenderbuffer) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, buffer);
	
	RET_BOOL(glIsRenderbuffer(buffer) != 0);
	
}


GL_METHOD(BindRenderbuffer) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	LET_INT32_ARG(1, buffer);
	glBindRenderbuffer(target, buffer);
	RET_UNDEFINED;
	
}


GL_METHOD(GetRenderbufferParameter) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, name);
	
	int value = 0;
	glGetRenderbufferParameteriv(target, name, &value);
	
	RET_NUM(value);
	
}


GL_METHOD(RenderbufferStorage) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, internalformat);
	REQ_UINT32_ARG(2, width);
	REQ_UINT32_ARG(3, height);


	
	if(internalformat == 34041) {
		glRenderbufferStorage(target, GL_DEPTH24_STENCIL8, width, height);
	} else {
		glRenderbufferStorage(target, internalformat, width, height);
	}
	
	RET_UNDEFINED;
	
}

GL_METHOD(RenderbufferStorageMultisample) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, target);
	REQ_UINT32_ARG(1, samples);
	REQ_UINT32_ARG(2, internalformat);
	REQ_UINT32_ARG(3, width);
	REQ_UINT32_ARG(4, height);
	
	glRenderbufferStorageMultisample(target, samples, internalformat, width, height);
	RET_UNDEFINED;
	
}

GL_METHOD(BlitFramebuffer) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, srcX0);
	REQ_UINT32_ARG(1, srcY0);
	REQ_UINT32_ARG(2, srcX1);
	REQ_UINT32_ARG(3, srcY1);

	REQ_UINT32_ARG(4, dstX0);
	REQ_UINT32_ARG(5, dstY0);
	REQ_UINT32_ARG(6, dstX1);
	REQ_UINT32_ARG(7, dstY1);
	
	REQ_UINT32_ARG(8, mask);
	REQ_UINT32_ARG(9, filter);

	glBlitFramebuffer(srcX0, srcY0, srcX1,srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	RET_UNDEFINED;
	
}





// Shaders

GL_METHOD(CreateShader) { NAPI_ENV;
	
	REQ_INT32_ARG(0, id);
	
	GLuint shader = glCreateShader(id);
	
	RET_NUM(shader);
	
}



GL_METHOD(DeleteShader) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, shader);
	
	glDeleteShader(shader);
	RET_UNDEFINED;
	
}


GL_METHOD(IsShader) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, shader);
	
	RET_BOOL(glIsShader(shader) != 0);
	
}


GL_METHOD(AttachShader) { NAPI_ENV;
	
	REQ_INT32_ARG(0, program);
	REQ_INT32_ARG(1, shader);
	
	glAttachShader(program, shader);
	RET_UNDEFINED;
	
}


GL_METHOD(CompileShader) { NAPI_ENV;
	
	REQ_INT32_ARG(0, id);
	
	glCompileShader(id);
	RET_UNDEFINED;
	
}


GL_METHOD(DetachShader) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, program);
	REQ_UINT32_ARG(1, shader);
	
	glDetachShader(program, shader);
	RET_UNDEFINED;
	
}


GL_METHOD(GetAttachedShaders) { NAPI_ENV;
	
	REQ_INT32_ARG(0, program);
	
	GLuint shaders[1024];
	GLsizei count;
	glGetAttachedShaders(program, 1024, &count, shaders);
	
	Napi::Array shadersArr = Napi::Array::New(env);
	for (int i = 0; i < count; i++) {
		shadersArr.Set(i, JS_NUM(static_cast<int>(shaders[i])));
	}
	
	RET_VALUE(shadersArr);
	
}


GL_METHOD(GetShaderInfoLog) { NAPI_ENV;
	
	REQ_INT32_ARG(0, id);
	
	int len = 1024;
	char error[1024];
	glGetShaderInfoLog(id, 1024, &len, error);
	
	RET_STR(error);
	
}


#define CASES_SHADER_PARAM_BOOL case GL_DELETE_STATUS: \
	case GL_COMPILE_STATUS:

#define CASES_SHADER_PARAM_NUMBER case GL_SHADER_TYPE: \
	case GL_INFO_LOG_LENGTH: \
	case GL_SHADER_SOURCE_LENGTH:

GL_METHOD(GetShaderParameter) { NAPI_ENV;
	
	REQ_INT32_ARG(0, shader);
	REQ_INT32_ARG(1, pname);
	
	int value = 0;
	
	switch (pname) {
	
	CASES_SHADER_PARAM_BOOL
		glGetShaderiv(shader, pname, &value);
		RET_BOOL(value != 0);
		break;
	
	CASES_SHADER_PARAM_NUMBER
		glGetShaderiv(shader, pname, &value);
		RET_NUM(value);
		break;
	
	default:
		JS_THROW("GetShaderParameter: Invalid Enum");
		RET_NULL;
	}
	
}


GL_METHOD(GetShaderSource) { NAPI_ENV;
	
	REQ_INT32_ARG(0, shader);
	
	GLint len;
	glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &len);
	GLchar *source = new GLchar[len];
	glGetShaderSource(shader, len, NULL, source);
	
	Napi::String str = JS_STR(source);
	delete [] source;
	
	RET_VALUE(str);
	
}


GL_METHOD(ShaderSource) { NAPI_ENV;
	
	REQ_INT32_ARG(0, id);
	REQ_STR_ARG(1, code);
	
	const char *strings = code.c_str();
	GLint lengths = code.length();
	
	glShaderSource(id, 1, &strings, &lengths);
	RET_UNDEFINED;
	
}

GL_METHOD(GetShaderPrecisionFormat) { NAPI_ENV;
	REQ_UINT32_ARG(0, shaderType);
	REQ_UINT32_ARG(1, precisionType);


	GLint range[2];
	GLint precision;

	glGetShaderPrecisionFormat(shaderType, precisionType, range, &precision);
	
	Napi::Object result = Napi::Object::New(env);
	result.Set(Napi::String::New(env, "rangeMin"), Napi::Number::New(env, range[0]));
	result.Set(Napi::String::New(env, "rangeMax"), Napi::Number::New(env, range[1]));
	result.Set(Napi::String::New(env, "precision"), Napi::Number::New(env, precision));
	
	//glGetShaderPrecisionFormat(target, mode);
	return result;
}




// Stencil

GL_METHOD(ClearStencil) { NAPI_ENV;
	
	REQ_INT32_ARG(0, s);
	
	glClearStencil(s);
	RET_UNDEFINED;
	
}


GL_METHOD(StencilFunc) { NAPI_ENV;
	
	REQ_INT32_ARG(0, func);
	REQ_INT32_ARG(1, ref);
	REQ_INT32_ARG(2, mask);
	
	glStencilFunc(func, ref, mask);
	RET_UNDEFINED;
	
}


GL_METHOD(StencilFuncSeparate) { NAPI_ENV;
	
	REQ_INT32_ARG(0, face);
	REQ_INT32_ARG(1, func);
	REQ_INT32_ARG(2, ref);
	REQ_INT32_ARG(3, mask);
	
	glStencilFuncSeparate(face, func, ref, mask);
	RET_UNDEFINED;
	
}


GL_METHOD(StencilMask) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, mask);
	
	glStencilMask(mask);
	RET_UNDEFINED;
	
}


GL_METHOD(StencilMaskSeparate) { NAPI_ENV;
	
	REQ_INT32_ARG(0, face);
	REQ_UINT32_ARG(1, mask);
	
	glStencilMaskSeparate(face, mask);
	RET_UNDEFINED;
	
}


GL_METHOD(StencilOp) { NAPI_ENV;
	
	REQ_INT32_ARG(0, fail);
	REQ_INT32_ARG(1, zfail);
	REQ_INT32_ARG(2, zpass);
	
	glStencilOp(fail, zfail, zpass);
	RET_UNDEFINED;
	
}


GL_METHOD(StencilOpSeparate) { NAPI_ENV;
	
	REQ_INT32_ARG(0, face);
	REQ_INT32_ARG(1, fail);
	REQ_INT32_ARG(2, zfail);
	REQ_INT32_ARG(2, zpass);
	
	glStencilOpSeparate(face, fail, zfail, zpass);
	RET_UNDEFINED;
	
}


// textures
unsigned char* WebGLRenderingContext::unpackPixels(GLenum type, GLenum format, GLint width, GLint height, unsigned char* pixels) {

  //Compute pixel size
  GLint pixelSize = 1;
  if(type == GL_UNSIGNED_BYTE || type == GL_FLOAT) {
    if(type == GL_FLOAT) {
      pixelSize = 4;
    }
    switch(format) {
      case GL_ALPHA:
      case GL_LUMINANCE:
      break;
      case GL_LUMINANCE_ALPHA:
        pixelSize *= 2;
      break;
      case GL_RGB:
        pixelSize *= 3;
      break;
      case GL_RGBA:
        pixelSize *= 4;
      break;
    }
  } else {
    pixelSize = 2;
  }

  //Compute row stride
  GLint rowStride = pixelSize * width;
  if((rowStride % unpack_alignment) != 0) {
    rowStride += unpack_alignment - (rowStride % unpack_alignment);
  }

  GLint imageSize = rowStride * height;
  unsigned char* unpacked = new unsigned char[imageSize];

  if(unpack_flip_y) {
    for(int i=0,j=height-1; j>=0; ++i, --j) {
      memcpy(
          reinterpret_cast<void*>(unpacked + j*rowStride)
        , reinterpret_cast<void*>(pixels   + i*rowStride)
        , width * pixelSize);
    }
  } else {
    memcpy(
        reinterpret_cast<void*>(unpacked)
      , reinterpret_cast<void*>(pixels)
      , imageSize);
  }

  //Premultiply alpha unpacking
  if(unpack_premultiply_alpha && (format == GL_LUMINANCE_ALPHA || format == GL_RGBA)) {

    for(int row=0; row<height; ++row) {
      for(int col=0; col<width; ++col) {
        unsigned char* pixel = unpacked + (row * rowStride) + (col * pixelSize);
        if(format == GL_LUMINANCE_ALPHA) {
          pixel[0] *= pixel[1] / 255.0;
        } else if(type == GL_UNSIGNED_BYTE) {
          float scale = pixel[3] / 255.0;
          pixel[0] *= scale;
          pixel[1] *= scale;
          pixel[2] *= scale;
        } else if(type == GL_UNSIGNED_SHORT_4_4_4_4) {
          int r = pixel[0]&0x0f;
          int g = pixel[0]>>4;
          int b = pixel[1]&0x0f;
          int a = pixel[1]>>4;

          float scale = a / 15.0;
          r *= scale;
          g *= scale;
          b *= scale;

          pixel[0] = r + (g<<4);
          pixel[1] = b + (a<<4);
        } else if(type == GL_UNSIGNED_SHORT_5_5_5_1) {
          if((pixel[0]&1) == 0) {
            pixel[0] = 1; //why does this get set to 1?!?!?!
            pixel[1] = 0;
          }
        }
      }
    }
  }

  return unpacked;
}


GL_METHOD(CreateTexture) { NAPI_ENV;
	
	GLuint texture;
	glGenTextures(1, &texture);
	
	RET_NUM(texture);
	
}


GL_METHOD(DeleteTexture) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, texture);
	
	glDeleteTextures(1, reinterpret_cast<GLuint*>(&texture));
	RET_UNDEFINED;
	
}


GL_METHOD(IsTexture) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, texture);
	
	RET_BOOL(glIsTexture(texture) != 0);
	
}


GL_METHOD(BindTexture) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	LET_INT32_ARG(1, texture);
	
	glBindTexture(target, texture);
	RET_UNDEFINED;
	
}


GL_METHOD(ActiveTexture) { NAPI_ENV;
	
	REQ_INT32_ARG(0, id);
	
	glActiveTexture(id);
	RET_UNDEFINED;
	
}


GL_METHOD(CopyTexImage2D) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, level);
	REQ_INT32_ARG(2, internalformat);
	REQ_INT32_ARG(3, x);
	REQ_INT32_ARG(4, y);
	REQ_INT32_ARG(5, width);
	REQ_INT32_ARG(6, height);
	REQ_INT32_ARG(7, border);
	
	glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
	RET_UNDEFINED;
	
}


GL_METHOD(CopyTexSubImage2D) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, level);
	REQ_INT32_ARG(2, xoffset);
	REQ_INT32_ARG(3, yoffset);
	REQ_INT32_ARG(4, x);
	REQ_INT32_ARG(5, y);
	REQ_INT32_ARG(6, width);
	REQ_INT32_ARG(7, height);

	
	glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
	RET_UNDEFINED;
	
}


GL_METHOD(GenerateMipmap) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	
	glGenerateMipmap(target);
	RET_UNDEFINED;
	
}


GL_METHOD(GetTexParameter) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, name);
	
	GLint param_value = 0;
	glGetTexParameteriv(target, name, &param_value);
	
	RET_NUM(param_value);
	
}


GL_METHOD(TexImage2D) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, level);
	REQ_INT32_ARG(2, internalformat);
	REQ_INT32_ARG(3, width);
	REQ_INT32_ARG(4, height);
	REQ_INT32_ARG(5, border);
	REQ_INT32_ARG(6, format);
	REQ_INT32_ARG(7, type);
	
	if (info.Length() <= 8 || IS_ARG_EMPTY(8)) {
		
		glTexImage2D(
			target,
			level,
			internalformat,
			width,
			height,
			border,
			format,
			type,
			nullptr
		);
		
	} else if (info[8].IsNumber()) {
		
		// In WebGL2 the last parameter can be a byte offset if glBindBuffer()
		// was called with GL_PIXEL_UNPACK_BUFFER prior to glTexImage2D
		REQ_OFFS_ARG(8, offset);
		// From https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glBindBuffer.xhtml
		// "The pointer parameter is interpreted as an offset within the buffer
		// object measured in basic machine units."
		glTexImage2D(
			target,
			level,
			internalformat,
			width,
			height,
			border,
			format,
			type,
			reinterpret_cast<void *>(offset)
		);
		
	} else {
		
		REQ_OBJ_ARG(8, image);
		
		void *ptr;
		int offset = 0;
		if (info.Length() == 10) {
			REQ_INT32_ARG(9, srcOffset);
			offset = srcOffset;
		}
        
		if(unpack_flip_y || unpack_premultiply_alpha) {
			ptr = (void*)unpackPixels(type, format, width, height, reinterpret_cast<uint8_t*>(getData(env, image, offset)));
		} else {
			ptr = getData(env, image, offset);
		}
		
		glTexImage2D(
			target,
			level,
			internalformat,
			width,
			height,
			border,
			format,
			type,
			ptr
		);
		
	}
	
	RET_UNDEFINED;
	
}

GL_METHOD(CreateSampler) { NAPI_ENV;
	GLuint samplers;
	glGenSamplers(1, &samplers);
	RET_NUM(samplers);
}

GL_METHOD(SamplerParameteri) { NAPI_ENV;
	REQ_UINT32_ARG(0, sampler);
	REQ_UINT32_ARG(1, pname);
	REQ_INT32_ARG(2, param);
	glSamplerParameteri(sampler, pname, param);
	RET_UNDEFINED;
}

GL_METHOD(BindSampler) { NAPI_ENV;
	REQ_UINT32_ARG(0, unit);
	REQ_UINT32_ARG(1, sampler);
	glBindSampler(unit, sampler);
	RET_UNDEFINED;
}


GL_METHOD(TexStorage2D) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, target);
	REQ_INT32_ARG(1, levels);
	REQ_UINT32_ARG(2, internalFormat);

	
	REQ_UINT32_ARG(3, width);
	REQ_UINT32_ARG(4, height);

	glTexStorage2D(target, levels, internalFormat, width, height);
	RET_UNDEFINED;
}



GL_METHOD(TexParameterf) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, name);
	REQ_FLOAT_ARG(2, param);
	
	glTexParameterf(target, name, param);
	RET_UNDEFINED;
	
}


GL_METHOD(TexParameteri) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, name);
	REQ_INT32_ARG(2, param);
	
	glTexParameteri(target, name, param);
	RET_UNDEFINED;
	
}


GL_METHOD(TexSubImage2D) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_INT32_ARG(1, level);
	REQ_INT32_ARG(2, xoffset);
	REQ_INT32_ARG(3, yoffset);
	REQ_INT32_ARG(4, width);
	REQ_INT32_ARG(5, height);
	REQ_INT32_ARG(6, format);
	REQ_INT32_ARG(7, type);
	
	if (info.Length() <= 8 || IS_ARG_EMPTY(8)) {
		
		glTexSubImage2D(
			target,
			level,
			xoffset,
			yoffset,
			width,
			height,
			format,
			type,
			nullptr
		);
		
	} else {
		
		REQ_OBJ_ARG(8, image);
		void *ptr;
		int offset = 0;
		if (info.Length() == 10) {
			REQ_INT32_ARG(9, srcOffset);
			offset = srcOffset;
		}
        
		
		void *pixels = getData(env, image, offset);
		glTexSubImage2D(
			target,
			level,
			xoffset,
			yoffset,
			width,
			height,
			format,
			type,
			pixels
		);
		
	}
	
	RET_UNDEFINED;
	
}

// Transformfeedback


GL_METHOD(CreateTransformFeedback) { NAPI_ENV;
	
	GLuint transformFeedback;
	glGenTransformFeedbacks(1, &transformFeedback);
	
	RET_NUM(transformFeedback);
	
}


GL_METHOD(DeleteTransformFeedback) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, transformFeedback);
	
	glDeleteTransformFeedbacks(1, reinterpret_cast<GLuint*>(&transformFeedback));
	RET_UNDEFINED;
	
}


GL_METHOD(IsTransformFeedback) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, transformFeedback);
	
	RET_VALUE(JS_BOOL(glIsTransformFeedback(transformFeedback) != 0));
	RET_UNDEFINED;
	
}


GL_METHOD(BindTransformFeedback) { NAPI_ENV;
	
	REQ_INT32_ARG(0, target);
	REQ_UINT32_ARG(1, transformFeedback);
	
	glBindTransformFeedback(target, transformFeedback);
	RET_UNDEFINED;
	
}


GL_METHOD(BeginTransformFeedback) { NAPI_ENV;
	
	REQ_INT32_ARG(0, mode);
	glBeginTransformFeedback(mode);
	RET_UNDEFINED;
	
}


GL_METHOD(EndTransformFeedback) { NAPI_ENV;
	glEndTransformFeedback();
	RET_UNDEFINED;
}


GL_METHOD(PauseTransformFeedback) { NAPI_ENV;
	glPauseTransformFeedback();
	RET_UNDEFINED;
}


GL_METHOD(ResumeTransformFeedback) { NAPI_ENV;
	glResumeTransformFeedback();
	RET_UNDEFINED;
}


GL_METHOD(TransformFeedbackVaryings) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, program);
	REQ_ARRAY_ARG(1, jsVaryings);
	REQ_INT32_ARG(2, bufferMode);
	
	uint32_t count = jsVaryings.Length();
	std::string *cppVaryings = new std::string[count];
	const char **varyings = new const char* [count];
	
	for (uint32_t i = 0; i < count; i++) {
		cppVaryings[i] = jsVaryings.Get(i).As<Napi::String>().Utf8Value();
		varyings[i] = cppVaryings[i].c_str();
	}
	
	glTransformFeedbackVaryings(program, count, varyings, bufferMode);
	
	delete [] varyings;
	delete [] cppVaryings;
	
	RET_UNDEFINED;
	
}


GL_METHOD(GetTransformFeedbackVarying) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, program);
	REQ_UINT32_ARG(1, index);
	
	GLsizei len;
	GLenum type;
	GLsizei size;
	
	char name[1024];
	
	glGetTransformFeedbackVarying(program, index, 1024, &len, &size, &type, name);
	
	Napi::Array activeInfo = Napi::Array::New(env);
	activeInfo.Set("size", JS_NUM(size));
	activeInfo.Set("type", JS_NUM(static_cast<int>(type)));
	activeInfo.Set("name", JS_STR(name));
	
	RET_VALUE(activeInfo);
	
}


// Uniforms

GL_METHOD(GetActiveUniform) { NAPI_ENV;
	
	REQ_INT32_ARG(0, program);
	REQ_INT32_ARG(1, index);
	
	char name[1024];
	GLsizei length = 0;
	GLenum type;
	GLsizei size;
	glGetActiveUniform(program, index, 1024, &length, &size, &type, name);
	
	Napi::Array activeInfo = Napi::Array::New(env);
	activeInfo.Set("size", JS_NUM(size));
	activeInfo.Set("type", JS_NUM(static_cast<int>(type)));
	activeInfo.Set("name", JS_STR(name));
	
	RET_VALUE(activeInfo);
	
}


GL_METHOD(GetUniform) { NAPI_ENV;
	
	REQ_INT32_ARG(0, program);
	REQ_INT32_ARG(1, location);
	
	if (location < 0) {
		RET_UNDEFINED;
	}
	
	float data[16]; // worst case scenario is 16 floats
	glGetUniformfv(program, location, data);
	
	Napi::Array arr = Napi::Array::New(env);
	for (int i = 0; i < 16; i++) {
		arr.Set(i, JS_NUM(data[i]));
	}
	
	RET_VALUE(arr);
	
}


GL_METHOD(GetUniformLocation) { NAPI_ENV;
	
	REQ_INT32_ARG(0, program);
	REQ_STR_ARG(1, name);
	
	RET_NUM(glGetUniformLocation(program, name.c_str()));
	
}


GL_METHOD(Uniform1f) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_FLOAT_ARG(1, x);
	
	glUniform1f(location, x);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform2f) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_FLOAT_ARG(1, x);
	REQ_FLOAT_ARG(2, y);
	
	glUniform2f(location, x, y);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform3f) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_FLOAT_ARG(1, x);
	REQ_FLOAT_ARG(2, y);
	REQ_FLOAT_ARG(3, z);
	
	glUniform3f(location, x, y, z);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform4f) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_FLOAT_ARG(1, x);
	REQ_FLOAT_ARG(2, y);
	REQ_FLOAT_ARG(3, z);
	REQ_FLOAT_ARG(4, w);
	
	glUniform4f(location, x, y, z, w);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform1i) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_INT32_ARG(1, x);
	
	glUniform1i(location, x);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform2i) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_INT32_ARG(1, x);
	REQ_INT32_ARG(2, y);
	
	glUniform2i(location, x, y);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform3i) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_INT32_ARG(1, x);
	REQ_INT32_ARG(2, y);
	REQ_INT32_ARG(3, z);
	
	glUniform3i(location, x, y, z);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform4i) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_INT32_ARG(1, x);
	REQ_INT32_ARG(2, y);
	REQ_INT32_ARG(3, z);
	REQ_INT32_ARG(4, w);
	
	glUniform4i(location, x, y, z, w);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform1fv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_OBJ_ARG(1, abv);

	int offset = 0;
	int length = 0;
	if (info.Length() > 3) {
		offset = info[2].As<Napi::Number>();
		length = info[3].As<Napi::Number>();
	}
	
	
	int num  = length;
	GLfloat *ptr = getArrayData<GLfloat>(env, abv, !length ? &num : nullptr, offset);
	
	glUniform1fv(location, num, ptr);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform2fv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_OBJ_ARG(1, abv);
	
	int offset = 0;
	int length = 0;
	if (info.Length() > 3) {
		offset = info[2].As<Napi::Number>();
		length = info[3].As<Napi::Number>();
	}
	
	int num  = length;
	GLfloat *ptr = getArrayData<GLfloat>(env, abv, !length ? &num : nullptr, offset);
	num /= 2;
	
	glUniform2fv(location, num, ptr);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform3fv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_OBJ_ARG(1, abv);
	
	int offset = 0;
	int length = 0;
	if (info.Length() > 3) {
		offset = info[2].As<Napi::Number>();
		length = info[3].As<Napi::Number>();
	}
	
	int num  = length;
	GLfloat *ptr = getArrayData<GLfloat>(env, abv, !length ? &num : nullptr, offset);
	num /= 3;
	
	glUniform3fv(location, num, ptr);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform4fv) { NAPI_ENV;
	REQ_INT32_ARG(0, location);
	REQ_OBJ_ARG(1, abv);
	
	int offset = 0;
	int length = 0;
	if (info.Length() > 3) {
		offset = info[2].As<Napi::Number>();
		length = info[3].As<Napi::Number>();
	}
	
	int num  = length;
	GLfloat *ptr = getArrayData<GLfloat>(env, abv, !length ? &num : nullptr, offset);
	num /= 4;

	glUniform4fv(location, num, ptr);
	RET_UNDEFINED;
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform1iv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_OBJ_ARG(1, abv);
	
	int offset = 0;
	int length = 0;
	if (info.Length() > 3) {
		offset = info[2].As<Napi::Number>();
		length = info[3].As<Napi::Number>();
	}
	
	int num  = length;
	GLint *ptr = getArrayData<GLint>(env, abv, !length ? &num : nullptr, offset);
	
	glUniform1iv(location, num, ptr);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform2iv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_OBJ_ARG(1, abv);
	
	int offset = 0;
	int length = 0;
	if (info.Length() > 3) {
		offset = info[2].As<Napi::Number>();
		length = info[3].As<Napi::Number>();
	}
	
	int num  = length;
	GLint *ptr = getArrayData<GLint>(env, abv, !length ? &num : nullptr, offset);
	num /= 2;
	
	glUniform2iv(location, num, ptr);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform3iv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_OBJ_ARG(1, abv);
	
	int offset = 0;
	int length = 0;
	if (info.Length() > 3) {
		offset = info[2].As<Napi::Number>();
		length = info[3].As<Napi::Number>();
	}
	
	int num  = length;
	GLint *ptr = getArrayData<GLint>(env, abv, !length ? &num : nullptr, offset);
	num /= 3;
	
	glUniform3iv(location, num, ptr);
	RET_UNDEFINED;
	
}


GL_METHOD(Uniform4iv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	REQ_OBJ_ARG(1, abv);
	
	int offset = 0;
	int length = 0;
	if (info.Length() > 3) {
		offset = info[2].As<Napi::Number>();
		length = info[3].As<Napi::Number>();
	}
	
	int num  = length;
	GLint *ptr = getArrayData<GLint>(env, abv, !length ? &num : nullptr, offset);
	num /= 4;
	
	glUniform4iv(location, num, ptr);
	RET_UNDEFINED;
	
}


GL_METHOD(UniformMatrix2fv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	LET_BOOL_ARG(1, transpose);
	REQ_OBJ_ARG(2, abv);
	
	int offset = 0;
	int length = 0;
	if(info.Length() == 5) {
		offset = info[3].As<Napi::Number>();
		length = info[4].As<Napi::Number>();
	}

	GLsizei count = length;
	GLfloat* data = getArrayData<GLfloat>(env, abv, !length ? &count : nullptr, offset);
	
	if (count < 4) {
		JS_THROW("Not enough data for UniformMatrix2fv");
	} else {
		glUniformMatrix2fv(location, count / 4, transpose, data);
	}
	
	RET_UNDEFINED;
	
}


GL_METHOD(UniformMatrix3fv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	LET_BOOL_ARG(1, transpose);
	REQ_OBJ_ARG(2, abv);
	
	int offset = 0;
	int length = 0;
	if(info.Length() == 5) {
		offset = info[3].As<Napi::Number>();
		length = info[4].As<Napi::Number>();
	}

	GLsizei count = length;
	GLfloat* data = getArrayData<GLfloat>(env, abv, !length ? &count : nullptr, offset);
	
	if (count < 9) {
		JS_THROW("Not enough data for UniformMatrix3fv");
	} else {
		glUniformMatrix3fv(location, count / 9, transpose, data);
	}
	
	RET_UNDEFINED;
	
}


GL_METHOD(UniformMatrix4fv) { NAPI_ENV;
	
	REQ_INT32_ARG(0, location);
	LET_BOOL_ARG(1, transpose);
	REQ_OBJ_ARG(2, abv);
	
	
	int offset = 0;
	int length = 0;
	if(info.Length() == 5) {
		offset = info[3].As<Napi::Number>();
		length = info[4].As<Napi::Number>();
	}

	GLsizei count = length;
	GLfloat* data = getArrayData<GLfloat>(env, abv, !length ? &count : nullptr, offset);
	
	if (count < 16) {
		JS_THROW("Not enough data for UniformMatrix4fv");
		RET_UNDEFINED;
	} else {
		glUniformMatrix4fv(location, count / 16, transpose, data);
	}
	
	RET_UNDEFINED;
	
}


// Vertex arrays
GL_METHOD(CreateVertexArray) { NAPI_ENV;
	GLuint vertexarray;
	glGenVertexArrays(1, &vertexarray);
	RET_NUM(vertexarray);
}


GL_METHOD(DeleteVertexArray) { NAPI_ENV;
	REQ_UINT32_ARG(0, vertexarray);
	glDeleteVertexArrays(1, reinterpret_cast<GLuint*>(&vertexarray));
	RET_UNDEFINED;
	
}


GL_METHOD(IsVertexArray) { NAPI_ENV;
	REQ_UINT32_ARG(0, vertexarray);
	RET_BOOL(glIsVertexArray(vertexarray) != 0);
	
}


GL_METHOD(BindVertexArray) { NAPI_ENV;
	REQ_UINT32_ARG(0, vertexarray);
	glBindVertexArray(vertexarray);
	RET_UNDEFINED;
	
}
