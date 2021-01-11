#pragma once

#include <base/gh.h>
#include <base/math/Matrix3.h>
#include <base/math/Matrix4.h>
#include <base/map/Map1D.h>
#include <base/opengl/util/ManagedGlResource.h>
#include <string>
#include <base/math/Vector3.h>

namespace graphics {
	class NodePart;
};

namespace graphics {
class ShaderProgram : public ManagedGlResource {priv typedef ManagedGlResource super;pub dCtor(ShaderProgram)
    priv std::string vsSource;
	priv std::string fsSource;

	priv Map1D<std::string, int> attribLocationsCache{};
	priv Map1D<std::string, int> uniformLocationsCache{};
	priv Map1D<std::string, int> uniformBlockIndicesCache{};

	pub explicit ShaderProgram(
		const std::string& vsSource, const std::string& fsSource);
	prot void createMain() override;

	priv int createShader();
	priv static unsigned int compileShader(unsigned int type, const std::string& source);

	priv static void checkedLinkProgram(unsigned int program);

	priv bool isValidated = false;
	pub void checkedValidateProgramIfNeeded();

	prot void onLoadToGl() override;
	prot void onUnloadFromGl() override;

	pub void onBind(int slot, graphics::ShaderProgram* shader) override;
	pub void onUnbind(int slot, graphics::ShaderProgram* shader) override;

	pub int getAttribLocationOptional(std::string& name);
	pub int getAttribLocationMustExist(std::string& name);
	
	pub int getUniformLocation(std::string& name, bool mustExist);
	pub int getUniformLocationOptional(std::string& name);
	pub int getUniformLocationMustExist(std::string& name);

	pub unsigned int getUniformBlockIndexOptional(std::string& uniformBlockName);
	pub unsigned int getUniformBlockIndexMustExist(std::string& uniformBlockName);

	pub void uniformBlockBinding(std::string& uniformBlockName, int uniformBindSlot);

	pub void setUniform1iOptional(std::string& name, int value0);
	pub void setUniform1iMustExist(std::string& name, int value0);

	pub void setUniform1i(
		std::string& name, bool mustExist,
		int value0);
	pub void setUniform2i(
		std::string& name, bool mustExist,
		int value0, int value1);
	pub void setUniform3i(
		std::string& name, bool mustExist,
		int value0, int value1, int value2);
	pub void setUniform4i(
		std::string& name, bool mustExist,
		int value0, int value1, int value2, int value3);
	
	pub void setUniformInt(std::string& name, int v, bool mustExist);
	pub void setUniformIntB(std::string name, int v, bool mustExist);
	
	pub void setUniformFloat(std::string& name, float v, bool mustExist);
	pub void setUniformFloatB(std::string name, float v, bool mustExist);

	pub void setUniformVec3(std::string& name, float v0, float v1, float v2, bool mustExist);
	pub void setUniformVec3B(std::string name, float v0, float v1, float v2, bool mustExist);
	pub void setUniformVec3(std::string& name, const Vector3& v, bool mustExist);
	pub void setUniformVec3B(std::string name, const Vector3& v, bool mustExist);

	pub void setUniform4fOptional(std::string& name, float v0, float v1, float v2, float v3);
	pub void setUniform4fMustExist(std::string& name, float v0, float v1, float v2, float v3);
	pub void setUniformMatrix3Optional(std::string& name, Matrix3* value);
	pub void setUniformMatrix3MustExist(std::string& name, Matrix3* value);
	pub void setUniformMatrix3Optional(std::string& name, unsigned int count, Matrix3* value);
	pub void setUniformMatrix3MustExist(std::string& name, unsigned int count, Matrix3* value);
	pub void setUniformMatrix4Optional(std::string& name, Matrix4 const* value);
	pub void setUniformMatrix4MustExist(std::string& name, Matrix4 const* value);
	pub void setUniformMatrix4Optional(std::string& name, unsigned int count, Matrix4 const* value);
	pub void setUniformMatrix4MustExist(std::string& name, unsigned int count, Matrix4 const* value);
	pub void setUniform1ivOptional(std::string& name, unsigned int count, const int* value);
	pub void setUniform1ivMustExist(std::string& name, unsigned int count, const int* value);

	prot void disposeMain() override;
	pub ~ShaderProgram() override ;
};
};
