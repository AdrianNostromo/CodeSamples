#pragma once

#include <base/gh.h>
#include <base/opengl/GLBase.h>

class GLNative : public GLBase {priv typedef GLBase super;pub dCtor(GLNative);
    pub explicit GLNative();

    pub void genTextures(GLsizei n, GLuint *textures) final;
    pub void activeTexture(GLenum texture) final;
    pub void bindTexture(GLenum target, GLuint texture) final;
    pub void deleteTextures(GLsizei n, const GLuint * textures, unsigned int loggedSizeBytes) final;
    pub void texParameteri(GLenum target, GLenum pname, GLint param) final;
    pub void texParameterfv(GLenum target, GLenum pname, const GLfloat* param) final;

    pub unsigned int texStorage2D(GLenum target, GLsizei levels, GLenum sizedInternalFormat, GLsizei width, GLsizei height) final;
    pub unsigned int texStorage3D(
        GLenum target, GLsizei levels, GLenum internalformat,
        GLsizei width, GLsizei height, GLsizei depth) final;
    
    pub unsigned int texImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) final;
    pub void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data) final;
    pub void texSubImage3D(
        GLenum target,
        GLint level,
        GLint xoffset, GLint yoffset, GLint zoffset,
        GLsizei width, GLsizei height, GLsizei depth,
        GLenum format, GLenum type,
        const void* data) final;

    pub void enableVertexAttribArray(GLuint index) final;
    pub void vertexAttribPointer(
        GLuint index,
        GLint size,
        GLenum type,
        GLboolean normalized,
        GLsizei stride,
        const GLvoid * pointer) final;
    pub void vertexAttribIPointer(
        GLuint index,
        GLint size,
        GLenum type,
        GLsizei stride,
        const GLvoid* pointer) final;
    pub void disableVertexAttribArray(GLuint index) final;

    pub GLuint createProgram() final;
    pub void attachShader(GLuint program, GLuint shader) final;
    pub void linkProgram(GLuint program) final;
    pub void validateProgram(GLuint program) final;
    pub GLuint createShader(GLenum shaderType) final;
    pub void deleteShader(GLuint shader) final;
    pub void shaderSource(
        GLuint shader,
        GLsizei count,
        const GLchar * const *string,
        const GLint *length) final;
    pub void compileShader(GLuint shader) final;

    pub void getShaderiv(
        GLuint shader,
        GLenum pname,
        GLint *params) final;
    pub void getShaderInfoLog(
        GLuint shader,
        GLsizei maxLength,
        GLsizei *length,
        GLchar *infoLog) final;

    pub void getProgramiv(GLuint program, GLenum pname, GLint *params) final;
    pub void getProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) final;

    pub void useProgram(GLuint program) final;
    pub GLint getAttribLocation(
        GLuint program,
        const GLchar *name) final;

    pub GLint getUniformLocation(
        GLuint program,
        const GLchar *name) final;
    pub GLuint getUniformBlockIndex(
        GLuint program,
        const GLchar * uniformBlockName) final;
    pub void uniformBlockBinding(
        GLuint program,
        GLuint uniformBlockIndex,
        GLuint uniformBlockBinding) final;
    
    pub void uniform1i(
        GLint location,
        GLint v0) final;
    pub void uniform2i(
        GLint location,
        GLint v0, GLint v1) final;
    pub void uniform3i(
        GLint location,
        GLint v0, GLint v1, GLint v2) final;
    pub void uniform4i(
        GLint location,
        GLint v0, GLint v1, GLint v2, GLint v3) final;

    pub void uniform1f(
        GLint location,
        GLfloat v) final;
    pub void uniform3f(
        GLint location,
        GLfloat v0,
        GLfloat v1,
        GLfloat v2) final;
    pub void uniform4f(
        GLint location,
        GLfloat v0,
        GLfloat v1,
        GLfloat v2,
        GLfloat v3) final;
    pub void uniformMatrix3fv(
        GLint location,
        GLsizei count,
        GLboolean transpose,
        const GLfloat *value) final;
    pub void uniformMatrix4fv(
        GLint location,
        GLsizei count,
        GLboolean transpose,
        const GLfloat *value) final;
    pub void uniform1iv(
        GLint location,
        GLsizei count,
        const GLint *value) final;
    pub void deleteProgram(GLuint program) final;

    pub void viewport(GLint x, GLint y, GLsizei width, GLsizei height) final;
    pub void scissor(GLint x, GLint y, GLsizei width, GLsizei height) final;

    pub void clearColor(
		GLfloat red,
		GLfloat green,
		GLfloat blue,
		GLfloat alpha) final;
    pub void clear(GLbitfield mask) final;

    pub GLenum getError() final;

    pub void getIntegerv(GLenum pname, GLint * params) final;
    pub const GLubyte* getString(GLenum name) final;
    pub bool isEnabled(GLenum cap) final;
    pub void fromRenderContext_enable(GLenum cap) final;
    pub void fromRenderContext_disable(GLenum cap) final;
    
    pub void fromRenderContext_blendFunc(GLenum sfactor, GLenum dfactor) final;

    pub void fromRenderContext_frontFace(GLenum mode) final;
    pub void fromRenderContext_cullFace(GLenum mode) final;
    
    pub void fromRenderContext_depthFunc(GLenum func) final;
    pub void fromRenderContext_depthRange(GLclampd zNear, GLclampd zFar) final;
    pub void fromRenderContext_depthMask(GLboolean depthMask) final;

    pub void genBuffers(GLsizei n, GLuint * buffers) final;

    pub void genVertexArrays(
        GLsizei n,
        GLuint* arrays) final;
    pub void bindVertexArray(GLuint array) final;
    pub void drawArrays(
        GLenum mode,
        GLint first,
        GLsizei count) final;

    pub void genFramebuffers(GLsizei n, GLuint* framebuffers) final;
    pub void deleteFramebuffers(GLsizei n, GLuint* framebuffers) final;
    pub void bindFramebuffer(GLenum target, GLuint framebuffer) final;
    pub void framebufferTexture2D(
        GLenum target,
        GLenum attachment,
        GLenum textarget,
        GLuint texture,
        GLint level) final;
    pub void drawBuffer(GLenum mode) final;
    pub void readBuffer(GLenum src) final;

    pub void bindBuffer(GLenum target, GLuint buffer) final;
    pub void bindBufferBase(GLenum target, GLuint index, GLuint buffer) final;

    pub unsigned int bufferData(
        GLenum target,
        GLsizeiptr sizeBytes,
        const GLvoid * data,
        GLenum usage) final;
    pub void bufferSubData(
        GLenum target,
        GLintptr offset,
        GLsizeiptr sizeBytes,
        const GLvoid * data) final;
    pub void deleteBuffers(GLsizei n, const GLuint * buffers, unsigned int loggedSizeBytes) final;
    pub void drawElements(
        GLenum mode,
        GLsizei count,
        GLenum type,
        const GLvoid * indices) final;

    pub ~GLNative() override;
};
