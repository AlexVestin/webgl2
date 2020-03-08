# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := webgl
DEFS_Debug := \
	'-DNODE_GYP_MODULE_NAME=webgl' \
	'-DUSING_UV_SHARED=1' \
	'-DUSING_V8_SHARED=1' \
	'-DV8_DEPRECATION_WARNINGS=1' \
	'-DV8_DEPRECATION_WARNINGS' \
	'-DV8_IMMINENT_DEPRECATION_WARNINGS' \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-D__STDC_FORMAT_MACROS' \
	'-DNAPI_DISABLE_CPP_EXCEPTIONS' \
	'-DBUILDING_NODE_EXTENSION' \
	'-DDEBUG' \
	'-D_DEBUG' \
	'-DV8_ENABLE_CHECKS'

# Flags passed to all source files.
CFLAGS_Debug := \
	-fPIC \
	-pthread \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-m64 \
	-g \
	-O0

# Flags passed to only C files.
CFLAGS_C_Debug :=

# Flags passed to only C++ files.
CFLAGS_CC_Debug := \
	-fno-rtti \
	-std=gnu++1y

INCS_Debug := \
	-I/home/alex/.cache/node-gyp/12.16.1/include/node \
	-I/home/alex/.cache/node-gyp/12.16.1/src \
	-I/home/alex/.cache/node-gyp/12.16.1/deps/openssl/config \
	-I/home/alex/.cache/node-gyp/12.16.1/deps/openssl/openssl/include \
	-I/home/alex/.cache/node-gyp/12.16.1/deps/uv/include \
	-I/home/alex/.cache/node-gyp/12.16.1/deps/zlib \
	-I/home/alex/.cache/node-gyp/12.16.1/deps/v8/include \
	-I/home/alex/prog/webgl/node_modules/node-addon-api \
	-I/opt/cuda/targets/x86_64-linux/include \
	-I/opt/angle

DEFS_Release := \
	'-DNODE_GYP_MODULE_NAME=webgl' \
	'-DUSING_UV_SHARED=1' \
	'-DUSING_V8_SHARED=1' \
	'-DV8_DEPRECATION_WARNINGS=1' \
	'-DV8_DEPRECATION_WARNINGS' \
	'-DV8_IMMINENT_DEPRECATION_WARNINGS' \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-D__STDC_FORMAT_MACROS' \
	'-DNAPI_DISABLE_CPP_EXCEPTIONS' \
	'-DBUILDING_NODE_EXTENSION'

# Flags passed to all source files.
CFLAGS_Release := \
	-fPIC \
	-pthread \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-m64 \
	-O3 \
	-fno-omit-frame-pointer

# Flags passed to only C files.
CFLAGS_C_Release :=

# Flags passed to only C++ files.
CFLAGS_CC_Release := \
	-fno-rtti \
	-std=gnu++1y

INCS_Release := \
	-I/home/alex/.cache/node-gyp/12.16.1/include/node \
	-I/home/alex/.cache/node-gyp/12.16.1/src \
	-I/home/alex/.cache/node-gyp/12.16.1/deps/openssl/config \
	-I/home/alex/.cache/node-gyp/12.16.1/deps/openssl/openssl/include \
	-I/home/alex/.cache/node-gyp/12.16.1/deps/uv/include \
	-I/home/alex/.cache/node-gyp/12.16.1/deps/zlib \
	-I/home/alex/.cache/node-gyp/12.16.1/deps/v8/include \
	-I/home/alex/prog/webgl/node_modules/node-addon-api \
	-I/opt/cuda/targets/x86_64-linux/include \
	-I/opt/angle

OBJS := \
	$(obj).target/$(TARGET)/src/native/bindings.o \
	$(obj).target/$(TARGET)/src/native/nvenc.o \
	$(obj).target/$(TARGET)/src/native/webgl.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# CFLAGS et al overrides must be target-local.
# See "Target-specific Variable Values" in the GNU Make manual.
$(OBJS): TOOLSET := $(TOOLSET)
$(OBJS): GYP_CFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE))
$(OBJS): GYP_CXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE))

# Suffix rules, putting all outputs into $(obj).

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# Try building from generated source, too.

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# End of this set of suffix rules
### Rules for final target.
LDFLAGS_Debug := \
	-pthread \
	-rdynamic \
	-m64 \
	-L/opt/angle/angle/out/Release

LDFLAGS_Release := \
	-pthread \
	-rdynamic \
	-m64 \
	-L/opt/angle/angle/out/Release

LIBS := \
	-lEGL \
	-lGLESv2 \
	-lcuda \
	-lavformat \
	-lavutil \
	-lavcodec \
	-lswscale \
	-ljpeg

$(obj).target/webgl.node: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(obj).target/webgl.node: LIBS := $(LIBS)
$(obj).target/webgl.node: TOOLSET := $(TOOLSET)
$(obj).target/webgl.node: $(OBJS) FORCE_DO_CMD
	$(call do_cmd,solink_module)

all_deps += $(obj).target/webgl.node
# Add target alias
.PHONY: webgl
webgl: $(builddir)/webgl.node

# Copy this to the executable output path.
$(builddir)/webgl.node: TOOLSET := $(TOOLSET)
$(builddir)/webgl.node: $(obj).target/webgl.node FORCE_DO_CMD
	$(call do_cmd,copy)

all_deps += $(builddir)/webgl.node
# Short alias for building this executable.
.PHONY: webgl.node
webgl.node: $(obj).target/webgl.node $(builddir)/webgl.node

# Add executable to "all" target.
.PHONY: all
all: $(builddir)/webgl.node

