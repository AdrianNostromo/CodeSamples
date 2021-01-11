#include "ShaderProgram.h"
#include <iostream>
#include <base/opengl/util/GLUtil.h>
#include <base/exceptions/LogicException.h>
#include <graphics/util/Renderable.h>
#include <graphics/model/NodePart.h>
#include <graphics/geometry/data/MeshPart.h>
#include <graphics/geometry/data/VertexBufferObject.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace graphics;

ShaderProgram::ShaderProgram(
	const std::string& vsSource, const std::string& fsSource)
    : super(&BindLocations::SHADER),
	vsSource(vsSource), fsSource(fsSource)
{
	//void;
}

void ShaderProgram::createMain() {
    super::createMain();

    //void
}

void ShaderProgram::onLoadToGl() {
	super::onLoadToGl();

    if (createShader() != 0) {
        throw LogicException(LOC);
    }
}

void ShaderProgram::onUnloadFromGl() {
    if (gHandle != 0) {
		GLUtil::renderContext->unbindShaderDependants(this);

        GLUtil::gl->deleteProgram(gHandle);

        gHandle = 0;
    }

    super::onUnloadFromGl();
}

void ShaderProgram::onBind(int slot, graphics::ShaderProgram* shader) {
	super::onBind(slot, shader);

	if (!GLUtil::gl->getIsGlContext()) {
		throw LogicException(LOC);
	}

	GLUtil::gl->useProgram(gHandle);
}

void ShaderProgram::onUnbind(int slot, graphics::ShaderProgram* shader) {
	super::onUnbind(slot, shader);

	if (!GLUtil::gl->getIsGlContext()) {
		throw LogicException(LOC);
	}

	GLUtil::gl->useProgram(0);
}

int ShaderProgram::createShader() {
    gHandle = GLUtil::gl->createProgram();
	unsigned int errorCode = GLUtil::gl->getError();
	if (errorCode != IGL::NO_ERROR) {
		throw LogicException(LOC);
	}
	if (gHandle == 0) {
		throw LogicException(LOC);
	}

	unsigned int vs = compileShader(IGL::VERTEX_SHADER, vsSource);
	if (vs == 0) {
		throw LogicException(LOC);
	}

	unsigned int fs = compileShader(IGL::FRAGMENT_SHADER, fsSource);
	if (fs == 0) {
		throw LogicException(LOC);
	}

	CallGLChecked(GLUtil::gl->attachShader(gHandle, vs));
	CallGLChecked(GLUtil::gl->attachShader(gHandle, fs));
	checkedLinkProgram(gHandle);
	
	// Validation must be called after first use because some uniforms must be initialised before use.
	// Error occurs if 2 texture samplers exist because they will both point to the same texture (zero) and that is not allowed.
	//checkedValidateProgram();

	CallGLChecked(GLUtil::gl->deleteShader(vs));
	CallGLChecked(GLUtil::gl->deleteShader(fs));

    GLUtil::gl->enableVertexAttribArray(0);
    GLUtil::gl->vertexAttribPointer(0, 2, IGL::FLOAT, IGL::FALSE, sizeof(float) * 2, 0);

	return 0;
}

unsigned int ShaderProgram::compileShader(unsigned int type, const std::string& source) {
	unsigned int id = GLUtil::gl->createShader(type);

	const char *c_str = source.c_str();
    GLUtil::gl->shaderSource(id, 1, &c_str, nullptr);

    GLUtil::gl->compileShader(id);

	int result;
	GLUtil::gl->getShaderiv(id, IGL::COMPILE_STATUS, &result);
	if (result == IGL::FALSE) {
		int length;
		GLUtil::gl->getShaderiv(id, IGL::INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		GLUtil::gl->getShaderInfoLog(id, length, &length, message);

		std::string s = (std::string("Failed to compile shader ") + ((type == IGL::VERTEX_SHADER) ? "vertex" : "fragment")) + "/n";
		GlobalAccessLogHandler::devLogHandler->post_debug(s.c_str());
		GlobalAccessLogHandler::devLogHandler->post_debug(message);
		GlobalAccessLogHandler::devLogHandler->post_debug(source.c_str());

		GLUtil::gl->deleteShader(id);

		return 0;
	}

	return id;
}

int ShaderProgram::getAttribLocationOptional(std::string& name) {
    int* attribLocationP = attribLocationsCache.getPointer(name);
	if (attribLocationP == nullptr) {
		CallGLChecked(int location = GLUtil::gl->getAttribLocation(gHandle, name.c_str()));
		attribLocationP = attribLocationsCache.putDirect(name, location);
	}

	return *attribLocationP;
}
int ShaderProgram::getAttribLocationMustExist(std::string& name) {
	int* attribLocationP = attribLocationsCache.getPointer(name);
	if (attribLocationP == nullptr) {
		CallGLChecked(int location = GLUtil::gl->getAttribLocation(gHandle, name.c_str()));
		attribLocationP = attribLocationsCache.putDirect(name, location);
	}

	if (*attribLocationP == -1) {
		// Not found in the shader.
		throw LogicException(LOC);
	}

	return *attribLocationP;
}

int ShaderProgram::getUniformLocation(std::string& name, bool mustExist) {
    int* uniformLocationP = uniformLocationsCache.getPointer(name);
	if (uniformLocationP == nullptr) {
		CallGLChecked(int location = GLUtil::gl->getUniformLocation(gHandle, name.c_str()));
		uniformLocationP = uniformLocationsCache.putDirect(name, location);
	}

	if (mustExist && *uniformLocationP < 0) {
		// Not found in the shader.
		throw LogicException(LOC);
	}

	return *uniformLocationP;
}

int ShaderProgram::getUniformLocationOptional(std::string& name) {
    int* uniformLocationP = uniformLocationsCache.getPointer(name);
	if (uniformLocationP == nullptr) {
		CallGLChecked(int location = GLUtil::gl->getUniformLocation(gHandle, name.c_str()));
		uniformLocationP = uniformLocationsCache.putDirect(name, location);
	}

	return *uniformLocationP;
}

int ShaderProgram::getUniformLocationMustExist(std::string& name) {
	int uniformLocationP = getUniformLocationOptional(name);
	if (uniformLocationP < 0) {
		// Not found in the shader.
		throw LogicException(LOC);
	}

	return uniformLocationP;
}

unsigned int ShaderProgram::getUniformBlockIndexOptional(std::string& uniformBlockName) {
	int* uniformBlockIndex = uniformBlockIndicesCache.getPointer(uniformBlockName);
	if (uniformBlockIndex == nullptr) {
		CallGLChecked(unsigned int location = GLUtil::gl->getUniformBlockIndex(gHandle, uniformBlockName.c_str()));
		uniformBlockIndex = uniformBlockIndicesCache.putDirect(uniformBlockName, location);
	}

	return *uniformBlockIndex;
}

unsigned int ShaderProgram::getUniformBlockIndexMustExist(std::string& uniformBlockName) {
	int uniformBlockIndex = getUniformBlockIndexOptional(uniformBlockName);
	if (uniformBlockIndex == IGL::INVALID_INDEX) {
		// Not found in the shader.
		throw LogicException(LOC);
	}

	return uniformBlockIndex;
}

void ShaderProgram::uniformBlockBinding(std::string& uniformBlockName, int uniformBindSlot) {
	unsigned int uniformBlockIndex = getUniformBlockIndexMustExist(uniformBlockName);
	GLUtil::gl->uniformBlockBinding(gHandle, uniformBlockIndex, uniformBindSlot);
}

void ShaderProgram::setUniform1iOptional(std::string& name, int value0) {
	int location = getUniformLocationOptional(name);
	if (location == -1) {
		return;
	}

    GLUtil::gl->uniform1i(location, value0);
}

void ShaderProgram::setUniform1iMustExist(std::string& name, int value0) {
	int location = getUniformLocationMustExist(name);

    GLUtil::gl->uniform1i(location, value0);
}

void ShaderProgram::setUniform1i(
	std::string& name, bool mustExist,
	int value0)
{
	int location = getUniformLocation(name, mustExist);
	if (location >= 0) {
		GLUtil::gl->uniform1i(location, value0);
	}
}

void ShaderProgram::setUniform2i(
	std::string& name, bool mustExist,
	int value0, int value1)
{
	int location = getUniformLocation(name, mustExist);
	if (location >= 0) {
		GLUtil::gl->uniform2i(location, value0, value1);
	}
}

void ShaderProgram::setUniform3i(
	std::string& name, bool mustExist,
	int value0, int value1, int value2)
{
	int location = getUniformLocation(name, mustExist);
	if (location >= 0) {
		GLUtil::gl->uniform3i(location, value0, value1, value2);
	}
}

void ShaderProgram::setUniform4i(
	std::string& name, bool mustExist,
	int value0, int value1, int value2, int value3)
{
	int location = getUniformLocation(name, mustExist);
	if (location >= 0) {
		GLUtil::gl->uniform4i(location, value0, value1, value2, value3);
	}
}

void ShaderProgram::setUniformInt(std::string& name, int v, bool mustExist) {
	int location = getUniformLocationOptional(name);
	if (location == -1) {
		if (mustExist) {
			throw LogicException(LOC);
		}

		return;
	}

    GLUtil::gl->uniform1i(location, v);
}

void ShaderProgram::setUniformIntB(std::string name, int v, bool mustExist) {
	setUniformInt(name, v, mustExist);
}

void ShaderProgram::setUniformFloat(std::string& name, float v, bool mustExist) {
	int location = getUniformLocationOptional(name);
	if (location == -1) {
		if (mustExist) {
			throw LogicException(LOC);
		}

		return;
	}

    GLUtil::gl->uniform1f(location, v);
}

void ShaderProgram::setUniformFloatB(std::string name, float v, bool mustExist) {
	setUniformFloat(name, v, mustExist);
}

void ShaderProgram::setUniformVec3(std::string& name, float v0, float v1, float v2, bool mustExist) {
	int location = getUniformLocationOptional(name);
	if (location == -1) {
		if (mustExist) {
			throw LogicException(LOC);
		}

		return;
	}

    GLUtil::gl->uniform3f(location, v0, v1, v2);
}

void ShaderProgram::setUniformVec3B(std::string name, float v0, float v1, float v2, bool mustExist) {
	setUniformVec3(name, v0, v1, v2, mustExist);
}

void ShaderProgram::setUniformVec3(std::string& name, const Vector3& v, bool mustExist) {
	int location = getUniformLocationOptional(name);
	if (location == -1) {
		if (mustExist) {
			throw LogicException(LOC);
		}

		return;
	}

    GLUtil::gl->uniform3f(location, v.x, v.y, v.z);
}

void ShaderProgram::setUniformVec3B(std::string name, const Vector3& v, bool mustExist) {
	setUniformVec3(name, v, mustExist);
}

void ShaderProgram::setUniform4fOptional(std::string& name, float v0, float v1, float v2, float v3) {
	int location = getUniformLocationOptional(name);
	if (location == -1) {
		return;
	}

    GLUtil::gl->uniform4f(location, v0, v1, v2, v3);
}

void ShaderProgram::setUniform4fMustExist(std::string& name, float v0, float v1, float v2, float v3) {
	int location = getUniformLocationMustExist(name);

    GLUtil::gl->uniform4f(location, v0, v1, v2, v3);
}

void ShaderProgram::setUniformMatrix3Optional(std::string& name, Matrix3* value) {
	setUniformMatrix3Optional(name, 1, value);
}
void ShaderProgram::setUniformMatrix3MustExist(std::string& name, Matrix3* value) {
	setUniformMatrix3MustExist(name, 1, value);
}

void ShaderProgram::setUniformMatrix3Optional(std::string& name, unsigned int count, Matrix3* value) {
	int location = getUniformLocationOptional(name);
	if (location == -1) {
		return;
	}

    GLUtil::gl->uniformMatrix3fv(location, count, IGL::FALSE, value->val);
}
void ShaderProgram::setUniformMatrix3MustExist(std::string& name, unsigned int count, Matrix3* value) {
	int location = getUniformLocationMustExist(name);

    GLUtil::gl->uniformMatrix3fv(location, count, IGL::FALSE, value->val);
}

void ShaderProgram::setUniformMatrix4Optional(std::string& name, Matrix4 const* value) {
	setUniformMatrix4Optional(name, 1, value);
}
void ShaderProgram::setUniformMatrix4MustExist(std::string& name, Matrix4 const* value) {
	setUniformMatrix4MustExist(name, 1, value);
}

void ShaderProgram::setUniformMatrix4Optional(std::string& name, unsigned int count, Matrix4 const* value) {
	int location = getUniformLocationOptional(name);
	if (location == -1) {
		return;
	}

    GLUtil::gl->uniformMatrix4fv(location, count, IGL::FALSE, value->val);
}
void ShaderProgram::setUniformMatrix4MustExist(std::string& name, unsigned int count, Matrix4 const* value) {
	int location = getUniformLocationMustExist(name);

    GLUtil::gl->uniformMatrix4fv(location, count, IGL::FALSE, value->val);
}

void ShaderProgram::setUniform1ivOptional(std::string& name, unsigned int count, const int* value) {
	int location = getUniformLocationOptional(name);
	if (location == -1) {
		return;
	}

    GLUtil::gl->uniform1iv(location, count, value);
}
void ShaderProgram::setUniform1ivMustExist(std::string& name, unsigned int count, const int* value) {
	int location = getUniformLocationMustExist(name);

    GLUtil::gl->uniform1iv(location, count, value);
}

void ShaderProgram::checkedLinkProgram(unsigned int gHandle) {
	CallGLChecked(GLUtil::gl->linkProgram(gHandle));

	int result;
	GLUtil::gl->getProgramiv(gHandle, IGL::LINK_STATUS, &result);
	if (result == IGL::FALSE) {
        int length;
        GLUtil::gl->getProgramiv(gHandle, IGL::INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        GLUtil::gl->getProgramInfoLog(gHandle, length, &length, message);

		GlobalAccessLogHandler::devLogHandler->post_debug((std::string("Failed to link shader program: ") + message).c_str());

		throw LogicException(LOC);
	}
}

void ShaderProgram::checkedValidateProgramIfNeeded() {
	if (isValidated) {
		return;
	}
	isValidated = true;

	CallGLChecked(GLUtil::gl->validateProgram(gHandle));

	int result;
	GLUtil::gl->getProgramiv(gHandle, IGL::VALIDATE_STATUS, &result);
	if (result == IGL::FALSE) {
        int length;
        GLUtil::gl->getProgramiv(gHandle, IGL::INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        GLUtil::gl->getProgramInfoLog(gHandle, length, &length, message);

		GlobalAccessLogHandler::devLogHandler->post_debug((std::string("Failed to validate shader program:") + message).c_str());

        throw LogicException(LOC);
	}
}

void ShaderProgram::disposeMain() {
	//void

    super::disposeMain();
}

ShaderProgram::~ShaderProgram() {
	//void
}
