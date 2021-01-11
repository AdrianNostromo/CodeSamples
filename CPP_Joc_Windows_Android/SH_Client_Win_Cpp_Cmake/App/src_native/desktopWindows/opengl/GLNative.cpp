#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>

#include "GLNative.h"
#include <base/opengl/IGL.h>
#include <base/exceptions/LogicException.h>
#include <base/opengl/renderContext/RenderContext.h>

const GLenum IGL::BYTE = GL_BYTE;
const GLenum IGL::UNSIGNED_BYTE = GL_UNSIGNED_BYTE;
const GLenum IGL::SHORT = GL_SHORT;
const GLenum IGL::UNSIGNED_SHORT = GL_UNSIGNED_SHORT;
const GLenum IGL::INT = GL_INT;
const GLenum IGL::UNSIGNED_INT = GL_UNSIGNED_INT;
const GLenum IGL::FIXED = GL_FIXED;
const GLenum IGL::FLOAT = GL_FLOAT;
const GLenum IGL::UNSIGNED_INT_24_8 = GL_UNSIGNED_INT_24_8;
const GLenum IGL::FLOAT_32_UNSIGNED_INT_24_8_REV = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;

const GLenum IGL::INVALID_INDEX = GL_INVALID_INDEX;

const GLuint IGL::POINTS = GL_POINTS;

const GLuint IGL::LINES = GL_LINES;
const GLuint IGL::LINE_STRIP = GL_LINE_STRIP;
const GLuint IGL::LINE_LOOP = GL_LINE_LOOP;

const GLuint IGL::TRIANGLES = GL_TRIANGLES;
const GLuint IGL::TRIANGLE_STRIP = GL_TRIANGLE_STRIP;
const GLuint IGL::TRIANGLE_FAN = GL_TRIANGLE_FAN;

const GLuint IGL::VERTEX_SHADER = GL_VERTEX_SHADER;
const GLuint IGL::FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
const GLuint IGL::COMPILE_STATUS = GL_COMPILE_STATUS;
const GLuint IGL::INFO_LOG_LENGTH = GL_INFO_LOG_LENGTH;

const GLuint IGL::LINK_STATUS = GL_LINK_STATUS;
const GLuint IGL::VALIDATE_STATUS = GL_VALIDATE_STATUS;

const GLuint IGL::TEXTURE_2D = GL_TEXTURE_2D;
const GLuint IGL::TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY;
const GLuint IGL::TEXTURE_MIN_FILTER = GL_TEXTURE_MIN_FILTER;
const GLuint IGL::TEXTURE_MAG_FILTER = GL_TEXTURE_MAG_FILTER;
const GLuint IGL::TEXTURE_WRAP_S = GL_TEXTURE_WRAP_S;
const GLuint IGL::TEXTURE_WRAP_T = GL_TEXTURE_WRAP_T;
const GLuint IGL::TEXTURE_BORDER_COLOR = GL_TEXTURE_BORDER_COLOR;

// This is textre format.
const GLenum IGL::DEPTH_COMPONENT = GL_DEPTH_COMPONENT;

// These are signed internal formats.
const GLenum IGL::DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16;
const GLenum IGL::DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24;
const GLenum IGL::DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F;

const GLuint IGL::CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE;
const GLuint IGL::CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER;
const GLuint IGL::MIRRORED_REPEAT = GL_MIRRORED_REPEAT;
const GLuint IGL::REPEAT = GL_REPEAT;

const GLuint IGL::NEAREST = GL_NEAREST;
const GLuint IGL::LINEAR = GL_LINEAR;
const GLuint IGL::NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST;
const GLuint IGL::LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST;
const GLuint IGL::NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR;
const GLuint IGL::LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR;

const GLuint IGL::RGBA8 = GL_RGBA8;
const GLuint IGL::RGBA = GL_RGBA;

const GLuint IGL::TEXTURE0 = GL_TEXTURE0;

const GLuint IGL::COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT;

const GLuint IGL::NO_ERROR = GL_NO_ERROR;

const GLuint IGL::INVALID_ENUM = GL_INVALID_ENUM;
const GLuint IGL::INVALID_VALUE = GL_INVALID_VALUE;
const GLuint IGL::INVALID_OPERATION = GL_INVALID_OPERATION;
const GLuint IGL::OUT_OF_MEMORY = GL_OUT_OF_MEMORY;
const GLuint IGL::INVALID_FRAMEBUFFER_OPERATION = GL_INVALID_FRAMEBUFFER_OPERATION;

const GLenum IGL::MAX_TEXTURE_IMAGE_UNITS = GL_MAX_TEXTURE_IMAGE_UNITS;
const GLenum IGL::MAX_TEXTURE_SIZE = GL_MAX_TEXTURE_SIZE;
const GLenum IGL::MAX_VERTEX_TEXTURE_IMAGE_UNITS = GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS;
const GLenum IGL::MAX_COMBINED_TEXTURE_IMAGE_UNITS = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;

const GLenum IGL::MAX_ELEMENTS_VERTICES = GL_MAX_ELEMENTS_VERTICES;
const GLenum IGL::MAX_ELEMENTS_INDICES = GL_MAX_ELEMENTS_INDICES;

const GLenum IGL::BLEND = GL_BLEND;
const GLenum IGL::SRC_ALPHA = GL_SRC_ALPHA;
const GLenum IGL::ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA;

const GLenum IGL::CW = GL_CW;
const GLenum IGL::CCW = GL_CCW;

const GLenum IGL::BACK = GL_BACK;
const GLenum IGL::FRONT = GL_FRONT;
const GLenum IGL::CULL_FACE = GL_CULL_FACE;
const GLenum IGL::DEPTH_TEST = GL_DEPTH_TEST;
const GLenum IGL::SCISSOR_TEST = GL_SCISSOR_TEST;
const GLenum IGL::LESS = GL_LESS;
const GLenum IGL::DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT;

const GLenum IGL::NONE = GL_NONE;
const GLenum IGL::ONE = GL_ONE;
const GLenum IGL::ZERO = GL_ZERO;

const GLenum IGL::FRAMEBUFFER = GL_FRAMEBUFFER;
const GLenum IGL::ARRAY_BUFFER = GL_ARRAY_BUFFER;
const GLenum IGL::ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER;
const GLenum IGL::UNIFORM_BUFFER = GL_UNIFORM_BUFFER;
const GLenum IGL::STATIC_DRAW = GL_STATIC_DRAW;
const GLenum IGL::DYNAMIC_DRAW = GL_DYNAMIC_DRAW;

const GLenum IGL::DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT;

const GLenum IGL::VERSION = GL_VERSION;
const GLenum IGL::MAJOR_VERSION = GL_MAJOR_VERSION;
const GLenum IGL::MINOR_VERSION = GL_MINOR_VERSION;

const GLenum IGL::UNIFORM_BLOCK_DATA_SIZE = GL_UNIFORM_BLOCK_DATA_SIZE;
const GLenum IGL::MAX_UNIFORM_BLOCK_SIZE = GL_MAX_UNIFORM_BLOCK_SIZE;
const GLenum IGL::UNIFORM_BUFFER_OFFSET_ALIGNMENT = GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT;
const GLenum IGL::MAX_VERTEX_UNIFORM_BLOCKS = GL_MAX_VERTEX_UNIFORM_BLOCKS;
const GLenum IGL::MAX_FRAGMENT_UNIFORM_BLOCKS = GL_MAX_FRAGMENT_UNIFORM_BLOCKS;
const GLenum IGL::MAX_COMBINED_UNIFORM_BLOCKS = GL_MAX_COMBINED_UNIFORM_BLOCKS;
const GLenum IGL::MAX_VERTEX_UNIFORM_COMPONENTS = GL_MAX_VERTEX_UNIFORM_COMPONENTS;
const GLenum IGL::MAX_VERTEX_UNIFORM_VECTORS = GL_MAX_VERTEX_UNIFORM_VECTORS;
const GLenum IGL::MAX_FRAGMENT_UNIFORM_VECTORS = GL_MAX_FRAGMENT_UNIFORM_VECTORS;
const GLenum IGL::MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS = GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS;
const GLenum IGL::MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS = GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS;

const GLboolean IGL::TRUE = GL_TRUE;
const GLboolean IGL::FALSE = GL_FALSE;

GLNative::GLNative()
    : super()
{
    //void
}

void GLNative::genTextures(GLsizei n, GLuint *textures) {
    glGenTextures(n, textures);
}

void GLNative::activeTexture(GLenum texture) {
    glActiveTexture(texture);
}

void GLNative::bindTexture(GLenum target, GLuint texture) {
    if(activeLocalsProfiler != nullptr) {
        activeLocalsProfiler->bindTexture++;
    }

    glBindTexture(target, texture);
}

void GLNative::deleteTextures(GLsizei n, const GLuint * textures, unsigned int loggedSizeBytes) {
    if (activeGlobalsCumulativeProfiler != nullptr) {
        if (n != 1) {
            // Profiling logic supports only 1 resource delete at a time.
            // Need to use a "uint* loggedSizeBytes" to allow for multiple deletes. Implement when used.
            throw LogicException(LOC);
        }
        
        activeGlobalsCumulativeProfiler->texturesCount_cumulative--;

        activeGlobalsCumulativeProfiler->vram_bytes_cumulative -= loggedSizeBytes;
    }

    glDeleteTextures(n, textures);
}

void GLNative::texParameteri(GLenum target, GLenum pname, GLint param) {
    glTexParameteri(target, pname, param);
}

void GLNative::texParameterfv(GLenum target, GLenum pname, const GLfloat* param) {
    glTexParameterfv(target, pname, param);
}

unsigned int GLNative::texStorage2D(GLenum target, GLsizei levels, GLenum sizedInternalFormat, GLsizei width, GLsizei height) {
    unsigned int loggedSizeBytes = -0;

    if (activeGlobalsCumulativeProfiler != nullptr) {
        activeGlobalsCumulativeProfiler->createCalls_textures++;

        activeGlobalsCumulativeProfiler->texturesCount_cumulative++;

        if (levels != 1) {
            // Multiple levels not implemented in the below calculations. Implement when needed.
            throw LogicException(LOC);
        }
        
        if (target == GL_TEXTURE_2D) {
            if (sizedInternalFormat == IGL::RGBA8) {
                loggedSizeBytes = 4/*chanels*/ * 1/*byte*/ * width * height;
            } else if (sizedInternalFormat == IGL::DEPTH_COMPONENT32F) {
                loggedSizeBytes = 1/*chanels*/ * 4/*byte*/ * width * height;
            } else {
                // Unsupported format. Implement when needed.
                throw LogicException(LOC);
            }
        } else {
            // Unsupported target. Implement when needed.
            throw LogicException(LOC);
        }

        activeGlobalsCumulativeProfiler->vram_bytes_cumulative += loggedSizeBytes;
    }

    glTexStorage2D(target, levels, sizedInternalFormat, width, height);

    return loggedSizeBytes;
}

unsigned int GLNative::texStorage3D(
    GLenum target, GLsizei levels, GLenum internalformat,
    GLsizei width, GLsizei height, GLsizei depth)
{
    unsigned int loggedSizeBytes = -0;

    if (activeGlobalsCumulativeProfiler != nullptr) {
        activeGlobalsCumulativeProfiler->createCalls_textures++;

        activeGlobalsCumulativeProfiler->texturesCount_cumulative++;

        if (levels != 1) {
            // Multiple levels not implemented in the below calculations. Implement when needed.
            throw LogicException(LOC);
        }

        if (target == IGL::TEXTURE_2D_ARRAY) {
            if (internalformat == IGL::RGBA8) {
                loggedSizeBytes = 4/*chanels*/ * 1/*byte_per_chanel*/ * width * height * depth;
            } else {
                // Unsupported format. Implement when needed.
                throw LogicException(LOC);
            }
        } else {
            // Unsupported target. Implement when needed.
            throw LogicException(LOC);
        }

        activeGlobalsCumulativeProfiler->vram_bytes_cumulative += loggedSizeBytes;
    }

    glTexStorage3D(
        target, levels, internalformat, 
        width, height, depth
    );

    return loggedSizeBytes;
}

unsigned int GLNative::texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) {
    unsigned int loggedSizeBytes = -0;

    if (border != 0) {
        // Border must be 0. Source is the docs: http://docs.gl/es3/glTexImage2D
    }

    if (activeGlobalsCumulativeProfiler != nullptr) {
        activeGlobalsCumulativeProfiler->createCalls_textures++;

        activeGlobalsCumulativeProfiler->texturesCount_cumulative++;

        if (border != 0) {
            // Border is not supported in the size calculations.
            // Implement when needed.
            throw LogicException(LOC);
        }
        
        if (target == IGL::TEXTURE_2D_ARRAY) {
            if (internalFormat == IGL::RGBA8) {
                loggedSizeBytes = 4/*chanels*/ * 1/*byte_per_chanel*/ * width * height;
            } else {
                // Unsupported format. Implement when needed.
                throw LogicException(LOC);
            }
        } else {
            // Unsupported target. Implement when needed.
            throw LogicException(LOC);
        }

        activeGlobalsCumulativeProfiler->vram_bytes_cumulative += loggedSizeBytes;
    }

    if (activeGlobalsProfiler != nullptr && pixels != nullptr) {
        if (format == IGL::RGBA && type == IGL::UNSIGNED_BYTE) {
            activeGlobalsProfiler->textureUploadCount++;
            activeGlobalsProfiler->textureUploadBytes += 4/*chanels*/ * 1/*bytes_per_chanel*/ * width * height;
        } else {
            // Unhandled format.
            throw LogicException(LOC);
        }
    }

    glTexImage2D(target, level, internalFormat, width, height, border, format, type, pixels);

    return loggedSizeBytes;
}

void GLNative::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data) {
    if (activeGlobalsProfiler != nullptr) {
        if (format == IGL::RGBA && type == IGL::UNSIGNED_BYTE) {
            activeGlobalsProfiler->textureUploadCount++;
            activeGlobalsProfiler->textureUploadBytes += 4/*chanels*/ * 1/*bytes_per_chanel*/ * width * height;
        } else {
            // Unhandled format.
            throw LogicException(LOC);
        }
    }

    glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, data);
}

void GLNative::texSubImage3D(
    GLenum target,
    GLint level,
    GLint xoffset, GLint yoffset, GLint zoffset,
    GLsizei width, GLsizei height, GLsizei depth,
    GLenum format, GLenum type,
    const void* data) 
{
    glTexSubImage3D(
        target, 
        level, 
        xoffset, yoffset, zoffset,
        width, height, depth,
        format, type, 
        data
    );
}

void GLNative::enableVertexAttribArray(GLuint index) {
    glEnableVertexAttribArray(index);
}

void GLNative::vertexAttribPointer(
    GLuint index,
    GLint size,
    GLenum type,
    GLboolean normalized,
    GLsizei stride,
    const GLvoid * pointer)
{
    glVertexAttribPointer(
        index,
        size,
        type,
        normalized,
        stride,
        pointer
    );
}

void GLNative::vertexAttribIPointer(
    GLuint index,
    GLint size,
    GLenum type,
    GLsizei stride,
    const GLvoid* pointer)
{
    glVertexAttribIPointer(
        index,
        size,
        type,
        stride,
        pointer
    );
}

void GLNative::disableVertexAttribArray(GLuint index) {
    glDisableVertexAttribArray(index);
}

GLuint GLNative::createProgram() {
    if (activeGlobalsCumulativeProfiler != nullptr) {
        activeGlobalsCumulativeProfiler->createCalls_shaderPrograms++;

        activeGlobalsCumulativeProfiler->shaderProgramsCount_cumulative++;
    }

    return glCreateProgram();
}

void GLNative::attachShader(GLuint program, GLuint shader) {
    return glAttachShader(program, shader);
}

void GLNative::linkProgram(GLuint program) {
    glLinkProgram(program);
}

void GLNative::validateProgram(GLuint program) {
    glValidateProgram(program);
}

GLuint GLNative::createShader(GLenum shaderType) {
    return glCreateShader(shaderType);
}

void GLNative::deleteShader(GLuint shader) {
    glDeleteShader(shader);
}

void GLNative::shaderSource(
    GLuint shader,
    GLsizei count,
    const GLchar * const *string,
    const GLint *length)
{
    glShaderSource(
        shader,
        count,
        string,
        length);
}

void GLNative::compileShader(GLuint shader) {
    glCompileShader(shader);
}

void GLNative::getShaderiv(
    GLuint shader,
    GLenum pname,
    GLint *params)
{
    glGetShaderiv(
        shader,
        pname,
        params);
}

void GLNative::getShaderInfoLog(
    GLuint shader,
    GLsizei maxLength,
    GLsizei *length,
    GLchar *infoLog)
{
    glGetShaderInfoLog(
        shader,
        maxLength,
        length,
        infoLog);
}

void GLNative::getProgramiv(GLuint program, GLenum pname, GLint *params) {
    glGetProgramiv(program, pname, params);
}

void GLNative::getProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
    glGetProgramInfoLog(program, bufSize, length, infoLog);
}

void GLNative::useProgram(GLuint program) {
    if(activeLocalsProfiler != nullptr) {
        activeLocalsProfiler->useProgram++;
    }

    glUseProgram(program);
}

GLint GLNative::getAttribLocation(
    GLuint program,
    const GLchar *name)
{
    return glGetAttribLocation(
        program,
        name);
}

GLint GLNative::getUniformLocation(
    GLuint program,
    const GLchar *name)
{
    return glGetUniformLocation(
        program,
        name);
}

GLuint GLNative::getUniformBlockIndex(
    GLuint program,
    const GLchar* uniformBlockName)
{
    return glGetUniformBlockIndex(
        program,
        uniformBlockName
    );
}

void GLNative::uniformBlockBinding(
    GLuint program,
    GLuint uniformBlockIndex,
    GLuint uniformBlockBinding)
{
    glUniformBlockBinding(
        program,
        uniformBlockIndex,
        uniformBlockBinding
    );
}

void GLNative::uniform1i(
    GLint location,
    GLint v0)
{
    if (activeGlobalsProfiler != nullptr) {
        activeGlobalsProfiler->shaderUniformSetCallsCount++;
        activeGlobalsProfiler->shaderUniformSetBytesCount += 4 * 1;
    }

    glUniform1i(
        location,
        v0
    );
}

void GLNative::uniform2i(
    GLint location,
    GLint v0, GLint v1)
{
    if (activeGlobalsProfiler != nullptr) {
        activeGlobalsProfiler->shaderUniformSetCallsCount++;
        activeGlobalsProfiler->shaderUniformSetBytesCount += 4 * 2;
    }

    glUniform2i(
        location,
        v0, v1
    );
}

void GLNative::uniform3i(
    GLint location,
    GLint v0, GLint v1, GLint v2)
{
    if (activeGlobalsProfiler != nullptr) {
        activeGlobalsProfiler->shaderUniformSetCallsCount++;
        activeGlobalsProfiler->shaderUniformSetBytesCount += 4 * 3;
    }

    glUniform3i(
        location,
        v0, v1, v2
    );
}

void GLNative::uniform4i(
    GLint location,
    GLint v0, GLint v1, GLint v2, GLint v3)
{
    if (activeGlobalsProfiler != nullptr) {
        activeGlobalsProfiler->shaderUniformSetCallsCount++;
        activeGlobalsProfiler->shaderUniformSetBytesCount += 4 * 4;
    }

    glUniform4i(
        location,
        v0, v1, v2, v3
    );
}

void GLNative::uniform1f(
    GLint location,
    GLfloat v)
{
    if (activeGlobalsProfiler != nullptr) {
        activeGlobalsProfiler->shaderUniformSetCallsCount++;
        activeGlobalsProfiler->shaderUniformSetBytesCount += 4;
    }

    glUniform1f(
        location,
        v
    );
}

void GLNative::uniform3f(
    GLint location,
    GLfloat v0,
    GLfloat v1,
    GLfloat v2)
{
    if (activeGlobalsProfiler != nullptr) {
        activeGlobalsProfiler->shaderUniformSetCallsCount++;
        activeGlobalsProfiler->shaderUniformSetBytesCount += 4 * 3;
    }

    glUniform3f(
        location,
        v0,
        v1,
        v2);
}

void GLNative::uniform4f(
    GLint location,
    GLfloat v0,
    GLfloat v1,
    GLfloat v2,
    GLfloat v3)
{
    if (activeGlobalsProfiler != nullptr) {
        activeGlobalsProfiler->shaderUniformSetCallsCount++;
        activeGlobalsProfiler->shaderUniformSetBytesCount += 4 * 4;
    }

    glUniform4f(
        location,
        v0,
        v1,
        v2,
        v3);
}

void GLNative::uniformMatrix3fv(
    GLint location,
    GLsizei count,
    GLboolean transpose,
    const GLfloat *value)
{
    if (activeGlobalsProfiler != nullptr) {
        activeGlobalsProfiler->shaderUniformSetCallsCount++;
        activeGlobalsProfiler->shaderUniformSetBytesCount += 4 * 3 * 3 * count;
    }

    glUniformMatrix3fv(
        location,
        count,
        transpose,
        value);
}

void GLNative::uniformMatrix4fv(
    GLint location,
    GLsizei count,
    GLboolean transpose,
    const GLfloat *value)
{
    if (activeGlobalsProfiler != nullptr) {
        activeGlobalsProfiler->shaderUniformSetCallsCount++;
        activeGlobalsProfiler->shaderUniformSetBytesCount += 4 * 4 * 4 * count;
    }

    glUniformMatrix4fv(
        location,
        count,
        transpose,
        value);
}

void GLNative::uniform1iv(
    GLint location,
    GLsizei count,
    const GLint *value)
{
    if (activeGlobalsProfiler != nullptr) {
        activeGlobalsProfiler->shaderUniformSetCallsCount++;
        activeGlobalsProfiler->shaderUniformSetBytesCount += 4 * count;
    }

    glUniform1iv(
        location,
        count,
        value);
}

void GLNative::deleteProgram(GLuint program) {
    if (activeGlobalsCumulativeProfiler != nullptr) {
        activeGlobalsCumulativeProfiler->shaderProgramsCount_cumulative--;
    }

    glDeleteProgram(program);
}

void GLNative::clearColor(
	GLfloat red,
	GLfloat green,
	GLfloat blue,
	GLfloat alpha)
{
    glClearColor(
		red,
		green,
		blue,
		alpha
	);
}

void GLNative::clear(GLbitfield mask) {
    glClear(mask);
}

void GLNative::viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    glViewport(x, y, width, height);
}

void GLNative::scissor(GLint x, GLint y, GLsizei width, GLsizei height) {
    glScissor(x, y, width, height);
}

GLenum GLNative::getError() {
    return glGetError();
}

void GLNative::getIntegerv(GLenum pname, GLint * params) {
    glGetIntegerv(pname, params);
}

const GLubyte* GLNative::getString(GLenum name) {
    return glGetString(name);
}

bool GLNative::isEnabled(GLenum cap) {
    return glIsEnabled(cap);
}

void GLNative::fromRenderContext_enable(GLenum cap) {
    glEnable(cap);
}

void GLNative::fromRenderContext_disable(GLenum cap) {
    glDisable(cap);
}

void GLNative::fromRenderContext_blendFunc(GLenum sfactor, GLenum dfactor) {
    glBlendFunc(sfactor, dfactor);
}

void GLNative::fromRenderContext_frontFace(GLenum mode) {
    glFrontFace(mode);
}
void GLNative::fromRenderContext_cullFace(GLenum mode) {
    glCullFace(mode);
}

void GLNative::fromRenderContext_depthFunc(GLenum func) {
    glDepthFunc(func);
}

void GLNative::fromRenderContext_depthRange(GLclampd zNear, GLclampd zFar) {
    glDepthRange(zNear, zFar);
}

void GLNative::fromRenderContext_depthMask(GLboolean depthMask) {
    glDepthMask(depthMask);
}

void GLNative::genBuffers(GLsizei n, GLuint * buffers) {
    glGenBuffers(n, buffers);
}

void GLNative::genVertexArrays(
    GLsizei n,
    GLuint* arrays)
{
    glGenVertexArrays(
        n,
        arrays
    );
}

void GLNative::bindVertexArray(GLuint array) {
    glBindVertexArray(array);
}

void GLNative::drawArrays(
    GLenum mode,
    GLint first,
    GLsizei count)
{
    if (activeLocalsProfiler != nullptr) {
        if (mode == IGL::LINES) {
            activeLocalsProfiler->drawLinesCount += count / 2;
        } else if (mode == IGL::TRIANGLES) {
            activeLocalsProfiler->drawTrianglesCount += count / 3;
        } else {
            // Unhandled mode.
            throw LogicException(LOC);
        }
    }

    glDrawArrays(
        mode,
        first,
        count
    );
}

void GLNative::genFramebuffers(GLsizei n, GLuint* framebuffers) {
    glGenFramebuffers(n, framebuffers);
}

void GLNative::deleteFramebuffers(GLsizei n, GLuint* framebuffers) {
    glDeleteFramebuffers(n, framebuffers);
}

void GLNative::bindFramebuffer(GLenum target, GLuint framebuffer) {
    glBindFramebuffer(target, framebuffer);
}

void GLNative::framebufferTexture2D(
    GLenum target,
    GLenum attachment,
    GLenum textarget,
    GLuint texture,
    GLint level)
{
    glFramebufferTexture2D(
        target,
        attachment,
        textarget,
        texture,
        level
    );
}

void GLNative::drawBuffer(GLenum mode) {
    glDrawBuffer(
        mode
    );
}

void GLNative::readBuffer(GLenum src) {
    glReadBuffer(src);
}

void GLNative::bindBuffer(GLenum target, GLuint buffer) {
    if(activeLocalsProfiler != nullptr) {
        activeLocalsProfiler->bindBuffer++;
    }

    glBindBuffer(target, buffer);
}

void GLNative::bindBufferBase(GLenum target, GLuint index, GLuint buffer) {
    if(activeLocalsProfiler != nullptr) {
        activeLocalsProfiler->bindBuffer++;
    }

    glBindBufferBase(target, index, buffer);
}

unsigned int GLNative::bufferData(
    GLenum target,
    GLsizeiptr sizeBytes,
    const GLvoid * data,
    GLenum usage)
{
    if (activeGlobalsCumulativeProfiler != nullptr) {
        activeGlobalsCumulativeProfiler->createCalls_buffers++;

        activeGlobalsCumulativeProfiler->buffersCount_cumulative++;

        activeGlobalsCumulativeProfiler->vram_bytes_cumulative += sizeBytes;
    }

    if (activeGlobalsProfiler != nullptr) {
        if (data != nullptr) {
            activeGlobalsProfiler->bufferUploadCount++;
            activeGlobalsProfiler->bufferUploadBytes += sizeBytes;
        }
    }

    glBufferData(
        target,
        sizeBytes,
        data,
        usage
    );

    return sizeBytes;
}

void GLNative::bufferSubData(
    GLenum target,
    GLintptr offset,
    GLsizeiptr sizeBytes,
    const GLvoid * data)
{
    if (activeGlobalsProfiler != nullptr) {
        activeGlobalsProfiler->bufferUploadCount++;
        activeGlobalsProfiler->bufferUploadBytes += sizeBytes;
    }

    glBufferSubData(
        target,
        offset,
        sizeBytes,
        data
    );
}

void GLNative::deleteBuffers(GLsizei n, const GLuint * buffers, unsigned int loggedSizeBytes) {
    if (activeGlobalsCumulativeProfiler != nullptr) {
        if (n != 1) {
            // Profiling logic supports only 1 resource delete at a time.
            // Need to use a "uint* loggedSizeBytes" to allow for multiple deletes. Implement when used.
            throw LogicException(LOC);
        }

        activeGlobalsCumulativeProfiler->buffersCount_cumulative--;

        if (loggedSizeBytes <= 0) {
            // Size is not correct.
            throw LogicException(LOC);
        }
        activeGlobalsCumulativeProfiler->vram_bytes_cumulative -= loggedSizeBytes;
    }

    glDeleteBuffers(n, buffers);
}

void GLNative::drawElements(
    GLenum mode,
    GLsizei count,
    GLenum type,
    const GLvoid* indices)
{
    if (!renderContext->getIsFlushed()) {
        // Must manually flush the renderContext changes before a draw call.
        throw LogicException(LOC);
    }

    if(activeLocalsProfiler != nullptr) {
        activeLocalsProfiler->drawElements++;
        activeLocalsProfiler->drawElementsCount += count;

        if (mode == IGL::LINES) {
            activeLocalsProfiler->drawLinesCount += count / 2;
        } else if (mode == IGL::TRIANGLES) {
            activeLocalsProfiler->drawTrianglesCount += count / 3;
        } else {
            // Unhandled mode.
            throw LogicException(LOC);
        }
    }

    glDrawElements(
        mode,
        count,
        type,
        indices
    );
}

GLNative::~GLNative() {
    //void
}
