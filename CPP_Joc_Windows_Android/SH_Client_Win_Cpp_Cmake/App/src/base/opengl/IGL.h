#pragma once

#include <base/gh.h>
#include <stddef.h>
#include <functional>
#include <base/opengl/util/IManagedGlResource.h>
#include <base/object/ManagedListener.h>
#include <graphics/visual/Color.h>

typedef void GLvoid;
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;

typedef unsigned char GLboolean;
typedef unsigned char GLubyte;
typedef char GLchar;
typedef int GLint;
typedef unsigned int GLuint;
typedef float GLfloat;

typedef int GLsizei;
typedef double GLclampd;

typedef signed   char          gl_int8_t;
typedef unsigned char          gl_uint8_t;
typedef signed   short int     gl_int16_t;
typedef unsigned short int     gl_uint16_t;
typedef signed   long  int     gl_intptr_t;
typedef unsigned long  int     gl_uintptr_t;
typedef signed   long  int     gl_ssize_t;
typedef unsigned long  int     gl_usize_t;


#if defined(_WIN32) || defined(_WIN64)
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
#else
typedef gl_intptr_t GLintptr;
typedef gl_ssize_t GLsizeiptr;
#endif

class IGL {
    pub class IListener : virtual public base::IManagedListener {
        pub virtual void onGlContextCreated() = 0;
        pub virtual void onGlContextDestroyedPre() = 0;

        pub ~IListener() override = default;
    };
    pub class CbListener : public base::ManagedListener, virtual public IListener {pub dCtor(CbListener);priv typedef base::ManagedListener super;
        pub std::function<void()> cb_onGlContextCreated = nullptr;
        pub std::function<void()> cb_onGlContextDestroyedPre = nullptr;

        pub explicit CbListener();

        pub void onGlContextCreated() override;
        pub void onGlContextDestroyedPre() override;

        pub ~CbListener() override;
    };

    pub class GlobalsCumulativeProfiler {pub dCtor(GlobalsCumulativeProfiler);
        pub int vram_bytes_cumulative = 0;

        pub int shaderProgramsCount_cumulative = 0;
        pub int buffersCount_cumulative = 0;
        pub int texturesCount_cumulative = 0;

        pub int createCalls_shaderPrograms = 0;
        pub int createCalls_buffers = 0;
        pub int createCalls_textures = 0;

        pub explicit GlobalsCumulativeProfiler() {};

        pub virtual ~GlobalsCumulativeProfiler() {};
    };
    pub class GlobalsProfiler {pub dCtor(GlobalsProfiler);
        pub int bufferUploadCount = 0;
        pub int textureUploadCount = 0;

        pub int bufferUploadBytes = 0;
        pub int textureUploadBytes = 0;

        pub int shaderUniformSetCallsCount = 0;
        pub int shaderUniformSetBytesCount = 0;

        pub explicit GlobalsProfiler() {};

        pub void clear() {
            bufferUploadCount = 0;
            textureUploadCount = 0;
            
            bufferUploadBytes = 0;
            textureUploadBytes = 0;

            shaderUniformSetCallsCount = 0;
            shaderUniformSetBytesCount = 0;

            // Cumulative values are not reset.
            //..._cumulative = 0;
	    }

        pub virtual ~GlobalsProfiler() {};
    };
    pub class LocalsProfiler {pub dCtor(LocalsProfiler);
        pub int bindBuffer = 0;
        pub int bindTexture = 0;

        pub int useProgram = 0;

        pub int drawElements = 0;
        pub int drawElementsCount = 0;
        pub int drawTrianglesCount = 0;
        pub int drawLinesCount = 0;

        pub explicit LocalsProfiler() {};

        pub void clear() {
		    bindBuffer = 0;
		    bindTexture = 0;

		    useProgram = 0;

		    drawElements = 0;
            drawTrianglesCount = 0;
            drawLinesCount = 0;

		    drawElementsCount = 0;
	    }

        pub virtual ~LocalsProfiler() {};
    };
    pub class Capabilities {pub dCtor(Capabilities);
        pub int majorVersion = -1;
        pub int minorVersion = -1;
        pub std::string version = "";

		// https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glGet.xhtml
		/// GL_MAX_UNIFORM_BLOCK_SIZE params returns one value, the maximum size in basic machine units of a uniform block. The value must be at least 16384. See glUniformBlockBinding.
        // The total amount of buffer object storage available for any given uniform block.
        // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_uniform_buffer_object.txt
        // On my old GPU this was 35376.
        // On my new GPU this is 65536.
        // glBindBufferRange can be used to combine multiple uniform blocks into a single buffer.
        /// The combining blocks must be aligned to uniformBufferOffsetAlignment multiples.
        // NOTE: Should also use this for UBO because the shader can access only this region. The UBO can be larger and use the region binding.
        // NOTE: TESTED: This is for each INDIVIDUAL block that is in the shader.
        pub int maxUniformBlockSizeBytes = -1;
        // On my low_end pc, this is 256.
        // This is used if multiple uniform blocks are combined into a single buffer and glBindBufferRange is used.
        pub int uniformBufferOffsetAlignment = -1;
        pub int maxVertexUniformBlocks = -1;
        pub int maxFragmentUniformBlocks = -1;
        pub int maxCombinedUniformBlocks = -1;
        // On my new GPU this is 4096.
        pub int maxVertexUniformComponents = -1;
        // This is the max number of vectors allowed as uniforms in a shader (mat3 has 3 vectors and mat4 has 4 vectors, deduced from tests).
        // https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glGet.xhtml
		/// GL_MAX_VERTEX_UNIFORM_VECTORS params returns one value, the maximum number of vector floating-point, integer, or boolean values that can be held in uniform variable storage for a vertex shader. The value must be at least 256. See glUniform.
		/// No sure if this is per uniform or for all uniforms combined.
        // On my new GPU this is 1024.
        // NOTE: TESTED: For UBO uniforms, this doesn't seem to be used (1024 mat4 were allocated in a shader but 1025 threw an error. That error probably was because of maxUniformBlockSizeBytes limit exceeded because the 1024 success mat4 array had vec4 4096 elements that this variable uses). Maybe this is used only for uniform_blocks without UBO binding.
        pub int maxVertexUniformVectors = -1;
        pub int maxFragmentUniformVectors = -1;
        pub int maxCombinedVertexUniformComponents = -1;
        pub int maxCombinedFragmentUniformComponents = -1;

        // This is the max number of modelTransform + modelNormalTransform that should be used in Unfirom blocks (that are uplaoded each frame to the shader).
        // This is not used for UBO.
        pub int maxShaderUniformBlockModelAndNormalTransformsAvailable = -1;

        pub int maxTextureImageUnits = -1;
        pub int maxTextureSize = -1;
        pub int maxVertexTextureImageUnits = -1;
        pub int maxCombinedTextureImageUnits = -1;

        // This may be just a recommended value, larger values may be used (need testing) but the value seems large enough (>100k on PC) and should be used as limits.
        pub int maxElementVertices = -1;
        // This may be just a recommended value, larger values may be used (need testing) but the value seems large enough (>100k on PC) and should be used as limits.
        pub int maxElementIndices = -1;

        pub explicit Capabilities() {};

        pub virtual ~Capabilities() {};
    };

    // Note. These are initialised by the upper class.
    pub static const GLenum BYTE;
    pub static const GLenum UNSIGNED_BYTE;
    pub static const GLenum SHORT;
    pub static const GLenum UNSIGNED_SHORT;
    pub static const GLenum INT;
    pub static const GLenum UNSIGNED_INT;
    pub static const GLenum FIXED;
    pub static const GLenum FLOAT;
    pub static const GLenum UNSIGNED_INT_24_8;
    pub static const GLenum FLOAT_32_UNSIGNED_INT_24_8_REV;

    pub static const GLenum INVALID_INDEX;

    // Primitives : https://www.khronos.org/opengl/wiki/Primitive
    /**
    * This will cause OpenGL to interpret each individual vertex in the stream as a point. Points that have a Texture mapped onto them are often called "point sprites".
    * Points are rasterized as screen-aligned squares of a given window-space size. The size can be given in two methods: by the last active vertex processing shader stage or by the context's state. To set the point size from a shader, enable the glEnable with argument (GL_PROGRAM_POINT_SIZE) to set the point size from the program. If GL_PROGRAM_POINT_SIZE is enabled, then the point size comes from the output variable float gl_PointSize. If it is disabled, the point size is constant for all points in a primitive, and is set by the glPointSize function.
    * The size defines the number of window pixels that each side of the point's square takes up. The point's position defines the center of that square.
    * Regardless of how the point size is defined, it must be greater than 0, or else undefined behavior results. There is an implementation-defined range for point sizes, and the size given by either method is clamped to that range. You can query the range with GL_POINT_SIZE_RANGE (returns 2 floats). There is also a point granularity that you can query with GL_POINT_SIZE_GRANULARITY; the implementation will clamp sizes to the nearest multiple of the granularity.
    * 
    * Point fragment inputs
    * Points are defined by only a single vertex. Therefore, every Fragment generated by rasterizing that point is given the same user-defined input values. However, it is useful to know exactly where within the point a particular Fragment Shader invocation is. While the specific position of the fragment, gl_FragCoord will change, per the fragment's location, it would be more useful to know where one is relative to the point primitive itself.
    * To assist in this, the fragment shader can use the built-in input vec2 gl_PointCoord. This gives coordinates on the range [0, 1]. The location of (0, 0) depends on the point parameter setting for GL_POINT_SPRITE_COORD_ORIGIN: if it is set to GL_LOWER_LEFT, then (0, 0) is the bottom-left coordinate. While if it is GL_UPPER_LEFT, then (0, 0) is the top-left coordinate. The default is GL_LOWER_LEFT, which fits with OpenGL's usual right-handed coordinate systems.
    **/
    pub static const GLenum POINTS;
    
    /**
    * Vertices 0 and 1 are considered a line. Vertices 2 and 3 are considered a line. And so on. If the user specifies a non-even number of vertices, then the extra vertex is ignored.
    **/
    pub static const GLenum LINES;
    /**
    * The adjacent vertices are considered lines. Thus, if you pass n vertices, you will get n-1 lines. If the user only specifies 1 vertex, the drawing command is ignored.
    **/
    pub static const GLenum LINE_STRIP;
    /**
    * As line strips, except that the first and last vertices are also used as a line. Thus, you get n lines for n input vertices. If the user only specifies 1 vertex, the drawing command is ignored. The line between the first and last vertices happens after all of the previous lines in the sequence.
    **/
    pub static const GLenum LINE_LOOP;
    
    // Any incomplete primitives will be ignored. For example, using GL_TRIANGLES with a number of vertices not divisible by 3 will ignore the incomplete primitive at the end. Rendering with less than 3 vertices will result in nothing rendering.
    /**
    * Vertices 0, 1, and 2 form a triangle. Vertices 3, 4, and 5 form a triangle. And so on.
    **/
    pub static const GLenum TRIANGLES;
    /**
    * Every group of 3 adjacent vertices forms a triangle. The face direction of the strip is determined by the winding of the first triangle. Each successive triangle will have its effective face order reversed, so the system compensates for that by testing it in the opposite way. A vertex stream of n length will generate n-2 triangles.
    **/
    pub static const GLenum TRIANGLE_STRIP;
    /**
    * The first vertex is always held fixed. From there on, every group of 2 adjacent vertices form a triangle with the first. So with a vertex stream, you get a list of triangles like so: (0, 1, 2) (0, 2, 3), (0, 3, 4), etc. A vertex stream of n length will generate n-2 triangles.
    **/
    pub static const GLenum TRIANGLE_FAN;

    pub static const GLuint VERTEX_SHADER;
    pub static const GLuint FRAGMENT_SHADER;

    pub static const GLenum COMPILE_STATUS;
    pub static const GLenum INFO_LOG_LENGTH;

    pub static const GLenum LINK_STATUS;
    pub static const GLenum VALIDATE_STATUS;

    pub static const GLenum TEXTURE_2D;
    pub static const GLenum TEXTURE_2D_ARRAY;
    pub static const GLenum TEXTURE_MIN_FILTER;
    pub static const GLenum TEXTURE_MAG_FILTER;
    pub static const GLenum TEXTURE_WRAP_S;
    pub static const GLenum TEXTURE_WRAP_T;
    // Not available on OpenGL ES 3.0 (the used version), only in OpenGL ES 3.2 which is not used on Android (Vulkan is used in new devices instead).
    pub static const GLenum TEXTURE_BORDER_COLOR;
    // This is textre format.
    pub static const GLenum DEPTH_COMPONENT;

    // These are signed internal formats.
    pub static const GLenum DEPTH_COMPONENT16;
    pub static const GLenum DEPTH_COMPONENT24;
    pub static const GLenum DEPTH_COMPONENT32F;

    pub static const GLenum CLAMP_TO_EDGE;
	// Not available on OpenGL ES 3.0 (the used version), only in OpenGL ES 3.2 which is not used on Android (Vulkan is used in new devices instead).
	pub static const GLenum CLAMP_TO_BORDER;
    pub static const GLenum MIRRORED_REPEAT;
    pub static const GLenum REPEAT;

    pub static const GLenum NEAREST;
    pub static const GLenum LINEAR;
    pub static const GLenum NEAREST_MIPMAP_NEAREST;
    pub static const GLenum LINEAR_MIPMAP_NEAREST;
    pub static const GLenum NEAREST_MIPMAP_LINEAR;
    pub static const GLenum LINEAR_MIPMAP_LINEAR;

    pub static const GLenum RGBA8;
    pub static const GLenum RGBA;

    pub static const GLenum TEXTURE0;

    pub static const GLbitfield COLOR_BUFFER_BIT;

    pub static const GLenum NO_ERROR;

    pub static const GLenum INVALID_ENUM;
    pub static const GLenum INVALID_VALUE;
    pub static const GLenum INVALID_OPERATION;
    pub static const GLenum OUT_OF_MEMORY;
    pub static const GLenum INVALID_FRAMEBUFFER_OPERATION;

    pub static const GLenum MAX_TEXTURE_IMAGE_UNITS;
    pub static const GLenum MAX_TEXTURE_SIZE;
    pub static const GLenum MAX_VERTEX_TEXTURE_IMAGE_UNITS;
    pub static const GLenum MAX_COMBINED_TEXTURE_IMAGE_UNITS;

    pub static const GLenum MAX_ELEMENTS_VERTICES;
    pub static const GLenum MAX_ELEMENTS_INDICES;

    pub static const GLenum BLEND;
    pub static const GLenum SRC_ALPHA;
    pub static const GLenum ONE_MINUS_SRC_ALPHA;

    pub static const GLenum CW;
    pub static const GLenum CCW;

    pub static const GLenum BACK;
    pub static const GLenum FRONT;
    pub static const GLenum CULL_FACE;
    pub static const GLenum DEPTH_TEST;
    pub static const GLenum SCISSOR_TEST;
    pub static const GLenum LESS;
    pub static const GLenum DEPTH_BUFFER_BIT;

    pub static const GLenum NONE;
    pub static const GLenum ONE;
    pub static const GLenum ZERO;

    pub static const GLenum FRAMEBUFFER;
    pub static const GLenum ARRAY_BUFFER;
    pub static const GLenum ELEMENT_ARRAY_BUFFER;
    pub static const GLenum UNIFORM_BUFFER;
    pub static const GLenum STATIC_DRAW;
    pub static const GLenum DYNAMIC_DRAW;

    pub static const GLenum DEPTH_ATTACHMENT;

    pub static const GLenum VERSION;
    pub static const GLenum MAJOR_VERSION;
    pub static const GLenum MINOR_VERSION;

    pub static const GLenum UNIFORM_BLOCK_DATA_SIZE;
    // The total amount of buffer object storage available for any given uniform block.
    // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_uniform_buffer_object.txt
    pub static const GLenum MAX_UNIFORM_BLOCK_SIZE;
    pub static const GLenum UNIFORM_BUFFER_OFFSET_ALIGNMENT;
    pub static const GLenum MAX_VERTEX_UNIFORM_BLOCKS;
    pub static const GLenum MAX_FRAGMENT_UNIFORM_BLOCKS;
    pub static const GLenum MAX_COMBINED_UNIFORM_BLOCKS;
    pub static const GLenum MAX_VERTEX_UNIFORM_COMPONENTS;
    pub static const GLenum MAX_VERTEX_UNIFORM_VECTORS;
    pub static const GLenum MAX_FRAGMENT_UNIFORM_VECTORS;
    pub static const GLenum MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS;
    pub static const GLenum MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS;

    pub static const GLboolean TRUE;
    pub static const GLboolean FALSE;

	pub virtual bool isProfilerEnabled() = 0;
	pub virtual void enableProfiler() = 0;
	pub virtual void disableProfiler() = 0;
	pub virtual GlobalsCumulativeProfiler* getGlobalsCumulativeProfiler() = 0;
	pub virtual GlobalsProfiler* getGlobalsProfiler() = 0;
	pub virtual LocalsProfiler* getProfiler2D() = 0;
	pub virtual LocalsProfiler* getProfiler3D() = 0;
	pub virtual LocalsProfiler* getProfilerSpecial() = 0;
	pub virtual void setActiveProfilerSpecial() = 0;
	pub virtual void setActiveProfiler2D() = 0;
	pub virtual void setActiveProfiler3D() = 0;

    pub virtual Capabilities& getCapabilities() = 0;

	pub virtual int getGlContextIndex() = 0;

	pub virtual bool getIsGlContext() = 0;

	pub virtual void addGlContextListener(IListener* listener) = 0;
	pub virtual void removeGlContextListener(IListener* listener) = 0;

	pub virtual void addManagedGlResource(IManagedGlResource* managedGlResource) = 0;
	pub virtual void removeManagedGlResource(IManagedGlResource* managedGlResource) = 0;

    pub static unsigned int sizeOfType(GLuint type);

	pub virtual void genTextures(GLsizei n, GLuint *textures) = 0;
    // glActiveTexture selects which texture unit subsequent texture state calls will affect.The number of texture units an implementation supports is implementation - dependent, but must be at least 32.
	pub virtual void activeTexture(GLenum texture) = 0;
    // glBindTexture binds the texture object with name texture to the texture target specified by target. Calling glBindTexture with target set to GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY, or GL_TEXTURE_CUBE_MAP and texture set to the name of the new texture binds the texture name to that target.When a texture is bound to a target, the previous binding for that target is automatically broken.
	pub virtual void bindTexture(GLenum target, GLuint texture) = 0;
	pub virtual void deleteTextures(GLsizei n, const GLuint * textures, unsigned int loggedSizeBytes) = 0;
	pub virtual void texParameteri(GLenum target, GLenum pname, GLint param) = 0;
	pub virtual void texParameterfv(GLenum target, GLenum pname, const GLfloat* param) = 0;

    pub virtual unsigned int texStorage2D(GLenum target, GLsizei levels, GLenum sizedInternalFormat, GLsizei width, GLsizei height) = 0;
    pub virtual unsigned int texStorage3D(
        GLenum target, GLsizei levels, GLenum internalformat,
        GLsizei width, GLsizei height, GLsizei depth) = 0;

    pub virtual unsigned int texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) = 0;
    pub virtual void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data) = 0;
    pub virtual void texSubImage3D(
        GLenum target,
        GLint level,
        GLint xoffset, GLint yoffset, GLint zoffset,
        GLsizei width, GLsizei height, GLsizei depth,
        GLenum format, GLenum type,
        const void* data) = 0;

	pub virtual void enableVertexAttribArray(GLuint index) = 0;
    // NOTE. This will convert any type provided to floats. If integer types are used in the shader, use the "I" variant below.
    // For glVertexAttribPointer, if normalized? is set to GL_TRUE?, it indicates that values stored in an integer format are to be mapped to the range [-1,1] (for signed values) or [0,1] (for unsigned values) when they are accessed and converted to floating point. Otherwise, values will be converted to floats directly without normalization.
	pub virtual void vertexAttribPointer(
        GLuint index,
        GLint size,
        GLenum type,
        GLboolean normalized,
        GLsizei stride,
        const GLvoid * pointer) = 0;
    // NOTE. This will leave the values provided as integer types. Use this if the shader attributes are of integer types.
    // For glVertexAttribIPointer, only the integer types GL_BYTE?, GL_UNSIGNED_BYTE?, GL_SHORT?, GL_UNSIGNED_SHORT?, GL_INT?, GL_UNSIGNED_INT? are accepted. Values are always left as integer values.
    pub virtual void vertexAttribIPointer(
        GLuint index,
        GLint size,
        GLenum type,
        GLsizei stride,
        const GLvoid* pointer) = 0;
    pub virtual void disableVertexAttribArray(GLuint index) = 0;

	pub virtual GLuint createProgram() = 0;
	pub virtual void attachShader(GLuint program, GLuint shader) = 0;
	pub virtual void linkProgram(GLuint program) = 0;
	pub virtual void validateProgram(GLuint program) = 0;
	pub virtual void deleteShader(GLuint shader) = 0;
	pub virtual GLuint createShader(GLenum shaderType) = 0;
	pub virtual void shaderSource(
        GLuint shader,
        GLsizei count,
        const GLchar * const *string,
        const GLint *length) = 0;
	pub virtual void compileShader(GLuint shader) = 0;

	pub virtual void getShaderiv(
        GLuint shader,
        GLenum pname,
        GLint *params) = 0;
	pub virtual void getShaderInfoLog(
        GLuint shader,
        GLsizei maxLength,
        GLsizei *length,
        GLchar *infoLog) = 0;

	pub virtual void getProgramiv(GLuint program, GLenum pname, GLint *params) = 0;
	pub virtual void getProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) = 0;

	pub virtual void useProgram(GLuint program) = 0;
	pub virtual GLint getAttribLocation(
        GLuint program,
        const GLchar *name) = 0;

	pub virtual GLint getUniformLocation(
        GLuint program,
        const GLchar *name) = 0;
    // This is the uniform block index in the provided shader program. It starts at 0 for each shader.
    pub virtual GLuint getUniformBlockIndex(
        GLuint program,
        const GLchar* uniformBlockName) = 0;
    /*program
      The name of a program object containing the active uniform block whose binding to assign.

      uniformBlockIndex
      The index of the active uniform block within program whose binding to assign.

      uniformBlockBinding
      Specifies the binding point to which to bind the uniform block with index uniformBlockIndex within program.*/
    pub virtual void uniformBlockBinding(
        GLuint program,
        GLuint uniformBlockIndex,
        GLuint uniformBlockBinding) = 0;

    // glUniform modifies the value of a uniform variable or a uniform variable array.The location of the uniform variable to be modified is specified by location, which should be a value returned by glGetUniformLocation.glUniform operates on the program object that was made part of current state by calling glUseProgram.
    /// Source: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml
    // location is the value provided by glGetUniformLocation. The location can the a index into an array.
    // For small array initialisations [1, 4], can use another version of this function and get the location of the array name with no "[#]".
	pub virtual void uniform1i(
        GLint location,
        GLint v0) = 0;
    // These can be used to update multiple entries in an array.
    pub virtual void uniform2i(
        GLint location,
        GLint v0, GLint v1) = 0;
	pub virtual void uniform3i(
        GLint location,
        GLint v0, GLint v1, GLint v2) = 0;
	pub virtual void uniform4i(
        GLint location,
        GLint v0, GLint v1, GLint v2, GLint v3) = 0;

	pub virtual void uniform1f(
        GLint location,
        GLfloat v) = 0;
	pub virtual void uniform3f(
        GLint location,
        GLfloat v0,
        GLfloat v1,
        GLfloat v2) = 0;
	pub virtual void uniform4f(
        GLint location,
        GLfloat v0,
        GLfloat v1,
        GLfloat v2,
        GLfloat v3) = 0;
	pub virtual void uniformMatrix3fv(
        GLint location,
        GLsizei count,
        GLboolean transpose,
        const GLfloat *value) = 0;
	pub virtual void uniformMatrix4fv(
        GLint location,
        GLsizei count,
        GLboolean transpose,
        const GLfloat *value) = 0;
	pub virtual void uniform1iv(
        GLint location,
        GLsizei count,
        const GLint *value) = 0;
	pub virtual void deleteProgram(GLuint program) = 0;

	pub virtual void viewport(GLint x, GLint y, GLsizei width, GLsizei height) = 0;
	pub virtual void scissor(GLint x, GLint y, GLsizei width, GLsizei height) = 0;

	pub virtual Color& getCachedClearColor() = 0;
	pub virtual void setCachedClearColor(Color& cachedClearColor) = 0;
	pub virtual void setCachedClearColorDirect(Color cachedClearColor) = 0;
	pub virtual void clearColor(
		GLfloat red,
		GLfloat green,
		GLfloat blue,
		GLfloat alpha) = 0;
	pub virtual void clear(GLbitfield mask) = 0;

	pub virtual GLenum getError() = 0;

	pub virtual void getIntegerv(GLenum pname, GLint * params) = 0;
    pub virtual const GLubyte* getString(GLenum name) = 0;
    pub virtual bool isEnabled(GLenum cap) = 0;
	pub virtual void fromRenderContext_enable(GLenum cap) = 0;
	pub virtual void fromRenderContext_disable(GLenum cap) = 0;

    // Use renderContext to modify.
    pub virtual void fromRenderContext_blendFunc(GLenum sfactor, GLenum dfactor) = 0;

    // Use renderContext to modify.
    pub virtual void fromRenderContext_frontFace(GLenum mode) = 0;
    // Use renderContext to modify.
    pub virtual void fromRenderContext_cullFace(GLenum mode) = 0;
	
    // Use renderContext to modify.
    pub virtual void fromRenderContext_depthFunc(GLenum func) = 0;
    // Use renderContext to modify.
    pub virtual void fromRenderContext_depthRange(GLclampd zNear, GLclampd zFar) = 0;
    // Use renderContext to modify.
    pub virtual void fromRenderContext_depthMask(GLboolean depthMask) = 0;

	pub virtual void genBuffers(GLsizei n, GLuint * buffers) = 0;

    pub virtual void genVertexArrays(
        GLsizei n,
        GLuint* arrays) = 0;
    pub virtual void bindVertexArray(GLuint array) = 0;
    pub virtual void drawArrays(
        GLenum mode,
        GLint first,
        GLsizei count) = 0;

	pub virtual void genFramebuffers(GLsizei n, GLuint* framebuffers) = 0;
	pub virtual void deleteFramebuffers(GLsizei n, GLuint* framebuffers) = 0;
    pub void virtual bindFramebuffer(GLenum target, GLuint framebuffer) = 0;
    pub void virtual framebufferTexture2D(
        GLenum target,
        GLenum attachment,
        GLenum textarget,
        GLuint texture,
        GLint level) = 0;
    pub void virtual drawBuffer(GLenum mode) = 0;
    pub void virtual readBuffer(GLenum src) = 0;

	pub virtual void bindBuffer(GLenum target, GLuint buffer) = 0;
	pub virtual void bindBufferBase(GLenum target, GLuint index, GLuint buffer) = 0;
    // Returns the logged vram bytes used. The value must be saved and sent with the delete call.
	pub virtual unsigned int bufferData(
        GLenum target,
        GLsizeiptr sizeBytes,
        const GLvoid * data,
        GLenum usage) = 0;
	pub virtual void bufferSubData(
        GLenum target,
        GLintptr offset,
        GLsizeiptr sizeBytes,
        const GLvoid * data) = 0;
	pub virtual void deleteBuffers(GLsizei n, const GLuint * buffers, unsigned int loggedSizeBytes) = 0;
    // Indices is a byte offset in the IBO for the start location.
    // Use "(void*)(indexStart * sizeof(ibo_entry_type))" as the indices param
	pub virtual void drawElements(
        GLenum mode,
        GLsizei count,
        GLenum type,
        const GLvoid * indices) = 0;

	pub virtual ~IGL() = default;
};
