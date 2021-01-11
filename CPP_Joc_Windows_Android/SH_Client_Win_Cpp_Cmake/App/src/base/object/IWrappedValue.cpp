#include "IWrappedValue.h"
#include "WrappedValue.h"
#include <graphics/visual/Color.h>
#include <base/math/Bounds2D.h>
#include <base/math/Vector3.h>
#include <base/math/Vector3Int.h>
#include <base/util/StringUtil.h>

using namespace base;

const int base::IWrappedValue::Type::T_None = 0;

const int base::IWrappedValue::Type::T_int = 1;
const int base::IWrappedValue::Type::T_uint = 2;
const int base::IWrappedValue::Type::T_float = 3;
const int base::IWrappedValue::Type::T_bool = 4;
const int base::IWrappedValue::Type::T_String = 5;

const int base::IWrappedValue::Type::T_Color = 6;
const int base::IWrappedValue::Type::T_Bounds2D = 7;
const int base::IWrappedValue::Type::T_Vector3 = 8;
const int base::IWrappedValue::Type::T_Vector3Int = 9;
const int base::IWrappedValue::Type::T_VisualValueConfig_ptr = 10;
const int base::IWrappedValue::Type::T_MenuVisualAnimationProperty_ptr = 11;
const int base::IWrappedValue::Type::T_float_sharedPtr = 12;
const int base::IWrappedValue::Type::T_String_ptr = 13;
const int base::IWrappedValue::Type::T_void_ptr = 14;

bool IWrappedValue::checkType(std::shared_ptr<IWrappedValue>& o) {
	return checkType(o->getTypeI(), o->getTypeS());
}

int IWrappedValue::getDirectAs_int() { return *getPointerOfTypeAs<int>(Type::T_int); }
unsigned int IWrappedValue::getDirectAs_uint() { return *getPointerOfTypeAs<unsigned int>(Type::T_uint); }

float IWrappedValue::getDirectAs_float() { return *getPointerOfTypeAs<float>(Type::T_float); }

bool IWrappedValue::getDirectAs_bool() { return *getPointerOfTypeAs<bool>(Type::T_bool); }
bool& IWrappedValue::getReferenceAs_bool() { return *getPointerOfTypeAs<bool>(Type::T_bool); }
int& IWrappedValue::getReferenceAs_int() { return *getPointerOfTypeAs<int>(Type::T_int); }
unsigned int& IWrappedValue::getReferenceAs_uint() { return *getPointerOfTypeAs<unsigned int>(Type::T_uint); }

std::string IWrappedValue::getDirectAs_String() { return *getPointerOfTypeAs<std::string>(Type::T_String); }
std::string& IWrappedValue::getReferenceAs_String() { return *getPointerOfTypeAs<std::string>(Type::T_String); }
std::string* IWrappedValue::getPointerOfTypeAs_String() { return getPointerOfTypeAs<std::string>(Type::T_String); }

Color& IWrappedValue::getReferenceAs_Color() { return *getPointerOfTypeAs<Color>(Type::T_Color); }
Color* IWrappedValue::getPointerOfTypeAs_Color() { return getPointerOfTypeAs<Color>(Type::T_Color); }

Bounds2D& IWrappedValue::getReferenceAs_Bounds2D() { return *getPointerOfTypeAs<Bounds2D>(Type::T_Bounds2D); }
Bounds2D* IWrappedValue::getPointerOfTypeAs_Bounds2D() { return getPointerOfTypeAs<Bounds2D>(Type::T_Bounds2D); }

Vector3& IWrappedValue::getReferenceAs_Vector3() { return *getPointerOfTypeAs<Vector3>(Type::T_Vector3); }
Vector3* IWrappedValue::getPointerOfTypeAs_Vector3() { return getPointerOfTypeAs<Vector3>(Type::T_Vector3); }

Vector3Int& IWrappedValue::getReferenceAs_Vector3Int() { return *getPointerOfTypeAs<Vector3Int>(Type::T_Vector3Int); }
Vector3Int* IWrappedValue::getPointerOfTypeAs_Vector3Int() { return getPointerOfTypeAs<Vector3Int>(Type::T_Vector3Int); }

VisualValueConfig* IWrappedValue::getDirectAs_VisualValueConfig_ptr() { return *getPointerOfTypeAs<VisualValueConfig*>(Type::T_VisualValueConfig_ptr); }

MenuVisualAnimationProperty* IWrappedValue::getDirectAs_MenuVisualAnimationProperty_ptr() { return *getPointerOfTypeAs<MenuVisualAnimationProperty*>(Type::T_MenuVisualAnimationProperty_ptr); }

std::shared_ptr<float> IWrappedValue::getDirectAs_float_sharedPtr() { return *getPointerOfTypeAs<std::shared_ptr<float>>(Type::T_float_sharedPtr); }
std::shared_ptr<float>& IWrappedValue::getReferenceAs_float_sharedPtr() { return *getPointerOfTypeAs<std::shared_ptr<float>>(Type::T_float_sharedPtr); }

std::string* IWrappedValue::getDirectAs_String_ptr() { return *getPointerOfTypeAs<std::string*>(Type::T_String_ptr); }

void* IWrappedValue::getDirectAs_void_ptr() { return *getPointerOfTypeAs<std::string*>(Type::T_void_ptr); }

bool IWrappedValue::equals(std::shared_ptr<IWrappedValue>& o) {
	if(!checkType(o->getTypeI(), o->getTypeS())) {
		return false;
	}

	if(getTypeI() == Type::T_int) {
		return getDirectAs_int() == o->getDirectAs_int();
	}else if(getTypeI() == Type::T_uint) {
		return getDirectAs_uint() == o->getDirectAs_uint();
	}else if(getTypeI() == Type::T_float) {
		return getDirectAs_float() == o->getDirectAs_float();
	}else if(getTypeI() == Type::T_bool) {
		return getDirectAs_bool() == o->getDirectAs_bool();
	}else if(getTypeI() == Type::T_String) {
		return getReferenceAs_String() == o->getReferenceAs_String();
	}else {
		// Implement when needed.
		throw LogicException(LOC);
	}
}

std::shared_ptr<IWrappedValue> IWrappedValue::clone() {
	if(getTypeI() == Type::T_int) {
		return new_int(getDirectAs_int());
	}else if(getTypeI() == Type::T_uint) {
		return new_uint(getDirectAs_uint());
	}else if(getTypeI() == Type::T_float) {
		return new_float(getDirectAs_float());
	}else if(getTypeI() == Type::T_bool) {
		return new_bool(getDirectAs_bool());
	}else if(getTypeI() == Type::T_String) {
		return new_String(getReferenceAs_String());
	}else {
		// Implement when needed.
		throw LogicException(LOC);
	}
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_void_ptr(void* data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<void*>>(data, Type::T_void_ptr));
}

std::shared_ptr<IWrappedValue> IWrappedValue::NewFromString(std::string& str) {
	// Split str by ':';
	std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(str, ":", false);

	int cLineIndex = 0;
	std::string& typeName = partsList->getReference(cLineIndex++);

	// Call creator with type.
	if (typeName == "int") {
		return WrappedValue<int>::NewSpFromStringParts(partsList, cLineIndex);
	}else if (typeName == "uint") {
		return WrappedValue<unsigned int>::NewSpFromStringParts(partsList, cLineIndex);
	}else if (typeName == "bool") {
		return WrappedValue<bool>::NewSpFromStringParts(partsList, cLineIndex);
	}else if (typeName == "float") {
		return WrappedValue<float>::NewSpFromStringParts(partsList, cLineIndex);
	}else if (typeName == "Vector3") {
		return WrappedValue<Vector3>::NewSpFromStringParts(partsList, cLineIndex);
	}else if (typeName == "Vector3Int") {
		return WrappedValue<Vector3Int>::NewSpFromStringParts(partsList, cLineIndex);
	} else {
		// Implement when needed.
		throw LogicException(LOC);
	}
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_int(int data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<int>>(data, Type::T_int));
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_uint(unsigned int data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<unsigned int>>(data, Type::T_uint));
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_float(float data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<float>>(data, Type::T_float));
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_bool(bool data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<bool>>(data, Type::T_bool));
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_String(std::string& data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<std::string>>(data, Type::T_String));
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_Color(Color& data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<Color>>(data, Type::T_Color));
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_ColorDirect(Color data) {
	return new_Color(data);
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_Bounds2D(Bounds2D& data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<Bounds2D>>(data, Type::T_Bounds2D));
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_Bounds2DDirect(Bounds2D data) {
	return new_Bounds2D(data);
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_Vector3(Vector3& data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<Vector3>>(data, Type::T_Vector3));
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_Vector3Direct(Vector3 data) {
	return new_Vector3(data);
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_Vector3Int(Vector3Int& data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<Vector3Int>>(data, Type::T_Vector3Int));
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_VisualValueConfig_ptr(VisualValueConfig* data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<VisualValueConfig*>>(data, Type::T_VisualValueConfig_ptr));
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_MenuVisualAnimationProperty_ptr(MenuVisualAnimationProperty* data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<MenuVisualAnimationProperty*>>(data, Type::T_MenuVisualAnimationProperty_ptr));
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_float_sharedPtr(std::shared_ptr<float> data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<std::shared_ptr<float>>>(data, Type::T_float_sharedPtr));
}

std::shared_ptr<IWrappedValue> IWrappedValue::new_String_ptr(std::string* data) {
	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<std::string*>>(data, Type::T_String_ptr));
}
