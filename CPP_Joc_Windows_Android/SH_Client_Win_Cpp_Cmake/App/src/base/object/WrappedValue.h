#pragma once

#include <base/gh.h>
#include "IWrappedValue.h"
#include <string>
#include <base/exceptions/LogicException.h>
#include <base/exceptions/NotImplementedException.h>
#include <iomanip>
#include <memory>
#include <base/math/Vector3.h>
#include <base/math/Vector3Int.h>
#include <base/list/ArrayList.h>
#include <base/util/StringUtil.h>

namespace base {
template <typename T>
class WrappedValue : virtual public IWrappedValue {pub dCtor(WrappedValue);
	priv int typeI;
	priv std::string typeS;

	priv T data;

	pub explicit WrappedValue(T& data, int typeI);
	pub explicit WrappedValue(T& data, std::string& typeS);

	pub bool checkType(int typeI) final;
	pub bool checkType(std::string& typeS) final;
	prot bool checkType(int typeI, std::string& typeS) final;

	prot int getTypeI() final;
	prot std::string& getTypeS() final;

	pub void* getRaw() override;

	pub T& getReference();

	pub void setReference(T& val);
	pub void setDirect(T val);

	pub static std::shared_ptr<base::WrappedValue<T>> NewSPDirect(T data, int typeI);
	pub static std::shared_ptr<base::WrappedValue<T>> NewSPDirect(T data, std::string& typeS);
	
	pub static std::shared_ptr<base::WrappedValue<T>> NewSPReference(T& data, int typeI);
	pub static std::shared_ptr<base::WrappedValue<T>> NewSPReference(T& data, std::string& typeS);

	pub void toString(std::stringstream& ss, bool includeTypePrefix) final;

	pub static std::shared_ptr<IWrappedValue> NewSpFromStringParts(std::shared_ptr<ArrayList<std::string>>& partsList, int cPartIndex);

	pub ~WrappedValue() override;

};

template <typename T>
WrappedValue<T>::WrappedValue(T& data, int typeI)
	: typeI(typeI), typeS(""), data(data)
{
	//void
}

template <typename T>
WrappedValue<T>::WrappedValue(T& data, std::string& typeS)
	: typeI(Type::T_None), typeS(typeS), data(data)
{
	//void
}

template <typename T>
bool WrappedValue<T>::checkType(int typeI) {
	if (!typeS.empty()) {
		throw LogicException(LOC);
	}

	return this->typeI == typeI ? true : false;
};

template <typename T>
bool WrappedValue<T>::checkType(std::string& typeS) {
	if (typeI != Type::T_None) {
		throw LogicException(LOC);
	}

	return this->typeS == typeS ? true : false;
};

template <typename T>
bool WrappedValue<T>::checkType(int typeI, std::string& typeS) {
	return (this->typeI == typeI && this->typeS == typeS) ? true : false;
};

template <typename T>
int WrappedValue<T>::getTypeI() {
	return typeI;
};

template <typename T>
std::string& WrappedValue<T>::getTypeS() {
	return typeS;
};

template <typename T>
void* WrappedValue<T>::getRaw() {
	return &data;
};

template <typename T>
T& WrappedValue<T>::getReference() {
	return data;
}

template <typename T>
void WrappedValue<T>::setReference(T& val) {
	data = val;
}

template <typename T>
void WrappedValue<T>::setDirect(T val) {
	data = val;
}

template <typename T>
std::shared_ptr<base::WrappedValue<T>> WrappedValue<T>::NewSPDirect(T data, int typeI) {
	return std::make_shared<base::WrappedValue<T>>(data, typeI);
}

template <typename T>
std::shared_ptr<base::WrappedValue<T>> WrappedValue<T>::NewSPDirect(T data, std::string& typeS) {
	return std::make_shared<base::WrappedValue<T>>(data, typeS);
}

template <typename T>
std::shared_ptr<base::WrappedValue<T>> WrappedValue<T>::NewSPReference(T& data, int typeI) {
	return std::make_shared<base::WrappedValue<T>>(data, typeI);
}

template <typename T>
std::shared_ptr<base::WrappedValue<T>> WrappedValue<T>::NewSPReference(T& data, std::string& typeS) {
	return std::make_shared<base::WrappedValue<T>>(data, typeS);
}

template<typename T>
void WrappedValue<T>::toString(std::stringstream& ss, bool includeTypePrefix) {
	throw NotImplementedException();
};

// Note. The inline keyword is required even if in a .cpp file or the specialisations won't be used. A compile warning will occur to notify that.
/// The problem seems to be that these full specialisation are no longer template.
/// Placing these in the .cpp file should work and worked but on another compilation it didn't work. Keep these in this .h file.
template<>
inline void WrappedValue<int>::toString(std::stringstream& ss, bool includeTypePrefix) {
	if (includeTypePrefix) {
		ss << "int:";
	}

	ss << data;
};

template<>
inline void WrappedValue<unsigned int>::toString(std::stringstream& ss, bool includeTypePrefix) {
	if (includeTypePrefix) {
		ss << "uint:";
	}

	ss << data;
};

template<>
inline void WrappedValue<bool>::toString(std::stringstream& ss, bool includeTypePrefix) {
	if (includeTypePrefix) {
		ss << "bool:";
	}

	if (data == true) {
		ss << "true";
	} else {
		ss << "false";
	}
};

template<>
inline void WrappedValue<float>::toString(std::stringstream& ss, bool includeTypePrefix) {
	if (includeTypePrefix) {
		ss << "float:";
	}

	// Set the max precission for floats.
	ss << std::setprecision(7) << data;
};

template<>
inline void WrappedValue<Vector3>::toString(std::stringstream& ss, bool includeTypePrefix) {
	if (includeTypePrefix) {
		ss << "Vector3:";
	}

	// Set the max precission for floats.
	ss << std::setprecision(7) << data.x << ":" << data.y << ":" << data.z;
};

template<>
inline void WrappedValue<Vector3Int>::toString(std::stringstream& ss, bool includeTypePrefix) {
	if (includeTypePrefix) {
		ss << "Vector3Int:";
	}

	// Set the max precission for floats.
	ss << data.x << ":" << data.y << ":" << data.z;
};

template <typename T>
std::shared_ptr<IWrappedValue> WrappedValue<T>::NewSpFromStringParts(std::shared_ptr<ArrayList<std::string>>& partsList, int cPartIndex) {
	throw NotImplementedException();
};

template<>
inline std::shared_ptr<IWrappedValue> WrappedValue<int>::NewSpFromStringParts(std::shared_ptr<ArrayList<std::string>>& partsList, int cPartIndex) {
	int val = StringUtil::parseInt(partsList->getReference(cPartIndex++));

	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<int>>(val, Type::T_int));
};

template<>
inline std::shared_ptr<IWrappedValue> WrappedValue<unsigned int>::NewSpFromStringParts(std::shared_ptr<ArrayList<std::string>>& partsList, int cPartIndex) {
	unsigned int val = StringUtil::parseUInt(partsList->getReference(cPartIndex++));

	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<unsigned int>>(val, Type::T_uint));
};

template<>
inline std::shared_ptr<IWrappedValue> WrappedValue<bool>::NewSpFromStringParts(std::shared_ptr<ArrayList<std::string>>& partsList, int cPartIndex) {
	bool val = StringUtil::parseBool(partsList->getReference(cPartIndex++));

	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<bool>>(val, Type::T_bool));
};

template<>
inline std::shared_ptr<IWrappedValue> WrappedValue<float>::NewSpFromStringParts(std::shared_ptr<ArrayList<std::string>>& partsList, int cPartIndex) {
	float val = StringUtil::parseFloat(partsList->getReference(cPartIndex++));

	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<float>>(val, Type::T_float));
};

template<>
inline std::shared_ptr<IWrappedValue> WrappedValue<Vector3>::NewSpFromStringParts(std::shared_ptr<ArrayList<std::string>>& partsList, int cPartIndex) {
	float valX = StringUtil::parseFloat(partsList->getReference(cPartIndex++));
	float valY = StringUtil::parseFloat(partsList->getReference(cPartIndex++));
	float valZ = StringUtil::parseFloat(partsList->getReference(cPartIndex++));
	Vector3 val{
		valX,
		valY,
		valZ
	};

	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<Vector3>>(val, Type::T_Vector3));
};

template<>
inline std::shared_ptr<IWrappedValue> WrappedValue<Vector3Int>::NewSpFromStringParts(std::shared_ptr<ArrayList<std::string>>& partsList, int cPartIndex) {
	int valX = StringUtil::parseInt(partsList->getReference(cPartIndex++));
	int valY = StringUtil::parseInt(partsList->getReference(cPartIndex++));
	int valZ = StringUtil::parseInt(partsList->getReference(cPartIndex++));
	Vector3Int val{
		valX,
		valY,
		valZ
	};

	return std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<Vector3Int>>(val, Type::T_Vector3Int));
};

template <typename T>
WrappedValue<T>::~WrappedValue() {
	//void
}

};

template<typename T>
using WValue = base::WrappedValue<T>;
