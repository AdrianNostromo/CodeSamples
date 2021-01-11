#pragma once

#include <base/gh.h>
#include <base/exceptions/LogicException.h>
#include <memory>
#include <string>
#include <sstream>

class Color;
class Bounds2D;
class Vector3;
class Vector3Int;
class VisualValueConfig;
class MenuVisualAnimationProperty;

namespace base {
	template <typename T>
	class WrappedValue;
}

namespace base {
class IWrappedValue {
	pub class Type {
		pub static const int T_None;

		pub static const int T_int;
		pub static const int T_uint;
		pub static const int T_float;
		pub static const int T_bool;
		pub static const int T_String;

		pub static const int T_Color;
		pub static const int T_Bounds2D;
		pub static const int T_Vector3;
		pub static const int T_Vector3Int;
		pub static const int T_VisualValueConfig_ptr;
		pub static const int T_MenuVisualAnimationProperty_ptr;
		pub static const int T_float_sharedPtr;
		pub static const int T_String_ptr;
		pub static const int T_void_ptr;

	};
	
	pub virtual bool checkType(int typeI) = 0;
	pub virtual bool checkType(std::string& typeS) = 0;
	prot virtual bool checkType(int typeI, std::string& typeS) = 0;
	pub bool checkType(std::shared_ptr<IWrappedValue>& o);

	prot virtual int getTypeI() = 0;
	prot virtual std::string& getTypeS() = 0;

	pub virtual void* getRaw() = 0;

	pub template <typename T>
	T getDirectAs(int validationType);

	pub template <typename T>
	T& getReferenceAs(int validationType);

	pub template <typename T>
	T* getPointerOfTypeAs(int validationType);

	pub template <typename T>
	T getDirectAs(std::string& validationType);

	pub template <typename T>
	T& getReferenceAs(std::string& validationType);

	pub template <typename T>
	T* getPointerOfTypeAs(std::string& validationType);

	pub int getDirectAs_int();
	pub unsigned int getDirectAs_uint();

	pub float getDirectAs_float();

	pub bool getDirectAs_bool();
	pub bool& getReferenceAs_bool();
	pub int& getReferenceAs_int();
	pub unsigned int& getReferenceAs_uint();

	pub std::string getDirectAs_String();
	pub std::string& getReferenceAs_String();
	pub std::string* getPointerOfTypeAs_String();

	pub Color& getReferenceAs_Color();
	pub Color* getPointerOfTypeAs_Color();
	
	pub Bounds2D& getReferenceAs_Bounds2D();
	pub Bounds2D* getPointerOfTypeAs_Bounds2D();
	
	pub Vector3& getReferenceAs_Vector3();
	pub Vector3* getPointerOfTypeAs_Vector3();
	
	pub Vector3Int& getReferenceAs_Vector3Int();
	pub Vector3Int* getPointerOfTypeAs_Vector3Int();

	pub VisualValueConfig* getDirectAs_VisualValueConfig_ptr();

	pub MenuVisualAnimationProperty* getDirectAs_MenuVisualAnimationProperty_ptr();

	pub std::shared_ptr<float> getDirectAs_float_sharedPtr();
	pub std::shared_ptr<float>& getReferenceAs_float_sharedPtr();

	pub std::string* getDirectAs_String_ptr();
	
	pub void* getDirectAs_void_ptr();

	// validationCustomType : a localised value <0;
	pub template <typename T>
	T getDirectAs_Custom(int validationCustomType);
	pub template <typename T>
	T getDirectAs_Custom(std::string& validationCustomType);
	pub template <typename T>
	T getDirectAs_CustomB(std::string validationCustomType);

	pub bool equals(std::shared_ptr<IWrappedValue>& o);
	pub std::shared_ptr<IWrappedValue> clone();

	pub static std::shared_ptr<IWrappedValue> NewFromString(std::string& str);

	pub static std::shared_ptr<IWrappedValue> new_int(int data);
	pub static std::shared_ptr<IWrappedValue> new_uint(unsigned int data);
	pub static std::shared_ptr<IWrappedValue> new_float(float data);
	pub static std::shared_ptr<IWrappedValue> new_bool(bool data);
	pub static std::shared_ptr<IWrappedValue> new_String(std::string& data);
	pub static std::shared_ptr<IWrappedValue> new_Color(Color& data);
	pub static std::shared_ptr<IWrappedValue> new_ColorDirect(Color data);
	pub static std::shared_ptr<IWrappedValue> new_Bounds2D(Bounds2D& data);
	pub static std::shared_ptr<IWrappedValue> new_Bounds2DDirect(Bounds2D data);
	pub static std::shared_ptr<IWrappedValue> new_Vector3(Vector3& data);
	pub static std::shared_ptr<IWrappedValue> new_Vector3Direct(Vector3 data);
	pub static std::shared_ptr<IWrappedValue> new_Vector3Int(Vector3Int& data);
	pub static std::shared_ptr<IWrappedValue> new_VisualValueConfig_ptr(VisualValueConfig* data);
	pub static std::shared_ptr<IWrappedValue> new_MenuVisualAnimationProperty_ptr(MenuVisualAnimationProperty* data);
	pub static std::shared_ptr<IWrappedValue> new_float_sharedPtr(std::shared_ptr<float> data);
	pub static std::shared_ptr<IWrappedValue> new_String_ptr(std::string* data);
	pub static std::shared_ptr<IWrappedValue> new_void_ptr(void* data);
	
	pub template <typename T>
	static std::shared_ptr<IWrappedValue> new_Custom(T data, int localisedType);
	pub template <typename T>
	static std::shared_ptr<IWrappedValue> new_Custom(T data, std::string& localisedType);
	pub template <typename T>
	static std::shared_ptr<IWrappedValue> new_CustomB(T data, std::string localisedType);

	pub virtual void toString(std::stringstream& ss, bool includeTypePrefix) = 0;

	pub virtual ~IWrappedValue() = default;
};

template <typename T>
T IWrappedValue::getDirectAs(int validationType) {
	return *getPointerOfTypeAs<T>(validationType);
}

template <typename T>
T& IWrappedValue::getReferenceAs(int validationType) {
	return *getPointerOfTypeAs<T>(validationType);
}

template <typename T>
T* IWrappedValue::getPointerOfTypeAs(int validationType) {
	if(!checkType(validationType)) {
		throw LogicException(LOC);
	}

	return ((T*)getRaw());
}

template <typename T>
T IWrappedValue::getDirectAs(std::string& validationType) {
	return *getPointerOfTypeAs<T>(validationType);
}

template <typename T>
T& IWrappedValue::getReferenceAs(std::string& validationType) {
	return *getPointerOfTypeAs<T>(validationType);
}

template <typename T>
T* IWrappedValue::getPointerOfTypeAs(std::string& validationType) {
	if(!checkType(validationType)) {
		throw LogicException(LOC);
	}

	return ((T*)getRaw());
}

template <typename T>
T IWrappedValue::getDirectAs_Custom(int validationCustomType) {
	return *getPointerOfTypeAs<T>(validationCustomType);
}

template <typename T>
T IWrappedValue::getDirectAs_Custom(std::string& validationCustomType) {
	return *getPointerOfTypeAs<T>(validationCustomType);
}

template <typename T>
T IWrappedValue::getDirectAs_CustomB(std::string validationCustomType) {
	return *getPointerOfTypeAs<T>(validationCustomType);
}

template <typename T>
std::shared_ptr<IWrappedValue> IWrappedValue::new_Custom(T data, int localisedType) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<T>>(data, localisedType));
}

template <typename T>
std::shared_ptr<IWrappedValue> IWrappedValue::new_Custom(T data, std::string& localisedType) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<T>>(data, localisedType));
}

template <typename T>
std::shared_ptr<IWrappedValue> IWrappedValue::new_CustomB(T data, std::string localisedType) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<T>>(data, localisedType));
}

};

using IWValue = base::IWrappedValue;
