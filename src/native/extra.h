#include <napi.h>
#include <iostream>

#define NAPI_ENV Napi::Env env = info.Env();
#define NAPI_HS Napi::HandleScope scope(env);

#define JS_STR(VAL) Napi::String::New(env, VAL)
#define JS_NUM(VAL) Napi::Number::New(env, static_cast<double>(VAL))
#define JS_EXT(VAL) Napi::External<void>::New(env, reinterpret_cast<void*>(VAL))
#define JS_BOOL(VAL) Napi::Boolean::New(env, static_cast<bool>(VAL))
#define JS_FUN(VAL) Napi::Function::New(env, VAL)
#define JS_OBJ(VAL) Napi::Object::New(env, VAL)


#define RET_VALUE(VAL) return VAL;
#define RET_UNDEFINED RET_VALUE(env.Undefined())
#define RET_NULL RET_VALUE(env.Null())
#define RET_STR(VAL) RET_VALUE(JS_STR(VAL))
#define RET_NUM(VAL) RET_VALUE(JS_NUM(VAL))
#define RET_EXT(VAL) RET_VALUE(JS_EXT(VAL))
#define RET_BOOL(VAL) RET_VALUE(JS_BOOL(VAL))
#define RET_FUN(VAL) RET_VALUE(JS_FUN(VAL))
#define RET_OBJ(VAL) RET_VALUE(JS_OBJ(VAL))


#define JS_THROW(VAL)                                                         \
	Napi::Error::New(env, VAL).ThrowAsJavaScriptException();


#define REQ_ARGS(N)                                                           \
	if (info.Length() < (N)) {                                                \
		JS_THROW("Expected at least " #N " arguments");                       \
		RET_UNDEFINED;                                                        \
	}


#define IS_EMPTY(VAL) (VAL.IsNull() || VAL.IsUndefined())
#define IS_ARG_EMPTY(I) IS_EMPTY(info[I])


#define CHECK_REQ_ARG(I, C, T)                                                \
	if (info.Length() <= (I) || ! info[I].C) {                                \
		JS_THROW("Argument " #I " must be of type `" T "`");                  \
		RET_UNDEFINED;                                                        \
	}

#define CHECK_LET_ARG(I, C, T)                                                \
	if ( ! (IS_ARG_EMPTY(I) || info[I].C) ) {                                 \
		JS_THROW(                                                             \
			"Argument " #I                                                    \
			" must be of type `" T                                            \
			"` or be `null`/`undefined`"                                      \
		);                                                                    \
		RET_UNDEFINED;                                                        \
	}


#define REQ_STR_ARG(I, VAR)                                                   \
	CHECK_REQ_ARG(I, IsString(), "String");                                   \
	std::string VAR = info[I].ToString().Utf8Value();

#define USE_STR_ARG(I, VAR, DEF)                                              \
	CHECK_LET_ARG(I, IsString(), "String");                                   \
	std::string VAR = IS_ARG_EMPTY(I) ? (DEF) : info[I].ToString().Utf8Value();

#define LET_STR_ARG(I, VAR) USE_STR_ARG(I, VAR, "")


#define REQ_INT32_ARG(I, VAR)                                                 \
	CHECK_REQ_ARG(I, IsNumber(), "Int32");                                    \
	int VAR = info[I].ToNumber().Int32Value();

#define USE_INT32_ARG(I, VAR, DEF)                                            \
	CHECK_LET_ARG(I, IsNumber(), "Int32");                                    \
	int VAR = IS_ARG_EMPTY(I) ? (DEF) : info[I].ToNumber().Int32Value();

#define LET_INT32_ARG(I, VAR) USE_INT32_ARG(I, VAR, 0)

#define REQ_INT_ARG(I, VAR) REQ_INT32_ARG(I, VAR)
#define USE_INT_ARG(I, VAR, DEF) USE_INT32_ARG(I, VAR, DEF)
#define LET_INT_ARG(I, VAR) LET_INT32_ARG(I, VAR)


#define REQ_UINT32_ARG(I, VAR)                                                \
	CHECK_REQ_ARG(I, IsNumber(), "Uint32");                                   \
	unsigned int VAR = info[I].ToNumber().Uint32Value();

#define USE_UINT32_ARG(I, VAR, DEF)                                           \
	CHECK_LET_ARG(I, IsNumber(), "Uint32");                                   \
	unsigned int VAR = IS_ARG_EMPTY(I)                                        \
		? (DEF)                                                               \
		: info[I].ToNumber().Uint32Value();

#define LET_UINT32_ARG(I, VAR) USE_UINT32_ARG(I, VAR, 0)

#define REQ_UINT_ARG(I, VAR) REQ_UINT_ARG(I, VAR)
#define USE_UINT_ARG(I, VAR, DEF) USE_UINT32_ARG(I, VAR, DEF)
#define LET_UINT_ARG(I, VAR) LET_UINT32_ARG(I, VAR)


#define REQ_BOOL_ARG(I, VAR)                                                  \
	CHECK_REQ_ARG(I, IsBoolean(), "Bool");                                    \
	bool VAR = info[I].ToBoolean().Value();

#define USE_BOOL_ARG(I, VAR, DEF)                                             \
	CHECK_LET_ARG(I, IsBoolean(), "Bool");                                    \
	bool VAR = IS_ARG_EMPTY(I) ? (DEF) : info[I].ToBoolean().Value();

#define LET_BOOL_ARG(I, VAR) USE_BOOL_ARG(I, VAR, false)


#define REQ_OFFS_ARG(I, VAR)                                                  \
	CHECK_REQ_ARG(I, IsNumber(), "Number");                                   \
	size_t VAR = static_cast<size_t>(info[I].ToNumber().DoubleValue());

#define USE_OFFS_ARG(I, VAR, DEF)                                             \
	CHECK_LET_ARG(I, IsNumber(), "Number");                                   \
	size_t VAR = IS_ARG_EMPTY(I)                                              \
	? (DEF)                                                                   \
	: static_cast<size_t>(info[I].ToNumber().DoubleValue());

#define LET_OFFS_ARG(I, VAR) USE_OFFS_ARG(I, VAR, 0)


#define REQ_DOUBLE_ARG(I, VAR)                                                \
	CHECK_REQ_ARG(I, IsNumber(), "Number");                                   \
	double VAR = info[I].ToNumber().DoubleValue();

#define USE_DOUBLE_ARG(I, VAR, DEF)                                           \
	CHECK_LET_ARG(I, IsNumber(), "Number");                                   \
	double VAR = IS_ARG_EMPTY(I) ? (DEF) : info[I].ToNumber().DoubleValue();

#define LET_DOUBLE_ARG(I, VAR) USE_DOUBLE_ARG(I, VAR, 0.0)


#define REQ_FLOAT_ARG(I, VAR)                                                 \
	CHECK_REQ_ARG(I, IsNumber(), "Number");                                   \
	float VAR = info[I].ToNumber().FloatValue();

#define USE_FLOAT_ARG(I, VAR, DEF)                                            \
	CHECK_LET_ARG(I, IsNumber(), "Number");                                   \
	float VAR = IS_ARG_EMPTY(I) ? (DEF) : info[I].ToNumber().FloatValue();

#define LET_FLOAT_ARG(I, VAR) USE_FLOAT_ARG(I, VAR, 0.f)


#define REQ_EXT_ARG(I, VAR)                                                   \
	CHECK_REQ_ARG(I, IsExternal(), "Pointer");                                \
	Napi::External<void> VAR = info[I].As< Napi::External<void> >();

#define USE_EXT_ARG(I, VAR, DEF)                                              \
	CHECK_LET_ARG(I, IsExternal(), "Pointer");                                \
	Napi::External<void> VAR = IS_ARG_EMPTY(I)                                \
		? (DEF)                                                               \
		: info[I].As< Napi::External<void> >();

#define LET_EXT_ARG(I, VAR) USE_EXT_ARG(I, VAR, JS_EXT(nullptr))


#define REQ_FUN_ARG(I, VAR)                                                   \
	CHECK_REQ_ARG(I, IsFunction(), "Function");                               \
	Napi::Function VAR = info[I].As<Napi::Function>();


#define REQ_OBJ_ARG(I, VAR)                                                   \
	CHECK_REQ_ARG(I, IsObject(), "Object");                                   \
	Napi::Object VAR = info[I].As<Napi::Object>();

#define USE_OBJ_ARG(I, VAR, DEF)                                              \
	CHECK_LET_ARG(I, IsObject(), "Object");                                   \
	Napi::Object VAR = IS_ARG_EMPTY(I) ? (DEF) : info[I].As<Napi::Object>();

#define LET_OBJ_ARG(I, VAR) USE_OBJ_ARG(I, VAR, info[I].As<Napi::Object>())


#define REQ_ARRV_ARG(I, VAR)                                                  \
	CHECK_REQ_ARG(I, IsArrayBuffer(), "Object");                              \
	Napi::ArrayBuffer VAR = info[I].As<Napi::ArrayBuffer>();


#define REQ_BUF_ARG(I, VAR)                                                   \
	CHECK_REQ_ARG(I, IsBuffer(), "Buffer");                                   \
	Napi::Buffer<uint8_t> VAR = info[I].As< Napi::Buffer<uint8_t> >();


#define REQ_ARRAY_ARG(I, VAR)                                                 \
	REQ_OBJ_ARG(I, _obj_##VAR);                                               \
	if ( ! _obj_##VAR.IsArray() ) {                                           \
		JS_THROW("Argument " #I " must  be of type `Array`");                 \
		RET_UNDEFINED;                                                        \
	}                                                                         \
	Napi::Array VAR = _obj_##VAR.As<Napi::Array>();


#define REQ_TYPED_ARRAY_ARG(I, VAR)                                           \
	REQ_OBJ_ARG(I, _obj_##VAR);                                               \
	if ( ! _obj_##VAR.IsTypedArray() ) {                                      \
		JS_THROW("Argument " #I " must be of type `TypedArray`");             \
		RET_UNDEFINED;                                                        \
	}                                                                         \
	Napi::TypedArray VAR = _obj_##VAR.As<Napi::TypedArray>();


#define DES_CHECK                                                             \
	if (_isDestroyed) return;

#define THIS_CHECK                                                            \
	NAPI_ENV;                                                                 \
	if (_isDestroyed) RET_UNDEFINED;

#define CACHE_CAS(CACHE, V)                                                   \
	if (CACHE == V) {                                                         \
		RET_UNDEFINED;                                                        \
	}                                                                         \
	CACHE = V;

#define SETTER_CHECK(C, T)                                                    \
	if ( ! value.C ) {                                                        \
		JS_THROW("Value must be " T);                                         \
		RET_UNDEFINED;                                                        \
	}


#define JS_METHOD(NAME) Napi::Value NAME(const Napi::CallbackInfo &info)

#define ACCESSOR_RW(CLASS, NAME)                                              \
	InstanceAccessor(#NAME, &CLASS::NAME ## Getter, &CLASS::NAME ## Setter)

#define ACCESSOR_R(CLASS, NAME)                                               \
	InstanceAccessor(#NAME, &CLASS::NAME ## Getter, nullptr)

#define ACCESSOR_M(CLASS, NAME)                                               \
	InstanceMethod(#NAME, &CLASS::NAME)

#define THIS_OBJ(VAR)                                                         \
	Napi::Object VAR = info.This().As<Napi::Object>();

#define SETTER_STR_ARG                                                        \
	SETTER_CHECK(IsString(), "String");                                       \
	std::string v = value.ToString().Utf8Value();

#define SETTER_INT32_ARG                                                      \
	SETTER_CHECK(IsNumber(), "Int32");                                        \
	int v = value.ToNumber().Int32Value();

#define SETTER_INT_ARG SETTER_INT32_ARG

#define SETTER_BOOL_ARG                                                       \
	SETTER_CHECK(IsBoolean(), "Bool");                                        \
	bool v = value.ToBoolean().Value();

#define SETTER_UINT32_ARG                                                     \
	SETTER_CHECK(IsNumber(), "Uint32");                                       \
	unsigned int v = value.ToNumber().Uint32Value();

#define SETTER_UINT_ARG SETTER_UINT32_ARG

#define SETTER_OFFS_ARG                                                       \
	SETTER_CHECK(IsNumber(), "Number");                                       \
	size_t v = static_cast<size_t>(value.ToNumber().DoubleValue());

#define SETTER_DOUBLE_ARG                                                     \
	SETTER_CHECK(IsNumber(), "Number");                                       \
	double v = value.ToNumber().DoubleValue();

#define SETTER_FLOAT_ARG                                                      \
	SETTER_CHECK(IsNumber(), "Number");                                       \
	float v = value.ToNumber().FloatValue();

#define SETTER_EXT_ARG                                                        \
	SETTER_CHECK(IsExternal(), "Pointer");                                    \
	Napi::External v = value.As<Napi::External>();

#define SETTER_FUN_ARG                                                        \
	SETTER_CHECK(IsFunction(), "Function");                                   \
	Napi::Function v = value.As<Napi::Function>()

#define SETTER_OBJ_ARG                                                        \
	SETTER_CHECK(IsObject(), "Object");                                       \
	Napi::Object v = value.As<Napi::Object>()

#define SETTER_ARRV_ARG                                                       \
	SETTER_CHECK(IsArrayBuffer(), "TypedArray");                              \
	Napi::ArrayBuffer v = value.As<Napi::ArrayBuffer>();



template<typename Type = uint8_t>
inline Type* getArrayData(
	Napi::Env env,
	Napi::Object obj,
	int *num = nullptr,
	int srcOffset = 0
) {
	
	Type *data = nullptr;
	
	if (obj.IsTypedArray()) {
		Napi::TypedArray ta = obj.As<Napi::TypedArray>();
		size_t offset = ta.ByteOffset();
		Napi::ArrayBuffer arr = ta.ArrayBuffer();
		if (num) {
			*num = ta.ByteLength() / sizeof(Type);
		}

		uint8_t *base = reinterpret_cast<uint8_t *>(arr.Data());
		data = reinterpret_cast<Type *>(base  + offset + ( sizeof(Type) * srcOffset ));

	} else if (obj.IsArrayBuffer()) {
		Napi::ArrayBuffer arr = obj.As<Napi::ArrayBuffer>();
		if (num) {
			*num = arr.ByteLength() / sizeof(Type);
		}
		data = reinterpret_cast<Type *>(arr.Data() + ( sizeof(Type) * srcOffset ));
	} else {
		if (num) {
			*num = 0;
		}
		JS_THROW("Argument must be of type `TypedArray`.");
	}
	
	return data;
	
}

template<typename Type = uint8_t>
inline Type* getBufferData(
	Napi::Env env,
	Napi::Object obj,
	int *num = nullptr,
	int offset = 0
) {
	
	Type *data = nullptr;
	
	if (num) {
		*num = 0;
	}
	
	if ( ! obj.IsBuffer() ) {
		JS_THROW("Argument must be of type `Buffer`.");
		return data;
	}
	
	Napi::Buffer<uint8_t> arr = obj.As< Napi::Buffer<uint8_t> >();
	if (num) {
		*num = arr.Length() / sizeof(Type);
	}
<<<<<<< HEAD
	data = arr.Data() + sizeof(Type) * offset;
=======
	data = arr.Data() + offset;
>>>>>>> origin/master
	
	return data;
	
}


inline void *getData(Napi::Env env, Napi::Object obj, int offset = 0) {
	
	void *pixels = nullptr;
	
	if (obj.IsTypedArray() || obj.IsArrayBuffer()) {
		pixels = getArrayData<uint8_t>(env, obj, nullptr, offset);
	} else if (obj.IsBuffer()) {
		pixels = getBufferData<uint8_t>(env, obj, nullptr, offset);
	} else if (obj.Has("data")) {
		Napi::Object data = obj.Get("data").As<Napi::Object>();
		if (data.IsTypedArray() || data.IsArrayBuffer()) {
			pixels = getArrayData<uint8_t>(env, data, nullptr, offset);
		} else if (data.IsBuffer()) {
			pixels = getBufferData<uint8_t>(env, data, nullptr, offset);
		}
	}
	
	return pixels;
}